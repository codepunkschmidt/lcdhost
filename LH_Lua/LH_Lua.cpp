
#include "LH_Lua.h"

/*
  This is a butt-ugly hack until Qt gets it's act together and exports it's deeply
  hidden CTFontRef using the therefore intended handle() method.
  http://bugreports.qt.nokia.com/browse/QTBUG-17890
  */
#ifdef Q_WS_MAC
#define private public
#include <QFont>
#include "private/qfontengine_p.h"
#undef private
#include <ApplicationServices/ApplicationServices.h>
#include <cairo-quartz.h>
#endif

#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QFont>
#include <QByteArray>
#include <QColor>
#include <QSettings>
#include <QHash>
#include <QImage>
#include <QPainter>
#include <QThread>

#include "LH_LuaClass.h"
#include "LH_LuaInstance.h"

#include <cairo.h>

int luaopen_xt(lua_State *L); // from luaXT.cpp
extern "C" int luaopen_lcairo(lua_State* L);

#ifndef LUA_STATIC
# include "lua_dyn.c"
lua_All_functions LuaFunctions;
#endif

#ifdef Q_WS_WIN
#include <windows.h>
#include <cairo-win32.h>
#endif

static void my_setenv(const char *name, const char *value )
{
#ifdef Q_WS_WIN
    int len = strlen(value)+1+strlen(value)+1;
    char *str = (char*) malloc(len);
    if( str )
    {
        sprintf(str, "%s=%s", name, value);
        putenv(str);
    }
#else
    setenv( name, value, 1 );
#endif
}

extern "C" int lh_lcdhost_rgba(lua_State *L)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    QColor c;
    c.setRgba( luaL_checkint(L,1) );
    lua_pushnumber(L,c.redF());
    lua_pushnumber(L,c.greenF());
    lua_pushnumber(L,c.blueF());
    lua_pushnumber(L,c.alphaF());
    Q_ASSERT( old_top+4 == lua_gettop(L) );
    return 4;
}

extern "C" int lh_lcdhost_fontsize(lua_State *L)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    double retv = 0.0;

    // extract font size from QFont.toString()
    if( lua_isstring(L,-1) )
    {
        double points, pixels;
        QByteArray str;
        str.append(lua_tostring(L,-1));
        QList<QByteArray> list = str.split(',');
        if( list.size() > 2 )
        {
            points = atof( list.at(1).constData() );
            pixels = atof( list.at(2).constData() );
            if( points > 0 ) retv = points;
            else retv = pixels;
        }
        else
            qDebug() << "lh_lua_fontsize:" << str << "yields" << list.size() << "items";
    }

    if( retv == 0.0 )
    {
        QFont font;
        retv = font.pointSizeF();
    }

    lua_pushnumber(L,retv);
    Q_ASSERT( old_top+1 == lua_gettop(L) );
    return 1;
}

// cs, x, y, fontdata, color, text
extern "C" int lh_lua_draw_text(lua_State *L)
{
    size_t flen = 0;
    size_t slen = 0;

    if( !lua_isuserdata(L,1) ) return 0;

    cairo_surface_t *cs = (cairo_surface_t *) lua_touserdata(L,1);
    int x = luaL_checkinteger(L,2);
    int y = luaL_checkinteger(L,3);
    const char *f = luaL_checklstring(L,4,&flen);
    int c = luaL_checkinteger(L,5);
    const char *s = luaL_checklstring(L,6,&slen);

    QImage img(
            (uchar*) cairo_image_surface_get_data(cs),
            cairo_image_surface_get_width(cs),
            cairo_image_surface_get_height(cs),
            QImage::Format_ARGB32_Premultiplied
            );
    img.setDotsPerMeterX(3780);
    img.setDotsPerMeterY(3780);

    QPainter p;
    if( p.begin(&img) )
    {
        QFont font;

        font.fromString( QString::fromUtf8(f,flen) );
        QFont::StyleStrategy wanted_strategy = QFont::PreferAntialias;
        if( !(font.styleStrategy() & wanted_strategy) ) font.setStyleStrategy( wanted_strategy );

        p.setFont( font );
        p.setPen( QColor::fromRgba(c) );
        p.drawText(x,y,QString::fromUtf8(s,slen) );
        p.end();
    }

    return 0;
}

extern "C" int lh_lua_font_face_create(lua_State *L)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    QFont font;
    cairo_font_face_t *v = 0;

    if( lua_isstring(L,-1) )
    {
        QString str(lua_tostring(L,-1));
        if( !str.isEmpty() ) font.fromString( str );
    }

#ifdef Q_WS_WIN
    v = cairo_win32_font_face_create_for_hfont ((HFONT)font.handle());
#endif
#ifdef Q_WS_MAC
#ifdef __LP64__
    {
        // Until http://bugreports.qt.nokia.com/browse/QTBUG-17890 gets fixed
        QFontEngine *fe = font.d->engineForScript(QUnicodeTables::Common);
        if (fe && fe->type() == QFontEngine::Mac)
        {
            QCoreTextFontEngine *cte = static_cast<QCoreTextFontEngine*>(fe);
            if( cte ) v = cairo_quartz_font_face_create_for_cgfont( cte->cgFont );
        }
    }
