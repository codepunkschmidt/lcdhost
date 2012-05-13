
#include "LogitechG19Thread.h"
#include "libusb.h"
#include <QDebug>
#include <QTime>
#include <QMutex>

LogitechG19Thread::LogitechG19Thread(QObject *parent) : QThread(parent)
{
    time_to_die_ = false;
}

bool LogitechG19Thread::waitToDie(int secs)
{
    for( int i=0; !time_to_die_ && i<(secs*10); ++i ) msleep(100);
    return time_to_die_;
}

void LogitechG19Thread::run()
{
    static QMutex mtx_;

    mtx_.lock();

    while( !waitToDie(2) )
    {
        libusb_context *ctx = 0;
        if( !libusb_init(&ctx) && ctx )
        {
            libusb_set_debug( ctx, 1 );
            while( !waitToDie(1) )
            {
                struct libusb_device_descriptor dd;
                libusb_device *g19dev = 0;
                libusb_device **device_list = 0;
                int device_count = libusb_get_device_list( ctx, &device_list );
                if( device_list )
                {
                    for( int i = 0; g19dev == 0 && !time_to_die_ && i < device_count; ++i )
                    {
                        if( !libusb_get_device_descriptor( device_list[i], &dd ) )
                        {
                            if( dd.idVendor == 0x046d && dd.idProduct == 0xc229 ) // Logitech G19
                                g19dev = device_list[i];
                        }
                        if( g19dev != device_list[i] )
                            libusb_unref_device( device_list[i] );
                        device_list[i] = 0;
                    }
                    libusb_free_device_list( device_list, 0 );
                }

                if( g19dev )
                {
                    struct timeval tv = { 0, 1000*100 };
                    LogitechG19 *the_g19 = new LogitechG19( ctx, g19dev, &dd );
                    the_g19->arrive();
                    while( !time_to_die_ && the_g19 && ! the_g19->offline() )
                    {
                        if( libusb_handle_events_timeout( ctx, & tv ) )
                            break;
                    }
                    the_g19->leave();
                    delete the_g19;
                    libusb_unref_device( g19dev );
                }
            }
            libusb_exit(ctx);
            ctx = 0;
        }
    }
    mtx_.unlock();
}
