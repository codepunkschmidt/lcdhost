/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




// --------------------------------------------------------------------------
// Region userdata for cairo_region_t
// --------------------------------------------------------------------------



#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
#if LUACAIRO_HAS_OO_INTERFACE

typedef struct _Region
{
    cairo_region_t* reg_;
    int             havereg_;
} Region;

#define check_Region_ud(L, idx) \
    (Region *) luaL_checkudata(L, idx, LUACAIRO ".Region.mt")

static int new_Region (lua_State *L)
{
    lua_remove(L, 1); // remove cairo.Region

// cairo_public cairo_region_t *
// cairo_region_create (void);

// cairo_public cairo_region_t *
// cairo_region_create_rectangle (const cairo_rectangle_int_t *rectangle);

    cairo_rectangle_int_t *rectangle = NULL;
    int top = lua_gettop(L);
    if (top > 0) rectangle = get_userdata (L, 1, LUACAIRO ".RectangleInt.mt");

    cairo_region_t *reg = NULL;
    if (rectangle)
        reg = cairo_region_create_rectangle (rectangle);
    else
        reg = cairo_region_create ();

    Region *o = (Region *) lua_newuserdata(L, sizeof(Region));
    o->reg_     = reg;
    o->havereg_ = 1;

    luaL_getmetatable(L, LUACAIRO ".Region.mt");
    lua_setmetatable(L, -2);

    return 1;
}

static int new_RegionFromLUD (lua_State *L)
{
    cairo_region_t *reg = (cairo_region_t *) check_lightuserdata(L, 1);
    int havereg  = lua_toboolean(L, 2);

    Region *o = (Region *) lua_newuserdata(L, sizeof(Region));
    o->reg_     = reg;
    o->havereg_ = havereg;

    luaL_getmetatable(L, LUACAIRO ".Region.mt");
    lua_setmetatable(L, -2);

    return 1;
}

static int gc_Region (lua_State *L)
{
    Region *o = check_Region_ud(L, 1);

    if (o->reg_ && o->havereg_)
    {
        cairo_region_destroy (o->reg_);
        o->reg_     = NULL;
        o->havereg_ = 0;
    }

    return 0;
}

static int tostring_Region (lua_State *L)
{
    Region *o = check_Region_ud(L, 1);
    lua_pushfstring(L, "Region (%p), cairo_region_t (%p)", (void*)o, (void*)o->reg_);
    return 1;
}

static const struct luaL_Reg Region_lib_f [] = {
    {"new", new_Region},
    {NULL, NULL}
};

static const struct luaL_Reg Region_lib_m [] = {
    {"__gc",                                     gc_Region},
    {"__tostring",                               tostring_Region},

  //{"create",                                   l_cairo_region_create},
  //{"create_rectangle",                         l_cairo_region_create_rectangle},
  //{"create_rectangles",                        l_cairo_region_create_rectangles},
    {"copy",                                     l_cairo_region_copy},
    {"reference",                                l_cairo_region_reference},
    {"destroy",                                  gc_Region},
    {"equal",                                    l_cairo_region_equal},
    {"status",                                   l_cairo_region_status},
    {"get_extents",                              l_cairo_region_get_extents},
    {"num_rectangles",                           l_cairo_region_num_rectangles},
    {"get_rectangle",                            l_cairo_region_get_rectangle},
    {"is_empty",                                 l_cairo_region_is_empty},
    {"contains_rectangle",                       l_cairo_region_contains_rectangle},
    {"contains_point",                           l_cairo_region_contains_point},
    {"translate",                                l_cairo_region_translate},
    {"subtract",                                 l_cairo_region_subtract},
    {"subtract_rectangle",                       l_cairo_region_subtract_rectangle},
    {"intersect",                                l_cairo_region_intersect},
    {"intersect_rectangle",                      l_cairo_region_intersect_rectangle},
    {"union",                                    l_cairo_region_union},
    {"union_rectangle",                          l_cairo_region_union_rectangle},
    {"xor",                                      l_cairo_region_xor},
    {"xor_rectangle",                            l_cairo_region_xor_rectangle},

    {NULL, NULL}
};


static int luaopen_lregion (lua_State *L)
{
    luaL_newmetatable(L, LUACAIRO ".Region.mt");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, Region_lib_m);
    luaL_register(L, LUACAIRO ".Region", Region_lib_f);
    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_Region);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    lua_pop(L, 1);

    lua_pop(L, 1);
    return 0;
}

#endif  /* LUACAIRO_HAS_OO_INTERFACE */
#endif  /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0) */



static int luaopen_lcairo_ud_region (lua_State* L)
{
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lregion (L);

#endif  /* LUACAIRO_HAS_OO_INTERFACE */
#endif  /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0) */
    return 0;
}
