
TARGET = lh_zlib
TEMPLATE = lib
CONFIG += staticlib

load($$[LCDHOST_PRF])

QMAKE_CFLAGS_WARN_ON = -w
DEFINES += USE_FILE32API

HEADERS += zip.h unzip.h

SOURCES += adler32.c compress.c crc32.c deflate.c gzclose.c gzlib.c gzread.c \
    gzwrite.c infback.c inffast.c inflate.c inftrees.c trees.c uncompr.c zutil.c \
    zip.c unzip.c ioapi.c
