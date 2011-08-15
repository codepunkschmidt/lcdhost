/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)


/* Region functions */


// cairo_public cairo_region_t *
// cairo_region_create (void);
static int l_cairo_region_create (lua_State* L)
{
    cairo_region_t *v = cairo_region_create();
    lua_pushlightuserdata(L, v);
    return 1;
}

// cairo_public cairo_region_t *
// cairo_region_create_rectangle (const cairo_rectangle_int_t *rectangle);
static int l_cairo_region_create_rectangle (lua_State* L)
{
    cairo_rectangle_int_t *rectangle = get_cairo_rectangle_int_t (L, 1);
    const cairo_region_t *v = cairo_region_create_rectangle (rectangle);
    lua_pushlightuserdata(L, (void*)v);
    return 1;
}

// cairo_public cairo_region_t *
// cairo_region_create_rectangles (const cairo_rectangle_int_t *rects,
// 				int count);
static int l_cairo_region_create_rectangles (lua_State* L)
{
    // TODO use table for rects?
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_region_t *
// cairo_region_copy (const cairo_region_t *original);
static int l_cairo_region_copy (lua_State* L)
{
    const cairo_region_t *original = get_cairo_region_t (L, 1);
    cairo_region_t *v = cairo_region_copy (original);
    lua_pushlightuserdata(L, v);
    return 1;
}

// cairo_public cairo_region_t *
// cairo_region_reference (cairo_region_t *region);
static int l_cairo_region_reference (lua_State* L)
{
    cairo_region_t *region = get_cairo_region_t (L, 1);
    cairo_region_t *v = cairo_region_reference (region);
    lua_pushlightuserdata(L, v);
    return 1;
}

// cairo_public void
// cairo_region_destroy (cairo_region_t *region);
static int l_cairo_region_destroy (lua_State* L)
{
    cairo_region_t *region = get_cairo_region_t (L, 1);
    cairo_region_destroy (region);
    return 0;
}

// cairo_public cairo_bool_t
// cairo_region_equal (const cairo_region_t *a, const cairo_region_t *b);
static int l_cairo_region_equal (lua_State* L)
{
    const cairo_region_t *a = get_cairo_region_t (L, 1);
    const cairo_region_t *b = get_cairo_region_t (L, 2);
    cairo_bool_t v = cairo_region_equal (a, b);
    lua_pushboolean(L, v);
    return 1;
}

// cairo_public cairo_status_t
// cairo_region_status (const cairo_region_t *region);
static int l_cairo_region_status (lua_State* L)
{
    const cairo_region_t *region = get_cairo_region_t (L, 1);
    cairo_status_t v = cairo_region_status (region);
    lua_pushinteger(L, v);
    return 1;
}

// cairo_public void
// cairo_region_get_extents (const cairo_region_t        *region,
// 			  cairo_rectangle_int_t *extents);
static int l_cairo_region_get_extents (lua_State* L)
{
    const cairo_region_t *region = get_cairo_region_t (L, 1);
    cairo_rectangle_int_t *extents = get_cairo_rectangle_int_t (L, 2);
    cairo_region_get_extents (region, extents);
    return 0;
}

// cairo_public int
// cairo_region_num_rectangles (const cairo_region_t *region);
static int l_cairo_region_num_rectangles (lua_State* L)
{
    const cairo_region_t *region = get_cairo_region_t (L, 1);
    int v = cairo_region_num_rectangles (region);
    lua_pushinteger(L, v);
    return 1;
}

// cairo_public void
// cairo_region_get_rectangle (const cairo_region_t  *region,
// 			    int                    nth,
// 			    cairo_rectangle_int_t *rectangle);
static int l_cairo_region_get_rectangle (lua_State* L)
{
    const cairo_region_t *region = get_cairo_region_t (L, 1);
    int nth = luaL_checkinteger(L, 2);
    cairo_rectangle_int_t *rectangle = get_cairo_rectangle_int_t (L, 3);
    cairo_region_get_rectangle (region, nth, rectangle);
    return 0;
}

// cairo_public cairo_bool_t
// cairo_region_is_empty (const cairo_region_t *region);
static int l_cairo_region_is_empty (lua_State* L)
{
    const cairo_region_t *region = get_cairo_region_t (L, 1);
    cairo_bool_t v = cairo_region_is_empty (region);
    lua_pushboolean(L, v);
    return 1;
}

// cairo_public cairo_region_overlap_t
// cairo_region_contains_rectangle (const cairo_region_t *region,
// 				 const cairo_rectangle_int_t *rectangle);
static int l_cairo_region_contains_rectangle (lua_State* L)
{
    const cairo_region_t *region = get_cairo_region_t (L, 1);
    const cairo_rectangle_int_t *rectangle = get_cairo_rectangle_int_t (L, 2);
    cairo_region_overlap_t v = cairo_region_contains_rectangle (region, rectangle);
    lua_pushinteger(L, v);
    return 1;
}

// cairo_public cairo_bool_t
// cairo_region_contains_point (const cairo_region_t *region, int x, int y);
static int l_cairo_region_contains_point (lua_State* L)
{
    const cairo_region_t *region = get_cairo_region_t (L, 1);
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 2);
    cairo_bool_t v = cairo_region_contains_point (region, x, y);
    lua_pushboolean(L, v);
    return 1;
}

// cairo_public void
// cairo_region_translate (cairo_region_t *region, int dx, int dy);
static int l_cairo_region_translate (lua_State* L)
{
    cairo_region_t *region = get_cairo_region_t (L, 1);
    int dx = luaL_checkinteger(L, 2);
    int dy = luaL_checkinteger(L, 2);
    cairo_region_translate (region, dx, dy);
    return 0;
}