#else
    v = cairo_quartz_font_face_create_for_atsu_font_id ((ATSUFontID)font.macFontID());
#endif
#endif

    lua_pushlightuserdata(L,v);

    Q_ASSERT( old_top+1 == lua_gettop(L) );
    return 1;
}

extern "C" int lh_lua_gettop(lua_State *L)
{
    lua_pushinteger(L,lua_gettop(L));
    return 1;
}

extern "C" int lh_lua_print(lua_State *L)
{
    QByteArray output;
    int n = lua_gettop(L);    /* number of arguments */
    int i;
    for( i = 1; i <= n; i++ )
    {
        // call tostring() on it
        lua_getglobal(L,"tostring");
        lua_pushvalue(L,i);
        lua_pcall(L,1,1,0);
        if( lua_isstring(L, -1) )
            output.append( lua_tostring(L,-1) );
        lua_pop(L,1);
    }
    output.replace( '&', "&amp;" );
    output.replace( '<', "&lt;" );
    output.replace( '>', "&gt;" );
    output.replace( ' ', "&nbsp;" );
    output.replace( '\t', "\\t" );
    QList<QByteArray> lines = output.split('\n');
    foreach( QByteArray line, lines )
    {
        qDebug( "LH_Lua: &nbsp;<tt>%s</tt>", line.constData() );
    }
    Q_ASSERT( lua_gettop(L) == n );
    return 0; // no return values
}

LH_Lua::LH_Lua(QObject *parent) : LH_QtObject( &obj_, "LH_Lua", parent )
{
    QString lua_cpath;
    QString lua_path;
    QString path;
    const char *env_lua_cpath;
    const char *env_lua_path;
    const char *env_path;
    QString dir_data = LH_QtPlugin::instance()->dir_data();
    QString dir_plugins = LH_QtPlugin::instance()->dir_plugins();

    // Create DATADIR/lua/clibs if it doesn't exist
    {
        QDir datadir( dir_data );
        if( !datadir.exists(LUADIR "/clibs") ) datadir.mkpath(LUADIR "/clibs");
    }

    // Add our paths to the environment if they're not already there
    env_lua_cpath = getenv("LUA_CPATH");
    if( !QString::fromLocal8Bit(env_lua_cpath).contains( dir_data ) &&
        !QString::fromLocal8Bit(env_lua_cpath).contains( dir_plugins )
        )
    {
        lua_cpath.append( dir_data );
#ifdef Q_OS_MAC
        lua_cpath.append( "lua/clibs/?.so;" );
#endif
#ifdef Q_OS_WIN
        lua_cpath.append( "lua/clibs/?.dll;" );
#endif
        lua_cpath.append( dir_plugins );
#ifdef Q_OS_MAC
        lua_cpath.append( "?.so" );
#endif
#ifdef Q_OS_WIN
        lua_cpath.append( "?.dll" );
#endif
        if( env_lua_cpath && *env_lua_cpath )
        {
            lua_cpath.append( ';' );
            lua_cpath.append( QString::fromLocal8Bit(env_lua_cpath) );
        }
    }
    env_lua_path = getenv("LUA_PATH");
    if( !QString::fromLocal8Bit(env_lua_path).contains( dir_data) )
    {
        lua_path.append( dir_data );
        lua_path.append( "lua/?.lua;" );
        lua_path.append( dir_data );
        lua_path.append( "lua/?.luac;" );
        lua_path.append( dir_data );
        lua_path.append( "lua/?/init.lua" );
        if( env_lua_path && *env_lua_path )
        {
            lua_path.append( ';' );
            lua_path.append( QString::fromLocal8Bit(env_lua_path) );
        }
    }

    env_path = getenv("PATH");
    if( !QString::fromLocal8Bit(env_path).contains( dir_data) )
    {
        if( env_path && *env_path )
        {
            path.append( QString::fromLocal8Bit(env_path) );
#ifdef Q_OS_WIN
            path.append( ';' );
#else
            path.append( ':' );
#endif
        }
        path.append( dir_data );
        path.append( "lua/clibs" );
    }

#ifdef Q_OS_WIN
    // Lua for Windows workarounds.
    //  It doesn't set LUA_CPATH
    //  It limits it's LUA_PATH to *.luac
    QByteArray lua_dev;
    lua_dev.append( getenv("LUA_DEV") );
    if( !lua_dev.isEmpty() )
    {
        if( !lua_cpath.isEmpty() )
        {
            lua_cpath.append( ';' );
            lua_cpath.append( lua_dev );
            lua_cpath.append( "\\clibs\\?.dll" );
        }
        if( !lua_path.isEmpty() )
        {
            lua_path.append( ';' );
            lua_path.append( lua_dev );
            lua_path.append( "\\lua\\?.lua;" );
            lua_path.append( lua_dev );
            lua_path.append( "\\lua\\?\\init.lua" );
        }
    }
#endif

    // Set the new environment variables
    {
        QByteArray ary;
        if( !lua_cpath.isEmpty() )
        {
            ary = QDir::toNativeSeparators(lua_cpath).toLocal8Bit();
            my_setenv( "LUA_CPATH", ary.data() );
        }
        if( !lua_path.isEmpty() )
        {
            ary = QDir::toNativeSeparators(lua_path).toLocal8Bit();
            my_setenv( "LUA_PATH", ary.data() );
        }
        if( !path.isEmpty() )
        {
            ary = QDir::toNativeSeparators(path).toLocal8Bit();
            my_setenv( "PATH", ary.data() );
        }
    }

    // Start Lua
    L = luaL_newstate();
    if( L == NULL )
    {
        qCritical() << "luaL_newstate() failed";
        thread()->quit();
        return;
    }

    luaL_openlibs(L);

    // init luaXT for the __pairs metamethod
    luaopen_xt(L);

    // load gd
    // luaopen_gd(L);

    luaopen_lcairo(L);

#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif

    // extend cairo with font_face_create and draw_text
    lua_getglobal(L,"cairo");
    lua_pushliteral(L,"font_face_create");
    lua_pushcclosure(L,lh_lua_font_face_create,0);
    lua_rawset(L,-3);
    lua_pushliteral(L,"draw_text");
    lua_pushcclosure(L,lh_lua_draw_text,0);
    lua_rawset(L,-3);
    lua_pop(L,1);

    // Create the lcdhost global variable
    // and it's fields 'state', 'class' and 'instance'
    lua_newtable(L);
    lua_pushliteral(L,"state");
    lua_newtable(L);
    lua_rawset(L,-3);
    lua_pushliteral(L,"class");
    lua_newtable(L);
    lua_rawset(L,-3);
    lua_pushliteral(L,"instance");
    lua_newtable(L);
    lua_rawset(L,-3);
    lua_pushliteral(L,"gettop");
    lua_pushcclosure(L,lh_lua_gettop,0);
    lua_rawset(L,-3);
    lua_pushliteral(L,"fontsize");
    lua_pushcclosure(L,lh_lcdhost_fontsize,0);
    lua_rawset(L,-3);
    lua_pushliteral(L,"rgba");
    lua_pushcclosure(L,lh_lcdhost_rgba,0);
    lua_rawset(L,-3);
    lua_setglobal(L, "lcdhost");

#if 0
    // set the lcdhost.state metatable
    lua_getglobal(L,"lcdhost");
    lua_pushliteral(L,"state");
    lua_rawget(L,-2);
    lua_newtable(L);
    lua_pushliteral(L,"__index");
    lua_pushcclosure(L,lh_lcdhost_state_index,0);
    lua_rawset(L,-3);
    lua_pushliteral(L,"__newindex");
    lua_pushcclosure(L,lh_block_newindex,0);
    lua_rawset(L,-3);
    lua_pushliteral(L,"__pairs");
    lua_pushcclosure(L,lh_lcdhost_state_pairs,0);
    lua_rawset(L,-3);
    lua_setmetatable(L,-2);
    lua_pop(L,2);
#endif

    // redirect print()
    lua_pushliteral(L,"print");
    lua_pushcclosure(L,lh_lua_print,0);
    lua_rawset(L,LUA_GLOBALSINDEX);

    Q_ASSERT( old_top == lua_gettop(L) );

    // make the font -> filename map
    // lh_make_fontmap();

    luadir_ = dir_data.append("lua");

    // report on Lua version and source
    lua_getglobal(L,"_VERSION");
    if( lua_isstring(L,-1) )
    {
        qDebug() << "LH_Lua: running" << lua_tostring(L,-1);
    }
    lua_pop(L,1);

    scanForFiles();
}

