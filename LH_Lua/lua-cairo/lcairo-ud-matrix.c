/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



//---------------------------------------------------------------------------
// Matrix userdata for cairo_matrix_t
//---------------------------------------------------------------------------



#define check_Matrix_ud(L, idx) \
    (cairo_matrix_t *) luaL_checkudata(L, idx, LUACAIRO ".Matrix.mt")

static cairo_matrix_t check_Matrix(lua_State* L, int idx)
{
    luaL_checktype(L, idx, LUA_TTABLE);
    lua_pushvalue(L, idx); // copy table to top

    cairo_matrix_t matrix;
    matrix.xx = get_numfield(L, "xx");
    matrix.yx = get_numfield(L, "yx");
    matrix.xy = get_numfield(L, "xy");
    matrix.yy = get_numfield(L, "yy");
    matrix.x0 = get_numfield(L, "x0");
    matrix.y0 = get_numfield(L, "y0");

    lua_pop(L, 1); // remove copied table
    return matrix;
}

static int new_Matrix (lua_State *L)
{
    cairo_matrix_t mtin = {0, 0, 0, 0, 0, 0};
    int top = lua_gettop(L);
    if (top > 1) mtin = check_Matrix(L, 2);

    cairo_matrix_t *m = (cairo_matrix_t *) lua_newuserdata(L, sizeof(cairo_matrix_t));
    *m = mtin;

    luaL_getmetatable(L, LUACAIRO ".Matrix.mt");
    lua_setmetatable(L, -2);

    return 1;
}

static int tostring_Matrix (lua_State *L)
{
    cairo_matrix_t *m = check_Matrix_ud(L, 1);
    lua_pushfstring(L, "xx:%f, yx:%f, xy:%f, yy:%f, x0:%f, y0:%f",
        m->xx, m->yx, m->xy, m->yy, m->x0, m->y0);
    return 1;
}

static const struct luaL_Reg Matrix_meta_methods[] = {
    {"__tostring",                               tostring_Matrix},
    {NULL, NULL}
};

static const struct luaL_Reg Matrix_methods[] = {
    {"new",                                      new_Matrix},
    {"init",                                     l_cairo_matrix_init},
    {"init_identity",                            l_cairo_matrix_init_identity},
    {"init_translate",                           l_cairo_matrix_init_translate},
    {"init_scale",                               l_cairo_matrix_init_scale},
    {"init_rotate",                              l_cairo_matrix_init_rotate},
    {"translate",                                l_cairo_matrix_translate},
    {"scale",                                    l_cairo_matrix_scale},
    {"rotate",                                   l_cairo_matrix_rotate},
    {"invert",                                   l_cairo_matrix_invert},
    {"multiply",                                 l_cairo_matrix_multiply},
    {"transform_distance",                       l_cairo_matrix_transform_distance},
    {"transform_point",                          l_cairo_matrix_transform_point},
    {NULL, NULL}
};

static const Xet_reg_pre Matrix_getters[] = {
    {"xx",    Xet_get_number, offsetof(cairo_matrix_t, xx) },
    {"yx",    Xet_get_number, offsetof(cairo_matrix_t, yx) },
    {"xy",    Xet_get_number, offsetof(cairo_matrix_t, xy) },
    {"yy",    Xet_get_number, offsetof(cairo_matrix_t, yy) },
    {"x0",    Xet_get_number, offsetof(cairo_matrix_t, x0) },
    {"y0",    Xet_get_number, offsetof(cairo_matrix_t, y0) },
    {0, 0, 0}
};

static const Xet_reg_pre Matrix_setters[] = {
    {"xx",    Xet_set_number, offsetof(cairo_matrix_t, xx) },
    {"yx",    Xet_set_number, offsetof(cairo_matrix_t, yx) },
    {"xy",    Xet_set_number, offsetof(cairo_matrix_t, xy) },
    {"yy",    Xet_set_number, offsetof(cairo_matrix_t, yy) },
    {"x0",    Xet_set_number, offsetof(cairo_matrix_t, x0) },
    {"y0",    Xet_set_number, offsetof(cairo_matrix_t, y0) },
    {0, 0, 0}
};


