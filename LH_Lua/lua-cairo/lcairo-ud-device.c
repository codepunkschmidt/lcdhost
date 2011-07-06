/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




// --------------------------------------------------------------------------
// Device userdata for cairo_device_t
// --------------------------------------------------------------------------



#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
#if LUACAIRO_HAS_OO_INTERFACE

typedef struct _Device
{
    cairo_device_t* dev_;
    int             havedev_;
} Device;

#define check_Device_ud(L, idx) \
    (Device *) luaL_checkudata(L, idx, LUACAIRO ".Device.mt")

static int new_Device (lua_State *L)
{
    lua_remove(L, 1); // remove cairo.Device

    // cairo_public cairo_device_t *
    // cairo_surface_get_device (cairo_surface_t *surface);
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_device_t *dev = cairo_surface_get_device (surface);
    
    Device *o = (Device *) lua_newuserdata(L, sizeof(Device));
    o->dev_     = dev;
    o->havedev_ = 1;

    luaL_getmetatable(L, LUACAIRO ".Device.mt");
    lua_setmetatable(L, -2);

    return 1;
}

static int new_DeviceFromLUD (lua_State *L)
{
    cairo_device_t *dev = (cairo_device_t *) check_lightuserdata(L, 1);
    int havedev  = lua_toboolean(L, 2);

    Device *o = (Device *) lua_newuserdata(L, sizeof(Device));
    o->dev_     = dev;
    o->havedev_ = havedev;

    luaL_getmetatable(L, LUACAIRO ".Device.mt");
    lua_setmetatable(L, -2);

    return 1;
}

static int gc_Device (lua_State *L)
{
    Device *o = check_Device_ud(L, 1);

    if (o->dev_ && o->havedev_)
    {
        cairo_device_destroy (o->dev_);
        o->dev_     = NULL;
        o->havedev_ = 0;
    }

    return 0;
}

static int tostring_Device (lua_State *L)
{
    Device *o = check_Device_ud(L, 1);
    lua_pushfstring(L, "Device (%p), cairo_device_t (%p)", (void*)o, (void*)o->dev_);
    return 1;
}

static const struct luaL_Reg Device_lib_f [] = {
    {"new", new_Device},
    {NULL, NULL}
};

static const struct luaL_Reg Device_lib_m [] = {
    {"__gc",                                     gc_Device},
    {"__tostring",                               tostring_Device},

    {"reference",                                l_cairo_device_reference},
    {"get_type",                                 l_cairo_device_get_type},
    {"status",                                   l_cairo_device_status},
    {"acquire",                                  l_cairo_device_acquire},
    {"release",                                  l_cairo_device_release},
    {"flush",                                    l_cairo_device_flush},
    {"finish",                                   l_cairo_device_finish},
    {"destroy",                                  gc_Device},
    {"get_reference_count",                      l_cairo_device_get_reference_count},
    {"get_user_data",                            l_cairo_device_get_user_data},
    {"set_user_data",                            l_cairo_device_set_user_data},

    {NULL, NULL}
};


static int luaopen_ldevice (lua_State *L)
{
    luaL_newmetatable(L, LUACAIRO ".Device.mt");           // mt
    lua_pushvalue(L, -1);                                  // mt mt
    lua_setfield(L, -2, "__index");                        // mt       (mt.__index = mt)
    luaL_register(L, NULL, Device_lib_m);                  // mt
    luaL_register(L, LUACAIRO ".Device", Device_lib_f);    // mt ct
    lua_newtable(L);                                       // mt ct t
    lua_pushstring(L, "__call");                           // mt ct t "__call"
    lua_pushcfunction(L, new_Device);                      // mt ct t "__call" fun
    lua_settable(L, -3);                                   // mt ct t  (t.__call = fun)
    lua_setmetatable(L, -2);                               // mt ct    (ct.meta = t)
    lua_pop(L, 1);                                         // mt

    lua_pop(L, 1);
    return 0;
}

#endif  /* LUACAIRO_HAS_OO_INTERFACE */
#endif /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0) */



static int luaopen_lcairo_ud_device (lua_State* L)
{
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_ldevice (L);

#endif /* LUACAIRO_HAS_OO_INTERFACE */
#endif /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0) */
    return 0;
}
