#include <QSharedMemory>
#include "LH_CursorInstance.h"

void LH_CursorInstance::getCursorData( cursorData& data ) const
{
    QSharedMemory shmem("LHCursorSharedMemory");
    memset( &data, 0, sizeof(cursorData) );
    if( shmem.attach(QSharedMemory::ReadOnly) )
    {
        if( shmem.lock() )
        {
            int to_copy = sizeof(cursorData);
            if( to_copy > shmem.size() ) to_copy = shmem.size();
            memcpy( &data, shmem.constData(), to_copy );
            shmem.unlock();
        }
        shmem.detach();
    }
    return;
}

void LH_CursorInstance::setCursorData( const cursorData& data ) const
{
    QSharedMemory shmem("LHCursorSharedMemory");
    if( shmem.attach(QSharedMemory::ReadWrite) )
    {
        if( shmem.lock() )
        {
            int to_copy = sizeof(cursorData);
            if( to_copy > shmem.size() ) to_copy = shmem.size();
            memcpy( shmem.data(), &data, to_copy );
            shmem.unlock();
        }
        shmem.detach();
    }
}