static int luaopen_lmatrix (lua_State *L)
{
    int metatable, methods;

    /* create methods table, & add it to the table of globals */
    //hd.. luaL_openlib(L, YOUR_T, your_methods, 0);
    luaL_register(L, LUACAIRO ".Matrix", Matrix_methods);
    methods = lua_gettop(L);

    /* create metatable for your_t, & add it to the registry */
    luaL_newmetatable(L, LUACAIRO ".Matrix.mt");
    //hd.. luaL_openlib(L, 0, your_meta_methods, 0);  /* fill metatable */
    luaL_register(L, NULL, Matrix_meta_methods);
    metatable = lua_gettop(L);

    lua_pushliteral(L, "__metatable");
    lua_pushvalue(L, methods);      /* dup methods table*/
    lua_rawset(L, metatable);       /* hide metatable:
                                       metatable.__metatable = methods */
    lua_pushliteral(L, "__index");
    lua_pushvalue(L, metatable);    /* upvalue index 1 */
    Xet_add(L, Matrix_getters);     /* fill metatable with getters */
    lua_pushvalue(L, methods);      /* upvalue index 2 */
    lua_pushcclosure(L, Xet_index_handler, 2);
    lua_rawset(L, metatable);       /* metatable.__index = index_handler */

    lua_pushliteral(L, "__newindex");
    lua_newtable(L);                /* table for members you can set */
    Xet_add(L, Matrix_setters);     /* fill with setters */
    lua_pushcclosure(L, Xet_newindex_handler, 1);
    lua_rawset(L, metatable);       /* metatable.__newindex = newindex_handler */

    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_Matrix);
    lua_settable(L, -3);
    lua_setmetatable(L, methods);

    lua_pop(L, 1);                  /* drop metatable */
    //hd.. return 1;                /* return methods on the stack */
    lua_pop(L, 1);                  //hd.. drop methods
    return 0;
}

//unused, obsolete
//static int set_Matrix (lua_State *L)
//{
//    cairo_matrix_t *m = check_Matrix_ud(L, 1);
//    const char* field = luaL_checkstring(L, 2);
//    double val = luaL_checknumber(L, 3);
//    if      (0 == strcmp("xx", field)) m->xx = val;
//    else if (0 == strcmp("yx", field)) m->yx = val;
//    else if (0 == strcmp("xy", field)) m->xy = val;
//    else if (0 == strcmp("yy", field)) m->yy = val;
//    else if (0 == strcmp("x0", field)) m->x0 = val;
//    else if (0 == strcmp("y0", field)) m->y0 = val;
//    else luaL_error(L, "unknown field");
//    return 0;
//}
//
//static int get_Matrix (lua_State *L)
//{
//    cairo_matrix_t *m = check_Matrix_ud(L, 1);
//    const char* field = luaL_checkstring(L, 2);
//    double val = 0;
//    if      (0 == strcmp("xx", field)) val = m->xx;
//    else if (0 == strcmp("yx", field)) val = m->yx;
//    else if (0 == strcmp("xy", field)) val = m->xy;
//    else if (0 == strcmp("yy", field)) val = m->yy;
//    else if (0 == strcmp("x0", field)) val = m->x0;
//    else if (0 == strcmp("y0", field)) val = m->y0;
//    else luaL_error(L, "unknown field");
//    lua_pushnumber(L, val);
//    return 1;
//}
//
//static const struct luaL_Reg Matrix_lib_f [] = {
//    {"new", new_Matrix},
//    {NULL, NULL}
//};
//
//static const struct luaL_Reg Matrix_lib_m [] = {
//    {"__newindex", set_Matrix},
//    {"__index",    get_Matrix},
//    {"__tostring", tostring_Matrix},
//    {NULL, NULL}
//};
//
//static int luaopen_lmatrix (lua_State *L)
//{
//    luaL_newmetatable(L, LUACAIRO ".Matrix.mt");
//    // lua_pushvalue(L, -1);
//    // lua_setfield(L, -2, "__index");
//    luaL_register(L, NULL, Matrix_lib_m);
//    luaL_register(L, LUACAIRO ".Matrix", Matrix_lib_f);
//    lua_newtable(L);
//    lua_pushstring(L, "__call");
//    lua_pushcfunction(L, new_Matrix);
//    lua_settable(L, -3);
//    lua_setmetatable(L, -2);
//    lua_pop(L, 1);
//
//    lua_pop(L, 1);
//    return 0;
//}



static int luaopen_lcairo_ud_matrix(lua_State* L)
{
    luaopen_lmatrix (L);
    return 0;
}
