#include "LH_NowPlayingThread.h"
#include "LH_NowPlayingReader.h"

void LH_NowPlayingThread::run()
{
    LH_NowPlayingReader *reader;
    reader = new LH_NowPlayingReader();
    exec();
    delete reader;
}
