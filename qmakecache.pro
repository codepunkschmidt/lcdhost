# Generates a .qmake.cache file in the build directory from .qmake.cache.in

TEMPLATE = subdirs
SUBDIRS =
QMAKE_SUBSTITUTES += .qmake.cache.in
