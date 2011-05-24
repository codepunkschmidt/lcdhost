/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




///--------------------------------------------------------------------------
/// TextExtents userdata for cairo_text_extents_t
///--------------------------------------------------------------------------


#define check_TextExtents_ud(L, idx) \
    (cairo_text_extents_t *) luaL_checkudata(L, idx, LUACAIRO ".TextExtents.mt")

static cairo_text_extents_t check_TextExtents(lua_State* L, int idx)
{
    luaL_checktype(L, idx, LUA_TTABLE);
    lua_pushvalue(L, idx); // copy table to top

    cairo_text_extents_t extents;
    extents.x_bearing = get_numfield(L, "x_bearing");
    extents.y_bearing = get_numfield(L, "y_bearing");
    extents.width     = get_numfield(L, "width");
    extents.height    = get_numfield(L, "height");
    extents.x_advance = get_numfield(L, "x_advance");
    extents.y_advance = get_numfield(L, "y_advance");

    lua_pop(L, 1); // remove copied table
    return extents;
}

static int new_TextExtents (lua_State *L)
{
    cairo_text_extents_t tein = {0, 0, 0, 0, 0, 0};
    int top = lua_gettop(L);
    if (top > 1) tein = check_TextExtents(L, 2);

    cairo_text_extents_t *te = (cairo_text_extents_t *) lua_newuserdata(L, sizeof(cairo_text_extents_t));
    *te = tein;

    luaL_getmetatable(L, LUACAIRO ".TextExtents.mt");
    lua_setmetatable(L, -2);

    return 1;
}

static int tostring_TextExtents (lua_State *L)
{
    cairo_text_extents_t *te = check_TextExtents_ud(L, 1);
    lua_pushfstring(L, "x_bearing:%f, y_bearing:%f, width:%f, height:%f, x_advance:%f, y_advance:%f",
        te->x_bearing, te->y_bearing, te->width, te->height, te->x_advance, te->y_advance);
    return 1;
}

static const struct luaL_Reg TextExtents_meta_methods[] = {
    {"__tostring",                               tostring_TextExtents},
    {NULL, NULL}
};

static const struct luaL_Reg TextExtents_methods[] = {
    {"new",                                      new_TextExtents},
    {NULL, NULL}
};

static const Xet_reg_pre TextExtents_getters[] = {
    {"x_bearing",    Xet_get_number, offsetof(cairo_text_extents_t, x_bearing) },
    {"y_bearing",    Xet_get_number, offsetof(cairo_text_extents_t, y_bearing) },
    {"width",        Xet_get_number, offsetof(cairo_text_extents_t, width)     },
    {"height",       Xet_get_number, offsetof(cairo_text_extents_t, height)    },
    {"x_advance",    Xet_get_number, offsetof(cairo_text_extents_t, x_advance) },
    {"y_advance",    Xet_get_number, offsetof(cairo_text_extents_t, y_advance) },
    {0, 0, 0}
};

static const Xet_reg_pre TextExtents_setters[] = {
    {"x_bearing",    Xet_set_number, offsetof(cairo_text_extents_t, x_bearing) },
    {"y_bearing",    Xet_set_number, offsetof(cairo_text_extents_t, y_bearing) },
    {"width",        Xet_set_number, offsetof(cairo_text_extents_t, width)     },
    {"height",       Xet_set_number, offsetof(cairo_text_extents_t, height)    },
    {"x_advance",    Xet_set_number, offsetof(cairo_text_extents_t, x_advance) },
    {"y_advance",    Xet_set_number, offsetof(cairo_text_extents_t, y_advance) },
    {0, 0, 0}
};


