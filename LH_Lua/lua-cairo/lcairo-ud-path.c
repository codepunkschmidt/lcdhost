/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




// --------------------------------------------------------------------------
// Path userdata for cairo_path_t
// --------------------------------------------------------------------------



#if LUACAIRO_HAS_OO_INTERFACE

typedef struct _Path
{
    cairo_path_t* path_;
    int           havepath_;
} Path;

#define check_Path_ud(L, idx) \
    (Path *) luaL_checkudata(L, idx, LUACAIRO ".Path.mt")

static int new_Path (lua_State *L)
{
    lua_remove(L, 1); // remove cairo.Path

    // cairo_public cairo_path_t *
    // cairo_copy_path (cairo_t *cr);
    
    // cairo_public cairo_path_t *
    // cairo_copy_path_flat (cairo_t *cr);
    
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_path_t *path = cairo_copy_path (cr);
    
    Path *o = (Path *) lua_newuserdata(L, sizeof(Path));
    o->path_     = path;
    o->havepath_ = 1;

    luaL_getmetatable(L, LUACAIRO ".Path.mt");
    lua_setmetatable(L, -2);

    return 1;
}

static int new_PathFromLUD (lua_State *L)
{
    cairo_path_t *path = (cairo_path_t *) check_lightuserdata(L, 1);
    int havepath  = lua_toboolean(L, 2);

    Path *o = (Path *) lua_newuserdata(L, sizeof(Path));
    o->path_     = path;
    o->havepath_ = havepath;

    luaL_getmetatable(L, LUACAIRO ".Path.mt");
    lua_setmetatable(L, -2);

    return 1;
}

static int gc_Path (lua_State *L)
{
    Path *o = check_Path_ud(L, 1);

    if (o->path_ && o->havepath_)
    {
        cairo_path_destroy (o->path_);
        o->path_     = NULL;
        o->havepath_ = 0;
    }

    return 0;
}

static int tostring_Path (lua_State *L)
{
    Path *o = check_Path_ud(L, 1);
    lua_pushfstring(L, "Path (%p), cairo_path_t (%p)", (void*)o, (void*)o->path_);
    return 1;
}

static const struct luaL_Reg Path_lib_f [] = {
    {"new", new_Path},
    {NULL, NULL}
};

static const struct luaL_Reg Path_lib_m [] = {
    {"__gc",                                     gc_Path},
    {"__tostring",                               tostring_Path},
    {"destroy",                                  gc_Path},
    
    {"status",                                   l_cairo_path_status},
    {"num_data",                                 l_cairo_path_num_data},
    {"data_header_type",                         l_cairo_path_data_header_type},
    {"data_header_length",                       l_cairo_path_data_header_length},
    {"data_point",                               l_cairo_path_data_point},

    {NULL, NULL}
};


static int luaopen_lpath (lua_State *L)
{
    luaL_newmetatable(L, LUACAIRO ".Path.mt");             // mt
    lua_pushvalue(L, -1);                                  // mt mt
    lua_setfield(L, -2, "__index");                        // mt       (mt.__index = mt)
    luaL_register(L, NULL, Path_lib_m);                    // mt
    luaL_register(L, LUACAIRO ".Path", Path_lib_f);        // mt ct
    lua_newtable(L);                                       // mt ct t
    lua_pushstring(L, "__call");                           // mt ct t "__call"
    lua_pushcfunction(L, new_Path);                        // mt ct t "__call" fun
    lua_settable(L, -3);                                   // mt ct t  (t.__call = fun)
    lua_setmetatable(L, -2);                               // mt ct    (ct.meta = t)
    lua_pop(L, 1);                                         // mt

    lua_pop(L, 1);
    return 0;
}

#endif  /* LUACAIRO_HAS_OO_INTERFACE */



static int luaopen_lcairo_ud_path (lua_State* L)
{
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lpath (L);

#endif /* LUACAIRO_HAS_OO_INTERFACE */
    return 0;
}
