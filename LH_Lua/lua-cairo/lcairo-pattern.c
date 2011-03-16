/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




/* Pattern creation functions */

// cairo_public cairo_pattern_t *
// cairo_pattern_create_rgb (double red, double green, double blue);
static int l_cairo_pattern_create_rgb(lua_State* L)
{
    double red = luaL_checknumber(L, 1);
    double green = luaL_checknumber(L, 2);
    double blue = luaL_checknumber(L, 3);
    cairo_pattern_t *v = cairo_pattern_create_rgb (red, green, blue);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public cairo_pattern_t *
// cairo_pattern_create_rgba (double red, double green, double blue,
//                double alpha);
static int l_cairo_pattern_create_rgba(lua_State* L)
{
    double red = luaL_checknumber(L, 1);
    double green = luaL_checknumber(L, 2);
    double blue = luaL_checknumber(L, 3);
    double alpha = luaL_checknumber(L, 4);
    cairo_pattern_t *v = cairo_pattern_create_rgba (red, green, blue, alpha);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public cairo_pattern_t *
// cairo_pattern_create_for_surface (cairo_surface_t *surface);
static int l_cairo_pattern_create_for_surface(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_pattern_t *v = cairo_pattern_create_for_surface (surface);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public cairo_pattern_t *
// cairo_pattern_create_linear (double x0, double y0,
//                  double x1, double y1);
static int l_cairo_pattern_create_linear(lua_State* L)
{
    double x0 = luaL_checknumber(L, 1);
    double y0 = luaL_checknumber(L, 2);
    double x1 = luaL_checknumber(L, 3);
    double y1 = luaL_checknumber(L, 4);
    cairo_pattern_t *v = cairo_pattern_create_linear(x0, y0, x1, y1);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public cairo_pattern_t *
// cairo_pattern_create_radial (double cx0, double cy0, double radius0,
//                  double cx1, double cy1, double radius1);
static int l_cairo_pattern_create_radial(lua_State* L)
{
    double cx0 = luaL_checknumber(L, 1);
    double cy0 = luaL_checknumber(L, 2);
    double radius0 = luaL_checknumber(L, 3);
    double cx1 = luaL_checknumber(L, 4);
    double cy1 = luaL_checknumber(L, 5);
    double radius1 = luaL_checknumber(L, 6);
    cairo_pattern_t *v = cairo_pattern_create_radial(cx0, cy0, radius0, cx1, cy1, radius1);
    lua_pushlightuserdata(L, v);
    return 1;
}


//FIXME should not be exposed to the user of the language binding,
//FIXME but rather used to implement memory management within the language binding
// cairo_public cairo_pattern_t *
// cairo_pattern_reference (cairo_pattern_t *pattern);
static int l_cairo_pattern_reference(lua_State* L)
{
    cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    cairo_pattern_t *v = cairo_pattern_reference(pattern);
    lua_pushlightuserdata(L, v);
    return 1;
}


//FIXME should not be exposed to the user of the language binding,
//FIXME but rather used to implement memory management within the language binding
// cairo_public void
// cairo_pattern_destroy (cairo_pattern_t *pattern);
static int l_cairo_pattern_destroy(lua_State* L)
{
    cairo_pattern_t *pattern = (cairo_pattern_t *) check_lightuserdata(L, 1);
    cairo_pattern_destroy(pattern);
    return 0;
}


//FIXME should not be exposed to the user of the language binding,
//FIXME but rather used to implement memory management within the language binding
// cairo_public unsigned int
// cairo_pattern_get_reference_count (cairo_pattern_t *pattern);
static int l_cairo_pattern_get_reference_count(lua_State* L)
{
    cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    unsigned int v = cairo_pattern_get_reference_count(pattern);
    lua_pushnumber(L, v);
    return 1;
}


// cairo_public cairo_status_t
// cairo_pattern_status (cairo_pattern_t *pattern);
static int l_cairo_pattern_status(lua_State* L)
{
    cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    cairo_status_t v = cairo_pattern_status(pattern);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public void *
// cairo_pattern_get_user_data (cairo_pattern_t		 *pattern,
//                  const cairo_user_data_key_t *key);
static int l_cairo_pattern_get_user_data(lua_State* L)
{
    //cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public cairo_status_t
// cairo_pattern_set_user_data (cairo_pattern_t		 *pattern,
//                  const cairo_user_data_key_t *key,
//                  void			 *user_data,
//                  cairo_destroy_func_t	  destroy);
static int l_cairo_pattern_set_user_data(lua_State* L)
{
    //cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public cairo_pattern_type_t
// cairo_pattern_get_type (cairo_pattern_t *pattern);
static int l_cairo_pattern_get_type(lua_State* L)
{
    cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    cairo_pattern_type_t v = cairo_pattern_get_type(pattern);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public void
// cairo_pattern_add_color_stop_rgb (cairo_pattern_t *pattern,
//                   double offset,
//                   double red, double green, double blue);
static int l_cairo_pattern_add_color_stop_rgb(lua_State* L)
{
    cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    double offset = luaL_checknumber(L, 2);
    double red = luaL_checknumber(L, 3);
    double green = luaL_checknumber(L, 4);
    double blue = luaL_checknumber(L, 5);
    cairo_pattern_add_color_stop_rgb(pattern, offset, red, green, blue);
    return 0;
}


// cairo_public void
// cairo_pattern_add_color_stop_rgba (cairo_pattern_t *pattern,
//                    double offset,
//                    double red, double green, double blue,
//                    double alpha);
static int l_cairo_pattern_add_color_stop_rgba(lua_State* L)
{
    cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    double offset = luaL_checknumber(L, 2);
    double red = luaL_checknumber(L, 3);
    double green = luaL_checknumber(L, 4);
    double blue = luaL_checknumber(L, 5);
    double alpha = luaL_checknumber(L, 6);
    cairo_pattern_add_color_stop_rgba(pattern, offset, red, green, blue, alpha);
    return 0;
}


// cairo_public void
// cairo_pattern_set_matrix (cairo_pattern_t      *pattern,
//               const cairo_matrix_t *matrix);
static int l_cairo_pattern_set_matrix(lua_State* L)
{
    cairo_pattern_t *pattern = get_cairo_pattern_t (L, 1);
    const cairo_matrix_t *matrix = get_cairo_matrix_t (L, 2);
    cairo_pattern_set_matrix (pattern, matrix);
    return 0;
}


// cairo_public void
// cairo_pattern_get_matrix (cairo_pattern_t *pattern,
//               cairo_matrix_t  *matrix);
static int l_cairo_pattern_get_matrix(lua_State* L)
{
    cairo_pattern_t *pattern = get_cairo_pattern_t (L, 1);
    cairo_matrix_t *matrix = get_cairo_matrix_t (L, 2);
    cairo_pattern_get_matrix (pattern, matrix);
    return 0;
}


// cairo_public void
// cairo_pattern_set_extend (cairo_pattern_t *pattern, cairo_extend_t extend);
static int l_cairo_pattern_set_extend(lua_State* L)
{
    cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    cairo_extend_t extend = (cairo_extend_t) luaL_checkinteger(L, 2);
    cairo_pattern_set_extend (pattern, extend);
    return 0;
}


// cairo_public cairo_extend_t
// cairo_pattern_get_extend (cairo_pattern_t *pattern);
static int l_cairo_pattern_get_extend(lua_State* L)
{
    cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    cairo_extend_t v = cairo_pattern_get_extend (pattern);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public void
// cairo_pattern_set_filter (cairo_pattern_t *pattern, cairo_filter_t filter);
static int l_cairo_pattern_set_filter(lua_State* L)
{
    cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    cairo_filter_t filter = (cairo_filter_t) luaL_checkinteger(L, 2);
    cairo_pattern_set_filter (pattern, filter);
    return 0;
}


// cairo_public cairo_filter_t
// cairo_pattern_get_filter (cairo_pattern_t *pattern);
static int l_cairo_pattern_get_filter(lua_State* L)
{
    cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    cairo_filter_t v = cairo_pattern_get_filter (pattern);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public cairo_status_t
// cairo_pattern_get_rgba (cairo_pattern_t *pattern,
//             double *red, double *green,
//             double *blue, double *alpha);
static int l_cairo_pattern_get_rgba(lua_State* L)
{
    /// WARN:different usage
    cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    double red   = luaL_optnumber(L, 2, 0.0);
    double green = luaL_optnumber(L, 3, 0.0);
    double blue  = luaL_optnumber(L, 4, 0.0);
    double alpha = luaL_optnumber(L, 5, 0.0);
    cairo_status_t v = cairo_pattern_get_rgba (pattern, &red, &green, &blue, &alpha);
    lua_pushinteger(L, v);
    lua_pushnumber(L, red);
    lua_pushnumber(L, green);
    lua_pushnumber(L, blue);
    lua_pushnumber(L, alpha);
    return 5;
}


// cairo_public cairo_status_t
// cairo_pattern_get_surface (cairo_pattern_t *pattern,
//                cairo_surface_t **surface);
static int l_cairo_pattern_get_surface(lua_State* L)
{
    /// WARN:different usage
    cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    cairo_surface_t *surface = (cairo_surface_t *) check_lightuserdata(L, 2);
    cairo_status_t v = cairo_pattern_get_surface (pattern, &surface);
    lua_pushinteger(L, v);
    lua_pushlightuserdata(L, surface);
    return 2;
}


// cairo_public cairo_status_t
// cairo_pattern_get_color_stop_rgba (cairo_pattern_t *pattern,
//                    int index, double *offset,
//                    double *red, double *green,
//                    double *blue, double *alpha);
static int l_cairo_pattern_get_color_stop_rgba(lua_State* L)
{
    /// WARN:different usage
    cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    int index     = luaL_checkinteger(L, 2);
    double offset = luaL_optnumber(L, 3, 0.0);
    double red    = luaL_optnumber(L, 4, 0.0);
    double green  = luaL_optnumber(L, 5, 0.0);
    double blue   = luaL_optnumber(L, 6, 0.0);
    double alpha  = luaL_optnumber(L, 7, 0.0);
    cairo_status_t v = cairo_pattern_get_color_stop_rgba (pattern, index, &offset, &red, &green, &blue, &alpha);
    lua_pushinteger(L, v);
    lua_pushnumber(L, offset);
    lua_pushnumber(L, red);
    lua_pushnumber(L, green);
    lua_pushnumber(L, blue);
    lua_pushnumber(L, alpha);
    return 6;
}


// cairo_public cairo_status_t
// cairo_pattern_get_color_stop_count (cairo_pattern_t *pattern,
//                     int *count);
static int l_cairo_pattern_get_color_stop_count(lua_State* L)
{
    /// WARN:different usage
    cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    int count = luaL_optinteger(L, 2, 0);
    cairo_status_t v = cairo_pattern_get_color_stop_count (pattern, &count);
    lua_pushinteger(L, v);
    lua_pushinteger(L, count);
    return 2;
}


// cairo_public cairo_status_t
// cairo_pattern_get_linear_points (cairo_pattern_t *pattern,
//                  double *x0, double *y0,
//                  double *x1, double *y1);
static int l_cairo_pattern_get_linear_points(lua_State* L)
{
    /// WARN:different usage
    cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    double x0 = luaL_optnumber(L, 2, 0.0);
    double y0 = luaL_optnumber(L, 3, 0.0);
    double x1 = luaL_optnumber(L, 4, 0.0);
    double y1 = luaL_optnumber(L, 5, 0.0);
    cairo_status_t v = cairo_pattern_get_linear_points (pattern, &x0, &y0, &x1, &y1);
    lua_pushinteger(L, v);
    lua_pushnumber(L, x0);
    lua_pushnumber(L, y0);
    lua_pushnumber(L, x1);
    lua_pushnumber(L, y1);
    return 5;
}


// cairo_public cairo_status_t
// cairo_pattern_get_radial_circles (cairo_pattern_t *pattern,
//                   double *x0, double *y0, double *r0,
//                   double *x1, double *y1, double *r1);
static int l_cairo_pattern_get_radial_circles(lua_State* L)
{
    /// WARN:different usage
    cairo_pattern_t *pattern = get_cairo_pattern_t(L, 1);
    double x0 = luaL_optnumber(L, 2, 0.0);
    double y0 = luaL_optnumber(L, 3, 0.0);
    double r0 = luaL_optnumber(L, 4, 0.0);
    double x1 = luaL_optnumber(L, 5, 0.0);
    double y1 = luaL_optnumber(L, 6, 0.0);
    double r1 = luaL_optnumber(L, 7, 0.0);
    cairo_status_t v = cairo_pattern_get_radial_circles (pattern, &x0, &y0, &r0, &x1, &y1, &r1);
    lua_pushinteger(L, v);
    lua_pushnumber(L, x0);
    lua_pushnumber(L, y0);
    lua_pushnumber(L, r0);
    lua_pushnumber(L, x1);
    lua_pushnumber(L, y1);
    lua_pushnumber(L, r1);
    return 7;
}



static const struct luaL_Reg pattern_f [] = {
    {"pattern_create_rgb",                       l_cairo_pattern_create_rgb},
    {"pattern_create_rgba",                      l_cairo_pattern_create_rgba},
    {"pattern_create_for_surface",               l_cairo_pattern_create_for_surface},
    {"pattern_create_linear",                    l_cairo_pattern_create_linear},
    {"pattern_create_radial",                    l_cairo_pattern_create_radial},
    {"pattern_reference",                        l_cairo_pattern_reference},
    {"pattern_destroy",                          l_cairo_pattern_destroy},
    {"pattern_get_reference_count",              l_cairo_pattern_get_reference_count},
    {"pattern_status",                           l_cairo_pattern_status},
    {"pattern_get_user_data",                    l_cairo_pattern_get_user_data},
    {"pattern_set_user_data",                    l_cairo_pattern_set_user_data},
    {"pattern_get_type",                         l_cairo_pattern_get_type},
    {"pattern_add_color_stop_rgb",               l_cairo_pattern_add_color_stop_rgb},
    {"pattern_add_color_stop_rgba",              l_cairo_pattern_add_color_stop_rgba},
    {"pattern_set_matrix",                       l_cairo_pattern_set_matrix},
    {"pattern_get_matrix",                       l_cairo_pattern_get_matrix},
    {"pattern_set_extend",                       l_cairo_pattern_set_extend},
    {"pattern_get_extend",                       l_cairo_pattern_get_extend},
    {"pattern_set_filter",                       l_cairo_pattern_set_filter},
    {"pattern_get_filter",                       l_cairo_pattern_get_filter},
    {"pattern_get_rgba",                         l_cairo_pattern_get_rgba},
    {"pattern_get_surface",                      l_cairo_pattern_get_surface},
    {"pattern_get_color_stop_rgba",              l_cairo_pattern_get_color_stop_rgba},
    {"pattern_get_color_stop_count",             l_cairo_pattern_get_color_stop_count},
    {"pattern_get_linear_points",                l_cairo_pattern_get_linear_points},
    {"pattern_get_radial_circles",               l_cairo_pattern_get_radial_circles},
	{NULL, NULL} /* sentinel */
};


static int luaopen_lcairo_pattern(lua_State* L)
{
    // register functions
    luaL_register(L, LUACAIRO, pattern_f);
    return 0;
}