LH_Lua::~LH_Lua()
{
    foreach( QObject *o, children() )
    {
        LH_LuaInstance *ali = qobject_cast<LH_LuaInstance *>(o);
        if( ali ) delete ali;
    }
    foreach( QObject *o, children() )
    {
        LH_LuaClass *alc = qobject_cast<LH_LuaClass *>(o);
        if( alc ) delete alc;
    }
    if( L ) { lua_close(L); L = NULL; }
}

void LH_Lua::loadLuaFile( QFileInfo fi )
{
    QString err = LH_LuaClass::load(this, fi);
    if( !err.isEmpty() ) qWarning() << "LH_Lua:" << err;
    return;
}

void LH_Lua::scanForFiles()
{
    QDir dir(luadir_);

    dir.setFilter( QDir::Files | QDir::Dirs | QDir::Readable | QDir::NoDotAndDotDot );
    foreach( QFileInfo fi, dir.entryInfoList() )
    {
        if( fi.isFile() && (fi.suffix() == "lua" || fi.suffix() == "luac") )
        {
            loadLuaFile(fi);
        }
        else if( fi.isDir() )
        {
            QDir subdir(fi.filePath());
            subdir.setFilter( QDir::Files | QDir::Readable );
            foreach( QFileInfo sfi, subdir.entryInfoList() )
            {
                if( sfi.baseName() == fi.baseName() && (sfi.suffix() == "lua" || sfi.suffix() == "luac") )
                {
                    loadLuaFile(sfi);
                }
            }
        }
    }

    return;
}
