/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




// --------------------------------------------------------------------------
// FontExtents userdata for cairo_font_extents_t
// --------------------------------------------------------------------------



#define check_FontExtents_ud(L, idx) \
    (cairo_font_extents_t *) luaL_checkudata(L, idx, LUACAIRO ".FontExtents.mt")

static cairo_font_extents_t check_FontExtents (lua_State* L, int idx)
{
    luaL_checktype(L, idx, LUA_TTABLE);
    lua_pushvalue(L, idx); // copy table to top

    cairo_font_extents_t extents;
    extents.ascent        = get_numfield(L, "ascent");
    extents.descent       = get_numfield(L, "descent");
    extents.height        = get_numfield(L, "height");
    extents.max_x_advance = get_numfield(L, "max_x_advance");
    extents.max_y_advance = get_numfield(L, "max_y_advance");

    lua_pop(L, 1); // remove copied table
    return extents;
}

static int new_FontExtents (lua_State *L)
{
    cairo_font_extents_t fein = {0, 0, 0, 0, 0};
    int top = lua_gettop(L);
    if (top > 1) fein = check_FontExtents (L, 2);

    cairo_font_extents_t *fe = (cairo_font_extents_t *) lua_newuserdata(L, sizeof(cairo_font_extents_t));
    *fe = fein;

    luaL_getmetatable(L, LUACAIRO ".FontExtents.mt");
    lua_setmetatable(L, -2);

    return 1;
}

static int tostring_FontExtents (lua_State *L)
{
    cairo_font_extents_t *fe = check_FontExtents_ud(L, 1);
    lua_pushfstring(L, "ascent:%f, descent:%f, height:%f, max_x_advance:%f, max_y_advance:%f",
        fe->ascent, fe->descent, fe->height, fe->max_x_advance, fe->max_y_advance);
    return 1;
}

static const struct luaL_Reg FontExtents_meta_methods[] = {
    {"__tostring",                               tostring_FontExtents},
    {NULL, NULL}
};

static const struct luaL_Reg FontExtents_methods[] = {
    {"new",                                      new_FontExtents},
    {NULL, NULL}
};

static const Xet_reg_pre FontExtents_getters[] = {
    {"ascent",        Xet_get_number, offsetof(cairo_font_extents_t, ascent)        },
    {"descent",       Xet_get_number, offsetof(cairo_font_extents_t, descent)       },
    {"height",        Xet_get_number, offsetof(cairo_font_extents_t, height)        },
    {"max_x_advance", Xet_get_number, offsetof(cairo_font_extents_t, max_x_advance) },
    {"max_y_advance", Xet_get_number, offsetof(cairo_font_extents_t, max_y_advance) },
    {0, 0, 0}
};

static const Xet_reg_pre FontExtents_setters[] = {
    {"ascent",        Xet_set_number, offsetof(cairo_font_extents_t, ascent)        },
    {"descent",       Xet_set_number, offsetof(cairo_font_extents_t, descent)       },
    {"height",        Xet_set_number, offsetof(cairo_font_extents_t, height)        },
    {"max_x_advance", Xet_set_number, offsetof(cairo_font_extents_t, max_x_advance) },
    {"max_y_advance", Xet_set_number, offsetof(cairo_font_extents_t, max_y_advance) },
    {0, 0, 0}
};


