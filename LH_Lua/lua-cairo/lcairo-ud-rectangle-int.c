/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



//---------------------------------------------------------------------------
// RectangleInt userdata for cairo_rectangle_int_t
//---------------------------------------------------------------------------


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)


#define check_RectangleInt_ud(L, idx) \
    (cairo_rectangle_int_t *) luaL_checkudata(L, idx, LUACAIRO ".RectangleInt.mt")

static cairo_rectangle_int_t check_RectangleInt(lua_State* L, int idx)
{
    luaL_checktype(L, idx, LUA_TTABLE);
    lua_pushvalue(L, idx); // copy table to top

    cairo_rectangle_int_t rect;
    rect.x      = get_numfield(L, "x");
    rect.y      = get_numfield(L, "y");
    rect.width  = get_numfield(L, "width");
    rect.height = get_numfield(L, "height");

    lua_pop(L, 1); // remove copied table
    return rect;
}

static int new_RectangleInt (lua_State *L)
{
    cairo_rectangle_int_t rectin = {0, 0, 0, 0};
    int top = lua_gettop(L);
    if (top > 1) rectin = check_RectangleInt(L, 2);

    cairo_rectangle_int_t *rect = (cairo_rectangle_int_t *) lua_newuserdata(L, sizeof(cairo_rectangle_int_t));
    *rect = rectin;

    luaL_getmetatable(L, LUACAIRO ".RectangleInt.mt");
    lua_setmetatable(L, -2);

    return 1;
}

static int tostring_RectangleInt (lua_State *L)
{
    cairo_rectangle_int_t *rect = check_RectangleInt_ud(L, 1);
    lua_pushfstring(L, "x:%d, y:%d, width:%d, height:%d",
        rect->x, rect->y, rect->width, rect->height);
    return 1;
}

static const struct luaL_Reg RectangleInt_meta_methods[] = {
    {"__tostring",                               tostring_RectangleInt},
    {NULL, NULL}
};

static const struct luaL_Reg RectangleInt_methods[] = {
    {"new",                                      new_RectangleInt},
    {NULL, NULL}
};

static const Xet_reg_pre RectangleInt_getters[] = {
    {"x",      Xet_get_number, offsetof(cairo_rectangle_int_t, x) },
    {"y",      Xet_get_number, offsetof(cairo_rectangle_int_t, y) },
    {"width",  Xet_get_number, offsetof(cairo_rectangle_int_t, width) },
    {"heignt", Xet_get_number, offsetof(cairo_rectangle_int_t, height) },
    {0, 0, 0}
};

static const Xet_reg_pre RectangleInt_setters[] = {
    {"x",      Xet_set_number, offsetof(cairo_rectangle_int_t, x) },
    {"y",      Xet_set_number, offsetof(cairo_rectangle_int_t, y) },
    {"width",  Xet_set_number, offsetof(cairo_rectangle_int_t, width) },
    {"height", Xet_set_number, offsetof(cairo_rectangle_int_t, height) },
    {0, 0, 0}
};


static int luaopen_lrectangleint (lua_State *L)
{
    int metatable, methods;

    /* create methods table, & add it to the table of globals */
    luaL_register(L, LUACAIRO ".RectangleInt", RectangleInt_methods);
    methods = lua_gettop(L);

    /* create metatable for your_t, & add it to the registry */
    luaL_newmetatable(L, LUACAIRO ".RectangleInt.mt");
    luaL_register(L, NULL, RectangleInt_meta_methods);
    metatable = lua_gettop(L);

    lua_pushliteral(L, "__metatable");
    lua_pushvalue(L, methods);      /* dup methods table*/
    lua_rawset(L, metatable);       /* hide metatable:
                                       metatable.__metatable = methods */
    lua_pushliteral(L, "__index");
    lua_pushvalue(L, metatable);    /* upvalue index 1 */
    Xet_add(L, RectangleInt_getters); /* fill metatable with getters */
    lua_pushvalue(L, methods);      /* upvalue index 2 */
    lua_pushcclosure(L, Xet_index_handler, 2);
    lua_rawset(L, metatable);       /* metatable.__index = index_handler */

    lua_pushliteral(L, "__newindex");
    lua_newtable(L);                /* table for members you can set */
    Xet_add(L, RectangleInt_setters);     /* fill with setters */
    lua_pushcclosure(L, Xet_newindex_handler, 1);
    lua_rawset(L, metatable);       /* metatable.__newindex = newindex_handler */

    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_RectangleInt);
    lua_settable(L, -3);
    lua_setmetatable(L, methods);

    lua_pop(L, 1);                  /* drop metatable */
    lua_pop(L, 1);                  //hd.. drop methods
    return 0;
}

#endif /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0) */


static int luaopen_lcairo_ud_rectangle_int (lua_State* L)
{
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)

    luaopen_lrectangleint (L);

#endif /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0) */
    return 0;
}