static int luaopen_ltextextents (lua_State *L)
{
    int metatable, methods;

    /* create methods table, & add it to the table of globals */
    //hd.. luaL_openlib(L, YOUR_T, your_methods, 0);
    luaL_register(L, LUACAIRO ".TextExtents", TextExtents_methods);
    methods = lua_gettop(L);

    /* create metatable for your_t, & add it to the registry */
    luaL_newmetatable(L, LUACAIRO ".TextExtents.mt");
    //hd.. luaL_openlib(L, 0, your_meta_methods, 0);  /* fill metatable */
    luaL_register(L, NULL, TextExtents_meta_methods);
    metatable = lua_gettop(L);

    lua_pushliteral(L, "__metatable");
    lua_pushvalue(L, methods);           /* dup methods table*/
    lua_rawset(L, metatable);            /* hide metatable:
                                            metatable.__metatable = methods */
    lua_pushliteral(L, "__index");
    lua_pushvalue(L, metatable);         /* upvalue index 1 */
    Xet_add(L, TextExtents_getters);     /* fill metatable with getters */
    lua_pushvalue(L, methods);           /* upvalue index 2 */
    lua_pushcclosure(L, Xet_index_handler, 2);
    lua_rawset(L, metatable);            /* metatable.__index = index_handler */

    lua_pushliteral(L, "__newindex");
    lua_newtable(L);                     /* table for members you can set */
    Xet_add(L, TextExtents_setters);     /* fill with setters */
    lua_pushcclosure(L, Xet_newindex_handler, 1);
    lua_rawset(L, metatable);            /* metatable.__newindex = newindex_handler */

    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_TextExtents);
    lua_settable(L, -3);
    lua_setmetatable(L, methods);

    lua_pop(L, 1);                       /* drop metatable */
    //hd.. return 1;                     /* return methods on the stack */
    lua_pop(L, 1);                       //hd.. drop methods
    return 0;
}

//unused, obsolete
//static int set_TextExtents (lua_State *L)
//{
//    cairo_text_extents_t *te = check_TextExtents_ud(L, 1);
//    const char* field = luaL_checkstring(L, 2);
//    double val = luaL_checknumber(L, 3);
//    if      (0 == strcmp("x_bearing", field)) te->x_bearing = val;
//    else if (0 == strcmp("y_bearing", field)) te->y_bearing = val;
//    else if (0 == strcmp("width",     field)) te->width = val;
//    else if (0 == strcmp("height",    field)) te->height = val;
//    else if (0 == strcmp("x_advance", field)) te->x_advance = val;
//    else if (0 == strcmp("y_advance", field)) te->y_advance = val;
//    else luaL_error(L, "unknown field");
//    return 0;
//}
//
//static int get_TextExtents (lua_State *L)
//{
//    cairo_text_extents_t *te = check_TextExtents_ud(L, 1);
//    const char* field = luaL_checkstring(L, 2);
//    double val = 0;
//    if      (0 == strcmp("x_bearing", field)) val = te->x_bearing;
//    else if (0 == strcmp("y_bearing", field)) val = te->y_bearing;
//    else if (0 == strcmp("width",     field)) val = te->width;
//    else if (0 == strcmp("height",    field)) val = te->height;
//    else if (0 == strcmp("x_advance", field)) val = te->x_advance;
//    else if (0 == strcmp("y_advance", field)) val = te->y_advance;
//    else luaL_error(L, "unknown field");
//    lua_pushnumber(L, val);
//    return 1;
//}
//
//static const struct luaL_Reg TextExtents_lib_f [] = {
//    {"new", new_TextExtents},
//    {NULL, NULL}
//};
//
//static const struct luaL_Reg TextExtents_lib_m [] = {
//    {"__newindex", set_TextExtents},
//    {"__index",    get_TextExtents},
//    {"__tostring", tostring_TextExtents},
//    {NULL, NULL}
//};
//
//static int luaopen_ltextextents (lua_State *L)
//{
//    luaL_newmetatable(L, LUACAIRO ".TextExtents.mt");
//    // lua_pushvalue(L, -1);
//    // lua_setfield(L, -2, "__index");
//    luaL_register(L, NULL, TextExtents_lib_m);
//    luaL_register(L, LUACAIRO ".TextExtents", TextExtents_lib_f);
//    lua_newtable(L);
//    lua_pushstring(L, "__call");
//    lua_pushcfunction(L, new_TextExtents);
//    lua_settable(L, -3);
//    lua_setmetatable(L, -2);
//    lua_pop(L, 1);
//
//    lua_pop(L, 1);
//    return 0;
//}




static int luaopen_lcairo_ud_text_extents(lua_State* L)
{
    luaopen_ltextextents (L);
    return 0;
}
