/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



#if CAIRO_HAS_IMAGE_SURFACE


/* Image-surface functions */

// cairo_public cairo_surface_t *
// cairo_image_surface_create (cairo_format_t	format,
//                 int			width,
//                 int			height);
static int l_cairo_image_surface_create(lua_State* L)
{
    cairo_format_t format = (cairo_format_t) luaL_checkinteger(L, 1);
    int width = luaL_checkinteger(L, 2);
    int height = luaL_checkinteger(L, 3);
    cairo_surface_t *v = cairo_image_surface_create (format, width, height);
    lua_pushlightuserdata(L, v);
    return 1;
}


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
// cairo_public int
// cairo_format_stride_for_width (cairo_format_t	format,
//                 int		width);
static int l_cairo_format_stride_for_width(lua_State* L)
{
    cairo_format_t format = (cairo_format_t) luaL_checkinteger(L, 1);
    int width = luaL_checkinteger(L, 2);
    int v = cairo_format_stride_for_width (format, width);
    lua_pushinteger(L, v);
    return 1;
}
#endif


// cairo_public cairo_surface_t *
// cairo_image_surface_create_for_data (unsigned char *data,
//                      cairo_format_t format,
//                      int			width,
//                      int			height,
//                      int			stride);
static int l_cairo_image_surface_create_for_data(lua_State* L)
{
    // Gerald Franz (gerald.franz@googlemail.com) reported that
    // this is not working.
    /*
    const char* data = luaL_checkstring(L, 1);
    cairo_format_t format = (cairo_format_t) luaL_checkinteger(L, 2);
    int width = luaL_checkinteger(L, 3);
    int height = luaL_checkinteger(L, 4);
    int stride = luaL_checkinteger(L, 5);
    // WARN: (unsigned char*)data ?
    cairo_surface_t *v = cairo_image_surface_create_for_data ((unsigned char*)data, format, width, height, stride);
    lua_pushlightuserdata(L, v);
    return 1;
    */

    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public unsigned char *
// cairo_image_surface_get_data (cairo_surface_t *surface);
static int l_cairo_image_surface_get_data(lua_State* L)
{
    // Courtesy of Gerald Franz (gerald.franz@googlemail.com)

    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    unsigned char *data = cairo_image_surface_get_data (surface);
    int stride = cairo_image_surface_get_stride (surface);
    int height = cairo_image_surface_get_height (surface);
    size_t size = stride * height;
    lua_pushlstring(L, data, size);
    return 1;
}


// cairo_public cairo_format_t
// cairo_image_surface_get_format (cairo_surface_t *surface);
static int l_cairo_image_surface_get_format(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_format_t v = cairo_image_surface_get_format (surface);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public int
// cairo_image_surface_get_width (cairo_surface_t *surface);
static int l_cairo_image_surface_get_width(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    int v = cairo_image_surface_get_width(surface);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public int
// cairo_image_surface_get_height (cairo_surface_t *surface);
static int l_cairo_image_surface_get_height(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    int v = cairo_image_surface_get_height(surface);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public int
// cairo_image_surface_get_stride (cairo_surface_t *surface);
static int l_cairo_image_surface_get_stride(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    int v = cairo_image_surface_get_stride(surface);
    lua_pushinteger(L, v);
    return 1;
}


#if CAIRO_HAS_PNG_FUNCTIONS

// cairo_public cairo_surface_t *
// cairo_image_surface_create_from_png (const char	*filename);
static int l_cairo_image_surface_create_from_png(lua_State* L)
{
    const char *filename = luaL_checkstring(L, 1);
    cairo_surface_t *v = cairo_image_surface_create_from_png(filename);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public cairo_surface_t *
// cairo_image_surface_create_from_png_stream (cairo_read_func_t	read_func,
//                         void		*closure);
static int l_cairo_image_surface_create_from_png_stream(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

#endif /* CAIRO_HAS_PNG_FUNCTIONS */



static const struct luaL_Reg image_surface_f [] = {
    {"image_surface_create",                     l_cairo_image_surface_create},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    {"format_stride_for_width",                  l_cairo_format_stride_for_width},
#endif
    {"image_surface_create_for_data",            l_cairo_image_surface_create_for_data},
    {"image_surface_get_data",                   l_cairo_image_surface_get_data},
    {"image_surface_get_format",                 l_cairo_image_surface_get_format},
    {"image_surface_get_width",                  l_cairo_image_surface_get_width},
    {"image_surface_get_height",                 l_cairo_image_surface_get_height},
    {"image_surface_get_stride",                 l_cairo_image_surface_get_stride},
#if CAIRO_HAS_PNG_FUNCTIONS
    {"image_surface_create_from_png",            l_cairo_image_surface_create_from_png},
    {"image_surface_create_from_png_stream",     l_cairo_image_surface_create_from_png_stream},
#endif
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_IMAGE_SURFACE */



static int luaopen_lcairo_image_surface(lua_State* L)
{
#if CAIRO_HAS_IMAGE_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, image_surface_f);

#endif /* CAIRO_HAS_IMAGE_SURFACE */
    return 0;
}
