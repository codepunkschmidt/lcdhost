README for lcdhost plugin development.

For all platforms:
	LCDHost: http://www.linkdata.se/
	Mercurial: http://mercurial.selenic.com/

For Windows:
	Windows 7 or later
	Visual C++ 2010 Express: http://go.microsoft.com/?linkid=9709949
	Qt 4.8.4 for VS2010: http://releases.qt-project.org/qt4/source/qt-win-opensource-4.8.4-vs2010.exe
	Optionally you can use the Windows SDK 7.1 instead of Visual C++ 2010 Express.

For Mac:
	OS/X Snow Leopard with XCode from Apple's developer site
	 OR
	OS/X Mountain Lion with XCode from the App Store
	Qt 4.8.4 for Mac: http://releases.qt-project.org/qt4/source/qt-mac-opensource-4.8.4.dmg

For Linux (very experimental):
	Linux distro with GCC and OpenGL (tested with Ubuntu 12.04 LTS)
	Qt 4.8.4 for X11: http://releases.qt-project.org/qt4/source/qt-everywhere-opensource-src-4.8.4.tar.gz

You need working knowledge of Mercurial, C++ and Qt.
Qt5 can also be used, but it's not yet quite stable enough for production use.

Download and install LCDHost, development tools and Qt as needed.

Create a directory to develop LCDHost in, create "release" and "debug"
subdirectories and copy the LCDHost binaries into them, excluding the
Qt shared libraries (note that phonon is a Qt shared library).

Check out the LCDHost repository from googlecode.
	hg clone https://lcdhost.googlecode.com/hg/

You should now have a directory structure something like this on Windows:
	LCDHostDev/
		release/
			(contents will vary depending on version)
			LCDHost.exe
			SignPlugin.exe
			TestPlugin.exe
			DistPlugin.exe
			WebKitServer.exe
			lh_hid.dll
			mingwm10.dll
			libgcc_s_dw2-1.dll
			libstdc++-6.dll
		debug/
			(same contents as release/)
		lcdhost/
			(contents will vary depending on version)
			.hg/
			codeleap/
			lh_features/
			linkdata/
			plugins/
			tests/
			.hgtags
			.hgignore
			.qmake.cache.in
			lcdhost.pro
			qmakecache.pro
			README.txt

Open "lcdhost/lcdhost.pro" in Qt Creator, configure it as a shadow build.
Good luck.

/J

