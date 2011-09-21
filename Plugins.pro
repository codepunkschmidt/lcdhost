TARGET = Plugins
TEMPLATE = subdirs
CONFIG += ordered

include($$PWD\linkdata\linkdata.pri)
include($$PWD\codeleap\codeleap.pri)

SUBDIRS = $$SIRREAL $$TRISCOPIC
