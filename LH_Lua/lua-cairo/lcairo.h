/* -*- C -*-
 *
 * Name:        lcairo.h
 * Purpose:     Lua binding of Cairo
 * Author:      Hakki Dogusan (dogusanh@tr.net)
 * Created:     2007-05-25
 * Copyright:   (c) 2007-2011 Hakki Dogusan
 * License:     MIT (http://www.opensource.org/licenses/mit-license.html)
 * RCS-ID:      $Id$
 *
 */

#ifndef LUA_CAIRO_H
#define LUA_CAIRO_H



#ifdef  __cplusplus
# define LUA_CAIRO_BEGIN_DECLS  extern "C" {
# define LUA_CAIRO_END_DECLS    }
#else
# define LUA_CAIRO_BEGIN_DECLS
# define LUA_CAIRO_END_DECLS
#endif


LUA_CAIRO_BEGIN_DECLS

typedef struct lua_State lua_State;


int luaopen_lcairo(lua_State* L);


LUA_CAIRO_END_DECLS


#endif  /* LUA_CAIRO_H */
