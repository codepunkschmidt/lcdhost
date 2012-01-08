/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



/* Surface manipulation */

// cairo_public cairo_surface_t *
// cairo_surface_create_similar (cairo_surface_t  *other,
//                   cairo_content_t	content,
//                   int		width,
//                   int		height);
static int l_cairo_surface_create_similar(lua_State* L)
{
    cairo_surface_t *other = get_cairo_surface_t (L, 1);
    cairo_content_t content = (cairo_content_t) luaL_checkinteger(L, 2);
    int width = luaL_checkinteger(L, 3);
    int height = luaL_checkinteger(L, 4);
    cairo_surface_t *v = cairo_surface_create_similar(other, content, width, height);
    lua_pushlightuserdata(L, v);
    return 1;
}


//FIXME should not be exposed to the user of the language binding,
//FIXME but rather used to implement memory management within the language binding
// cairo_public cairo_surface_t *
// cairo_surface_reference (cairo_surface_t *surface);
static int l_cairo_surface_reference(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_surface_t *v = cairo_surface_reference(surface);
    lua_pushlightuserdata(L, v);
    return 1;
}


//FIXME should not be exposed to the user of the language binding,
//FIXME but rather used to implement memory management within the language binding
// cairo_public void
// cairo_surface_finish (cairo_surface_t *surface);
static int l_cairo_surface_finish(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_surface_finish(surface);
    return 0;
}


//FIXME should not be exposed to the user of the language binding,
//FIXME but rather used to implement memory management within the language binding
// cairo_public void
// cairo_surface_destroy (cairo_surface_t *surface);
static int l_cairo_surface_destroy(lua_State* L)
{
    cairo_surface_t *surface = (cairo_surface_t *) check_lightuserdata(L, 1);
    cairo_surface_destroy(surface);
    return 0;
}


//FIXME should not be exposed to the user of the language binding,
//FIXME but rather used to implement memory management within the language binding
// cairo_public unsigned int
// cairo_surface_get_reference_count (cairo_surface_t *surface);
static int l_cairo_surface_get_reference_count(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    unsigned int v = cairo_surface_get_reference_count(surface);
    lua_pushnumber(L, v);
    return 1;
}