// cairo_public cairo_status_t
// cairo_region_subtract (cairo_region_t *dst, const cairo_region_t *other);
static int l_cairo_region_subtract (lua_State* L)
{
    cairo_region_t *dst = get_cairo_region_t (L, 1);
    const cairo_region_t *other = get_cairo_region_t (L, 2);
    cairo_status_t v = cairo_region_subtract (dst, other);
    lua_pushinteger(L, v);
    return 1;
}

// cairo_public cairo_status_t
// cairo_region_subtract_rectangle (cairo_region_t *dst,
// 				 const cairo_rectangle_int_t *rectangle);
static int l_cairo_region_subtract_rectangle (lua_State* L)
{
    cairo_region_t *dst = get_cairo_region_t (L, 1);
    const cairo_rectangle_int_t *rectangle = get_cairo_rectangle_int_t (L, 2);
    cairo_status_t v = cairo_region_subtract_rectangle (dst, rectangle);
    lua_pushinteger(L, v);
    return 1;
}

// cairo_public cairo_status_t
// cairo_region_intersect (cairo_region_t *dst, const cairo_region_t *other);
static int l_cairo_region_intersect (lua_State* L)
{
    cairo_region_t *dst = get_cairo_region_t (L, 1);
    const cairo_region_t *other = get_cairo_region_t (L, 2);
    cairo_status_t v = cairo_region_intersect (dst, other);
    lua_pushinteger(L, v);
    return 1;
}

// cairo_public cairo_status_t
// cairo_region_intersect_rectangle (cairo_region_t *dst,
// 				  const cairo_rectangle_int_t *rectangle);
static int l_cairo_region_intersect_rectangle (lua_State* L)
{
    cairo_region_t *dst = get_cairo_region_t (L, 1);
    const cairo_rectangle_int_t *rectangle = get_cairo_rectangle_int_t (L, 2);
    cairo_status_t v = cairo_region_intersect_rectangle (dst, rectangle);
    lua_pushinteger(L, v);
    return 1;
}

// cairo_public cairo_status_t
// cairo_region_union (cairo_region_t *dst, const cairo_region_t *other);
static int l_cairo_region_union (lua_State* L)
{
    cairo_region_t *dst = get_cairo_region_t (L, 1);
    const cairo_region_t *other = get_cairo_region_t (L, 2);
    cairo_status_t v = cairo_region_union (dst, other);
    lua_pushinteger(L, v);
    return 1;
}

// cairo_public cairo_status_t
// cairo_region_union_rectangle (cairo_region_t *dst,
// 			      const cairo_rectangle_int_t *rectangle);
static int l_cairo_region_union_rectangle (lua_State* L)
{
    cairo_region_t *dst = get_cairo_region_t (L, 1);
    const cairo_rectangle_int_t *rectangle = get_cairo_rectangle_int_t (L, 2);
    cairo_status_t v = cairo_region_union_rectangle (dst, rectangle);
    lua_pushinteger(L, v);
    return 1;
}

// cairo_public cairo_status_t
// cairo_region_xor (cairo_region_t *dst, const cairo_region_t *other);
static int l_cairo_region_xor (lua_State* L)
{
    cairo_region_t *dst = get_cairo_region_t (L, 1);
    const cairo_region_t *other = get_cairo_region_t (L, 2);
    cairo_status_t v = cairo_region_xor (dst, other);
    lua_pushinteger(L, v);
    return 1;
}

// cairo_public cairo_status_t
// cairo_region_xor_rectangle (cairo_region_t *dst,
// 			    const cairo_rectangle_int_t *rectangle);
static int l_cairo_region_xor_rectangle (lua_State* L)
{
    cairo_region_t *dst = get_cairo_region_t (L, 1);
    const cairo_rectangle_int_t *rectangle = get_cairo_rectangle_int_t (L, 2);
    cairo_status_t v = cairo_region_xor_rectangle (dst, rectangle);
    lua_pushinteger(L, v);
    return 1;
}




static const struct luaL_Reg region_f [] = {
    {"region_create",                              l_cairo_region_create},
    {"region_create_rectangle",                    l_cairo_region_create_rectangle},
    {"region_create_rectangles",                   l_cairo_region_create_rectangles},
    {"region_copy",                                l_cairo_region_copy},
    {"region_reference",                           l_cairo_region_reference},
    {"region_destroy",                             l_cairo_region_destroy},
    {"region_equal",                               l_cairo_region_equal},
    {"region_status",                              l_cairo_region_status},
    {"region_get_extents",                         l_cairo_region_get_extents},
    {"region_num_rectangles",                      l_cairo_region_num_rectangles},
    {"region_get_rectangle",                       l_cairo_region_get_rectangle},
    {"region_is_empty",                            l_cairo_region_is_empty},
    {"region_contains_rectangle",                  l_cairo_region_contains_rectangle},
    {"region_contains_point",                      l_cairo_region_contains_point},
    {"region_translate",                           l_cairo_region_translate},
    {"region_subtract",                            l_cairo_region_subtract},
    {"region_subtract_rectangle",                  l_cairo_region_subtract_rectangle},
    {"region_intersect",                           l_cairo_region_intersect},
    {"region_intersect_rectangle",                 l_cairo_region_intersect_rectangle},
    {"region_union",                               l_cairo_region_union},
    {"region_union_rectangle",                     l_cairo_region_union_rectangle},
    {"region_xor",                                 l_cairo_region_xor},
    {"region_xor_rectangle",                       l_cairo_region_xor_rectangle},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0) */


static int luaopen_lcairo_region (lua_State* L)
{
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)

    // register functions
    luaL_register(L, LUACAIRO, region_f);

#endif /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0) */
    return 0;
}
