/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



// cairo_public cairo_font_options_t *
// cairo_font_options_create (void);
static int l_cairo_font_options_create(lua_State* L)
{
    cairo_font_options_t *v = cairo_font_options_create();
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public cairo_font_options_t *
// cairo_font_options_copy (const cairo_font_options_t *original);
static int l_cairo_font_options_copy(lua_State* L)
{
    const cairo_font_options_t *original = get_cairo_font_options_t (L, 1);
    cairo_font_options_t *v = cairo_font_options_copy (original);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public void
// cairo_font_options_destroy (cairo_font_options_t *options);
static int l_cairo_font_options_destroy(lua_State* L)
{
    cairo_font_options_t *options = (cairo_font_options_t *) check_lightuserdata(L, 1);
    cairo_font_options_destroy(options);
    return 0;
}


// cairo_public cairo_status_t
// cairo_font_options_status (cairo_font_options_t *options);
static int l_cairo_font_options_status(lua_State* L)
{
    cairo_font_options_t *options = get_cairo_font_options_t (L, 1);
    cairo_status_t v = cairo_font_options_status(options);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public void
// cairo_font_options_merge (cairo_font_options_t *options,
//           const cairo_font_options_t *other);
static int l_cairo_font_options_merge(lua_State* L)
{
    cairo_font_options_t *options = get_cairo_font_options_t (L, 1);
    const cairo_font_options_t *other = get_cairo_font_options_t (L, 2);
    cairo_font_options_merge(options, other);
    return 0;
}


// cairo_public cairo_bool_t
// cairo_font_options_equal (const cairo_font_options_t *options,
//           const cairo_font_options_t *other);
static int l_cairo_font_options_equal(lua_State* L)
{
    const cairo_font_options_t *options = get_cairo_font_options_t (L, 1);
    const cairo_font_options_t *other = get_cairo_font_options_t (L, 2);
    cairo_bool_t v = cairo_font_options_equal(options, other);
    lua_pushboolean(L, v);
    return 1;
}


// cairo_public unsigned long
// cairo_font_options_hash (const cairo_font_options_t *options);
static int l_cairo_font_options_hash(lua_State* L)
{
    const cairo_font_options_t *options = get_cairo_font_options_t (L, 1);
    unsigned long v = cairo_font_options_hash(options);
    lua_pushnumber(L, v);
    return 1;
}


// cairo_public void
// cairo_font_options_set_antialias (cairo_font_options_t *options,
//                   cairo_antialias_t     antialias);
static int l_cairo_font_options_set_antialias(lua_State* L)
{
    cairo_font_options_t *options = get_cairo_font_options_t (L, 1);
    cairo_antialias_t antialias = (cairo_antialias_t) luaL_checkinteger(L, 2);
    cairo_font_options_set_antialias(options, antialias);
    return 0;
}


// cairo_public cairo_antialias_t
// cairo_font_options_get_antialias (const cairo_font_options_t *options);
static int l_cairo_font_options_get_antialias(lua_State* L)
{
    const cairo_font_options_t *options = get_cairo_font_options_t (L, 1);
    cairo_antialias_t v = cairo_font_options_get_antialias(options);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public void
// cairo_font_options_set_subpixel_order (cairo_font_options_t *options,
//                        cairo_subpixel_order_t  subpixel_order);
static int l_cairo_font_options_set_subpixel_order(lua_State* L)
{
    cairo_font_options_t *options = get_cairo_font_options_t (L, 1);
    cairo_subpixel_order_t subpixel_order = (cairo_subpixel_order_t) luaL_checkinteger(L, 2);
    cairo_font_options_set_subpixel_order(options, subpixel_order);
    return 0;
}


// cairo_public cairo_subpixel_order_t
// cairo_font_options_get_subpixel_order (const cairo_font_options_t *options);
static int l_cairo_font_options_get_subpixel_order(lua_State* L)
{
    const cairo_font_options_t *options = get_cairo_font_options_t (L, 1);
    cairo_subpixel_order_t v = cairo_font_options_get_subpixel_order(options);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public void
// cairo_font_options_set_hint_style (cairo_font_options_t *options,
//                    cairo_hint_style_t     hint_style);
static int l_cairo_font_options_set_hint_style(lua_State* L)
{
    cairo_font_options_t *options = get_cairo_font_options_t (L, 1);
    cairo_hint_style_t hint_style = (cairo_hint_style_t) luaL_checkinteger(L, 2);
    cairo_font_options_set_hint_style(options, hint_style);
    return 0;
}


// cairo_public cairo_hint_style_t
// cairo_font_options_get_hint_style (const cairo_font_options_t *options);
static int l_cairo_font_options_get_hint_style(lua_State* L)
{
    const cairo_font_options_t *options = get_cairo_font_options_t (L, 1);
    cairo_hint_style_t v = cairo_font_options_get_hint_style(options);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public void
// cairo_font_options_set_hint_metrics (cairo_font_options_t *options,
//                      cairo_hint_metrics_t  hint_metrics);
static int l_cairo_font_options_set_hint_metrics(lua_State* L)
{
    cairo_font_options_t *options = get_cairo_font_options_t (L, 1);
    cairo_hint_metrics_t hint_metrics = (cairo_hint_metrics_t) luaL_checkinteger(L, 2);
    cairo_font_options_set_hint_metrics(options, hint_metrics);
    return 0;
}


// cairo_public cairo_hint_metrics_t
// cairo_font_options_get_hint_metrics (const cairo_font_options_t *options);
static int l_cairo_font_options_get_hint_metrics(lua_State* L)
{
    const cairo_font_options_t *options = get_cairo_font_options_t (L, 1);
    cairo_hint_metrics_t v = cairo_font_options_get_hint_metrics(options);
    lua_pushinteger(L, v);
    return 1;
}



static const struct luaL_Reg font_options_f [] = {
    {"font_options_create",                      l_cairo_font_options_create},
    {"font_options_copy",                        l_cairo_font_options_copy},
    {"font_options_destroy",                     l_cairo_font_options_destroy},
    {"font_options_status",                      l_cairo_font_options_status},
    {"font_options_merge",                       l_cairo_font_options_merge},
    {"font_options_equal",                       l_cairo_font_options_equal},
    {"font_options_hash",                        l_cairo_font_options_hash},
    {"font_options_set_antialias",               l_cairo_font_options_set_antialias},
    {"font_options_get_antialias",               l_cairo_font_options_get_antialias},
    {"font_options_set_subpixel_order",          l_cairo_font_options_set_subpixel_order},
    {"font_options_get_subpixel_order",          l_cairo_font_options_get_subpixel_order},
    {"font_options_set_hint_style",              l_cairo_font_options_set_hint_style},
    {"font_options_get_hint_style",              l_cairo_font_options_get_hint_style},
    {"font_options_set_hint_metrics",            l_cairo_font_options_set_hint_metrics},
    {"font_options_get_hint_metrics",            l_cairo_font_options_get_hint_metrics},
	{NULL, NULL} /* sentinel */
};


static int luaopen_lcairo_font_options(lua_State* L)
{
    // register functions
    luaL_register(L, LUACAIRO, font_options_f);
    return 0;
}