static int luaopen_lfontextents (lua_State *L)
{
    int metatable, methods;

    /* create methods table, & add it to the table of globals */
    //hd.. luaL_openlib(L, YOUR_T, your_methods, 0);
    luaL_register(L, LUACAIRO ".FontExtents", FontExtents_methods);
    methods = lua_gettop(L);

    /* create metatable for your_t, & add it to the registry */
    luaL_newmetatable(L, LUACAIRO ".FontExtents.mt");
    //hd.. luaL_openlib(L, 0, your_meta_methods, 0);  /* fill metatable */
    luaL_register(L, NULL, FontExtents_meta_methods);
    metatable = lua_gettop(L);

    lua_pushliteral(L, "__metatable");
    lua_pushvalue(L, methods);           /* dup methods table*/
    lua_rawset(L, metatable);            /* hide metatable:
                                            metatable.__metatable = methods */
    lua_pushliteral(L, "__index");
    lua_pushvalue(L, metatable);         /* upvalue index 1 */
    Xet_add(L, FontExtents_getters);     /* fill metatable with getters */
    lua_pushvalue(L, methods);           /* upvalue index 2 */
    lua_pushcclosure(L, Xet_index_handler, 2);
    lua_rawset(L, metatable);            /* metatable.__index = index_handler */

    lua_pushliteral(L, "__newindex");
    lua_newtable(L);                     /* table for members you can set */
    Xet_add(L, FontExtents_setters);     /* fill with setters */
    lua_pushcclosure(L, Xet_newindex_handler, 1);
    lua_rawset(L, metatable);            /* metatable.__newindex = newindex_handler */

    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_FontExtents);
    lua_settable(L, -3);
    lua_setmetatable(L, methods);

    lua_pop(L, 1);                       /* drop metatable */
    //hd.. return 1;                     /* return methods on the stack */
    lua_pop(L, 1);                       //hd.. drop methods
    return 0;
}

//unused, obsolete
//static int set_FontExtents (lua_State *L)
//{
//    cairo_font_extents_t *fe = check_FontExtents_ud(L, 1);
//    const char* field = luaL_checkstring(L, 2);
//    double val = luaL_checknumber(L, 3);
//    if      (0 == strcmp("ascent",        field)) fe->ascent = val;
//    else if (0 == strcmp("descent",       field)) fe->descent = val;
//    else if (0 == strcmp("height",        field)) fe->height= val;
//    else if (0 == strcmp("max_x_advance", field)) fe->max_x_advance = val;
//    else if (0 == strcmp("max_y_advance", field)) fe->max_y_advance = val;
//    else luaL_error(L, "unknown field");
//    return 0;
//}
//
//static int get_FontExtents (lua_State *L)
//{
//    cairo_font_extents_t *fe = check_FontExtents_ud(L, 1);
//    const char* field = luaL_checkstring(L, 2);
//    double val = 0;
//    if      (0 == strcmp("ascent",        field)) val = fe->ascent;
//    else if (0 == strcmp("descent",       field)) val = fe->descent;
//    else if (0 == strcmp("height",        field)) val = fe->height;
//    else if (0 == strcmp("max_x_advance", field)) val = fe->max_x_advance;
//    else if (0 == strcmp("max_y_advance", field)) val = fe->max_y_advance;
//    else luaL_error(L, "unknown field");
//    lua_pushnumber(L, val);
//    return 1;
//}
//
//static const struct luaL_Reg FontExtents_lib_f [] = {
//    {"new", new_FontExtents},
//    {NULL, NULL}
//};
//
//static const struct luaL_Reg FontExtents_lib_m [] = {
//    {"__newindex", set_FontExtents},
//    {"__index",    get_FontExtents},
//    {"__tostring", tostring_FontExtents},
//    {NULL, NULL}
//};
//
//static int luaopen_lfontextents (lua_State *L)
//{
//    luaL_newmetatable(L, LUACAIRO ".FontExtents.mt");                  // mt
//  //lua_pushvalue(L, -1);                                            // mt mt
//  //lua_setfield(L, -2, "__index");                                  // mt          (mt.__index = mt)
//    luaL_register(L, NULL, FontExtents_lib_m);                       // mt
//    luaL_register(L, LUACAIRO ".FontExtents", FontExtents_lib_f);      // mt ft
//    lua_newtable(L);                                                 // mt ft t
//    lua_pushstring(L, "__call");                                     // mt ft t "__call"
//    lua_pushcfunction(L, new_FontExtents);                           // mt ft t "__call" fun
//    lua_settable(L, -3);                                             // mt ft t     (t.__call = fun)
//    lua_setmetatable(L, -2);                                         // mt ft       (ft.meta = t)
//    lua_pop(L, 1);                                                   // mt
//
//    lua_pop(L, 1);
//    return 0;
//}



static int luaopen_lcairo_ud_font_extents(lua_State* L)
{
    luaopen_lfontextents (L);
    return 0;
}