// cairo_public cairo_status_t
// cairo_surface_status (cairo_surface_t *surface);
static int l_cairo_surface_status(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_status_t v = cairo_surface_status(surface);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public cairo_surface_type_t
// cairo_surface_get_type (cairo_surface_t *surface);
static int l_cairo_surface_get_type(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_surface_type_t v = cairo_surface_get_type(surface);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public cairo_content_t
// cairo_surface_get_content (cairo_surface_t *surface);
static int l_cairo_surface_get_content(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_content_t v = cairo_surface_get_content (surface);
    lua_pushinteger(L, v);
    return 1;
}


#if CAIRO_HAS_PNG_FUNCTIONS

// cairo_public cairo_status_t
// cairo_surface_write_to_png (cairo_surface_t	*surface,
//                 const char		*filename);
static int l_cairo_surface_write_to_png(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    const char *filename = luaL_checkstring(L, 2);
    cairo_status_t v = cairo_surface_write_to_png (surface, filename);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public cairo_status_t
// cairo_surface_write_to_png_stream (cairo_surface_t *surface,
//                    cairo_write_func_t	write_func,
//                    void			*closure);
static int l_cairo_surface_write_to_png_stream(lua_State* L)
{
    //cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}

#endif /* CAIRO_HAS_PNG_FUNCTIONS */


// cairo_public void *
// cairo_surface_get_user_data (cairo_surface_t *surface,
//                  const cairo_user_data_key_t *key);
static int l_cairo_surface_get_user_data(lua_State* L)
{
    //cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public cairo_status_t
// cairo_surface_set_user_data (cairo_surface_t *surface,
//                  const cairo_user_data_key_t *key,
//                  void			 *user_data,
//                  cairo_destroy_func_t	 destroy);
static int l_cairo_surface_set_user_data(lua_State* L)
{
    //cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public void
// cairo_surface_get_font_options (cairo_surface_t *surface,
//                 cairo_font_options_t *options);
static int l_cairo_surface_get_font_options(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_font_options_t *options = get_cairo_font_options_t (L, 2);
    cairo_surface_get_font_options (surface, options);
    return 0;
}


// cairo_public void
// cairo_surface_flush (cairo_surface_t *surface);
static int l_cairo_surface_flush(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_surface_flush(surface);
    return 0;
}


// cairo_public void
// cairo_surface_mark_dirty (cairo_surface_t *surface);
static int l_cairo_surface_mark_dirty(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_surface_mark_dirty (surface);
    return 0;
}


// cairo_public void
// cairo_surface_mark_dirty_rectangle (cairo_surface_t *surface,
//                     int              x,
//                     int              y,
//                     int              width,
//                     int              height);
static int l_cairo_surface_mark_dirty_rectangle(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    int width = luaL_checkinteger(L, 4);
    int height = luaL_checkinteger(L, 5);
    cairo_surface_mark_dirty_rectangle (surface, x, y, width, height);
    return 0;
}


// cairo_public void
// cairo_surface_set_device_offset (cairo_surface_t *surface,
//                  double           x_offset,
//                  double           y_offset);
static int l_cairo_surface_set_device_offset(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    double x_offset = luaL_checknumber(L, 2);
    double y_offset = luaL_checknumber(L, 3);
    cairo_surface_set_device_offset (surface, x_offset, y_offset);
    return 0;
}


// cairo_public void
// cairo_surface_get_device_offset (cairo_surface_t *surface,
//                  double          *x_offset,
//                  double          *y_offset);
static int l_cairo_surface_get_device_offset(lua_State* L)
{
    /// WARN:different usage
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    double x_offset = luaL_optnumber(L, 2, 0.0);
    double y_offset = luaL_optnumber(L, 3, 0.0);
    cairo_surface_get_device_offset (surface, &x_offset, &y_offset);
    lua_pushnumber(L, x_offset);
    lua_pushnumber(L, y_offset);
    return 2;
}


// cairo_public void
// cairo_surface_set_fallback_resolution (cairo_surface_t	*surface,
//                        double		 x_pixels_per_inch,
//                        double		 y_pixels_per_inch);
static int l_cairo_surface_set_fallback_resolution(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    double x_pixels_per_inch = luaL_checknumber(L, 2);
    double y_pixels_per_inch = luaL_checknumber(L, 3);
    cairo_surface_set_fallback_resolution (surface, x_pixels_per_inch, y_pixels_per_inch);
    return 0;
}


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
// cairo_public void
// cairo_surface_get_fallback_resolution (cairo_surface_t	*surface,
//                        double		*x_pixels_per_inch,
//                        double		*y_pixels_per_inch);
static int l_cairo_surface_get_fallback_resolution(lua_State* L)
{
    /// WARN:different usage
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    double x_pixels_per_inch = luaL_checknumber(L, 2);
    double y_pixels_per_inch = luaL_checknumber(L, 3);
    cairo_surface_get_fallback_resolution (surface, &x_pixels_per_inch, &y_pixels_per_inch);
    lua_pushnumber(L, x_pixels_per_inch);
    lua_pushnumber(L, y_pixels_per_inch);
    return 2;
}
#endif


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
// cairo_public void
// cairo_surface_copy_page (cairo_surface_t *surface);
static int l_cairo_surface_copy_page(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_surface_copy_page (surface);
    return 0;
}
#endif


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
// cairo_public void
// cairo_surface_show_page (cairo_surface_t *surface);
static int l_cairo_surface_show_page(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_surface_show_page (surface);
    return 0;
}
#endif


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
// cairo_public cairo_bool_t
// cairo_surface_has_show_text_glyphs (cairo_surface_t *surface);
static int l_cairo_surface_has_show_text_glyphs(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_bool_t v = cairo_surface_has_show_text_glyphs (surface);
    lua_pushboolean(L, v);
    return 1;
}
#endif


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
// cairo_public void
// cairo_surface_get_mime_data (cairo_surface_t		*surface,
//                              const char			*mime_type,
//                              const unsigned char       **data,
//                              unsigned long		*length);
static int l_cairo_surface_get_mime_data (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}
#endif


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
// cairo_public cairo_status_t
// cairo_surface_set_mime_data (cairo_surface_t		*surface,
//                              const char			*mime_type,
//                              const unsigned char	*data,
//                              unsigned long		 length,
//              			    cairo_destroy_func_t	 destroy,
// 			                    void			*closure);
static int l_cairo_surface_set_mime_data (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}
#endif


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
// cairo_public cairo_device_t *
// cairo_surface_get_device (cairo_surface_t *surface);
static int l_cairo_surface_get_device (lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_device_t *v = cairo_surface_get_device (surface);
    lua_pushlightuserdata(L, v);
    return 1;
}
#endif



#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
// cairo_public cairo_surface_t *
// cairo_surface_create_for_rectangle (cairo_surface_t	*target,
//                                     double		 x,
//                                     double		 y,
//                                     double		 width,
//                                     double		 height);
static int l_cairo_surface_create_for_rectangle (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}
#endif



static const struct luaL_Reg surface_f [] = {
    {"surface_create_similar",                   l_cairo_surface_create_similar},
    {"surface_reference",                        l_cairo_surface_reference},
    {"surface_finish",                           l_cairo_surface_finish},
    {"surface_destroy",                          l_cairo_surface_destroy},
    {"surface_get_reference_count",              l_cairo_surface_get_reference_count},
    {"surface_status",                           l_cairo_surface_status},
    {"surface_get_type",                         l_cairo_surface_get_type},
    {"surface_get_content",                      l_cairo_surface_get_content},
#if CAIRO_HAS_PNG_FUNCTIONS
    {"surface_write_to_png",                     l_cairo_surface_write_to_png},
    {"surface_write_to_png_stream",              l_cairo_surface_write_to_png_stream},
#endif
    {"surface_get_user_data",                    l_cairo_surface_get_user_data},
    {"surface_set_user_data",                    l_cairo_surface_set_user_data},
    {"surface_get_font_options",                 l_cairo_surface_get_font_options},
    {"surface_flush",                            l_cairo_surface_flush},
    {"surface_mark_dirty",                       l_cairo_surface_mark_dirty},
    {"surface_mark_dirty_rectangle",             l_cairo_surface_mark_dirty_rectangle},
    {"surface_set_device_offset",                l_cairo_surface_set_device_offset},
    {"surface_get_device_offset",                l_cairo_surface_get_device_offset},
    {"surface_set_fallback_resolution",          l_cairo_surface_set_fallback_resolution},

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    {"surface_copy_page",                        l_cairo_surface_copy_page},
    {"surface_show_page",                        l_cairo_surface_show_page},
#endif

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    {"surface_get_fallback_resolution",          l_cairo_surface_get_fallback_resolution},
    {"surface_has_show_text_glyphs",             l_cairo_surface_has_show_text_glyphs},
#endif

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
    {"surface_create_for_rectangle",             l_cairo_surface_create_for_rectangle},
    {"surface_get_mime_data",                    l_cairo_surface_get_mime_data},
    {"surface_set_mime_data",                    l_cairo_surface_set_mime_data},
    {"surface_get_device",                       l_cairo_surface_get_device},
#endif
    
	{NULL, NULL} /* sentinel */
};



static int luaopen_lcairo_surface(lua_State* L)
{
    // register functions
    luaL_register(L, LUACAIRO, surface_f);
    return 0;
}
