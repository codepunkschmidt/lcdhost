TEMPLATE=subdirs
isEmpty(LH_DIR_SRCROOT): exists(../.qmake.cache.in) {
	LH_DIR_SRCROOT=$$quote($$dirname(PWD))
	LH_DIR_DSTROOT=$$quote($$dirname(OUT_PWD))
	QMAKE_SUBSTITUTES=../.qmake.cache.in
}
SUBDIRS=
