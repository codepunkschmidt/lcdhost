TARGET = Plugins
TEMPLATE = subdirs
CONFIG += ordered

include(SirReal.pri)
include(Triscopic.pri)

SUBDIRS = $$SIRREAL $$TRISCOPIC
