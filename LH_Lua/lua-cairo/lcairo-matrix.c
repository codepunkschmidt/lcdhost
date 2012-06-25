/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



/* Matrix functions */

// cairo_public void
// cairo_matrix_init (cairo_matrix_t *matrix,
//            double  xx, double  yx,
//            double  xy, double  yy,
//            double  x0, double  y0);
static int l_cairo_matrix_init(lua_State* L)
{
    cairo_matrix_t *matrix = get_cairo_matrix_t (L, 1);
    double  xx = luaL_checknumber(L, 2);
    double  yx = luaL_checknumber(L, 3);
    double  xy = luaL_checknumber(L, 4);
    double  yy = luaL_checknumber(L, 5);
    double  x0 = luaL_checknumber(L, 6);
    double  y0 = luaL_checknumber(L, 7);
    cairo_matrix_init (matrix, xx, yx, xy, yy, x0, y0);
    return 0;
}


// cairo_public void
// cairo_matrix_init_identity (cairo_matrix_t *matrix);
static int l_cairo_matrix_init_identity(lua_State* L)
{
    cairo_matrix_t *matrix = get_cairo_matrix_t (L, 1);
    cairo_matrix_init_identity (matrix);
    return 0;
}


// cairo_public void
// cairo_matrix_init_translate (cairo_matrix_t *matrix,
//                  double tx, double ty);
static int l_cairo_matrix_init_translate(lua_State* L)
{
    cairo_matrix_t *matrix = get_cairo_matrix_t (L, 1);
    double  tx = luaL_checknumber(L, 2);
    double  ty = luaL_checknumber(L, 3);
    cairo_matrix_init_translate (matrix, tx, ty);
    return 0;
}


// cairo_public void
// cairo_matrix_init_scale (cairo_matrix_t *matrix,
//              double sx, double sy);
static int l_cairo_matrix_init_scale(lua_State* L)
{
    cairo_matrix_t *matrix = get_cairo_matrix_t (L, 1);
    double  sx = luaL_checknumber(L, 2);
    double  sy = luaL_checknumber(L, 3);
    cairo_matrix_init_scale (matrix, sx, sy);
    return 0;
}


// cairo_public void
// cairo_matrix_init_rotate (cairo_matrix_t *matrix,
//               double radians);
static int l_cairo_matrix_init_rotate(lua_State* L)
{
    cairo_matrix_t *matrix = get_cairo_matrix_t (L, 1);
    double radians = luaL_checknumber(L, 2);
    cairo_matrix_init_rotate (matrix, radians);
    return 0;
}


// cairo_public void
// cairo_matrix_translate (cairo_matrix_t *matrix, double tx, double ty);
static int l_cairo_matrix_translate(lua_State* L)
{
    cairo_matrix_t *matrix = get_cairo_matrix_t (L, 1);
    double  tx = luaL_checknumber(L, 2);
    double  ty = luaL_checknumber(L, 3);
    cairo_matrix_translate (matrix, tx, ty);
    return 0;
}


// cairo_public void
// cairo_matrix_scale (cairo_matrix_t *matrix, double sx, double sy);
static int l_cairo_matrix_scale(lua_State* L)
{
    cairo_matrix_t *matrix = get_cairo_matrix_t (L, 1);
    double  sx = luaL_checknumber(L, 2);
    double  sy = luaL_checknumber(L, 3);
    cairo_matrix_scale (matrix, sx, sy);
    return 0;
}


// cairo_public void
// cairo_matrix_rotate (cairo_matrix_t *matrix, double radians);
static int l_cairo_matrix_rotate(lua_State* L)
{
    cairo_matrix_t *matrix = get_cairo_matrix_t (L, 1);
    double radians = luaL_checknumber(L, 2);
    cairo_matrix_rotate (matrix, radians);
    return 0;
}


// cairo_public cairo_status_t
// cairo_matrix_invert (cairo_matrix_t *matrix);
static int l_cairo_matrix_invert(lua_State* L)
{
    cairo_matrix_t *matrix = get_cairo_matrix_t (L, 1);
    cairo_status_t v = cairo_matrix_invert (matrix);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public void
// cairo_matrix_multiply (cairo_matrix_t *result,
//                const cairo_matrix_t *a,
//                const cairo_matrix_t *b);
static int l_cairo_matrix_multiply(lua_State* L)
{
    cairo_matrix_t *result = get_cairo_matrix_t (L, 1);
    const cairo_matrix_t *a = get_cairo_matrix_t (L, 2);
    const cairo_matrix_t *b = get_cairo_matrix_t (L, 3);
    cairo_matrix_multiply (result, a, b);
    return 0;
}


// cairo_public void
// cairo_matrix_transform_distance (const cairo_matrix_t *matrix,
//                  double *dx, double *dy);
static int l_cairo_matrix_transform_distance(lua_State* L)
{
    /// WARN:different usage
    const cairo_matrix_t *matrix = get_cairo_matrix_t (L, 1);
    double  dx = luaL_checknumber(L, 2);
    double  dy = luaL_checknumber(L, 3);
    cairo_matrix_transform_distance (matrix, &dx, &dy);
    lua_pushnumber(L, dx);
    lua_pushnumber(L, dy);
    return 2;
}


// cairo_public void
// cairo_matrix_transform_point (const cairo_matrix_t *matrix,
//                   double *x, double *y);
static int l_cairo_matrix_transform_point(lua_State* L)
{
    /// WARN:different usage
    const cairo_matrix_t *matrix = get_cairo_matrix_t (L, 1);
    double  x = luaL_checknumber(L, 2);
    double  y = luaL_checknumber(L, 3);
    cairo_matrix_transform_point (matrix, &x, &y);
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    return 2;
}



static const struct luaL_Reg matrix_f [] = {
    {"matrix_init",                              l_cairo_matrix_init},
    {"matrix_init_identity",                     l_cairo_matrix_init_identity},
    {"matrix_init_translate",                    l_cairo_matrix_init_translate},
    {"matrix_init_scale",                        l_cairo_matrix_init_scale},
    {"matrix_init_rotate",                       l_cairo_matrix_init_rotate},
    {"matrix_translate",                         l_cairo_matrix_translate},
    {"matrix_scale",                             l_cairo_matrix_scale},
    {"matrix_rotate",                            l_cairo_matrix_rotate},
    {"matrix_invert",                            l_cairo_matrix_invert},
    {"matrix_multiply",                          l_cairo_matrix_multiply},
    {"matrix_transform_distance",                l_cairo_matrix_transform_distance},
    {"matrix_transform_point",                   l_cairo_matrix_transform_point},
	{NULL, NULL} /* sentinel */
};


static int luaopen_lcairo_matrix(lua_State* L)
{
    // register functions
    luaL_register(L, LUACAIRO, matrix_f);
    return 0;
}
