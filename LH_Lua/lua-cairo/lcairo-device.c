/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)


/* Backend device manipulation */

// cairo_public cairo_device_t *
// cairo_device_reference (cairo_device_t *device);
static int l_cairo_device_reference (lua_State* L)
{
    cairo_device_t *device = get_cairo_device_t (L, 1);
    cairo_device_t *v = cairo_device_reference (device);
    lua_pushlightuserdata(L, v);
    return 1;
}

// cairo_public cairo_device_type_t
// cairo_device_get_type (cairo_device_t *device);
static int l_cairo_device_get_type (lua_State* L)
{
    cairo_device_t *device = get_cairo_device_t (L, 1);
    cairo_device_type_t v = cairo_device_get_type (device);
    lua_pushnumber(L, v);
    return 1;
}

// cairo_public cairo_status_t
// cairo_device_status (cairo_device_t *device);
static int l_cairo_device_status (lua_State* L)
{
    cairo_device_t *device = get_cairo_device_t (L, 1);
    cairo_status_t v = cairo_device_status (device);
    lua_pushnumber(L, v);
    return 1;
}

// cairo_public cairo_status_t
// cairo_device_acquire (cairo_device_t *device);
static int l_cairo_device_acquire (lua_State* L)
{
    cairo_device_t *device = get_cairo_device_t (L, 1);
    cairo_status_t v = cairo_device_acquire (device);
    lua_pushnumber(L, v);
    return 1;
}

// cairo_public void
// cairo_device_release (cairo_device_t *device);
static int l_cairo_device_release (lua_State* L)
{
    cairo_device_t *device = get_cairo_device_t (L, 1);
    cairo_device_release (device);
    return 0;
}

// cairo_public void
// cairo_device_flush (cairo_device_t *device);
static int l_cairo_device_flush (lua_State* L)
{
    cairo_device_t *device = get_cairo_device_t (L, 1);
    cairo_device_flush (device);
    return 0;
}

// cairo_public void
// cairo_device_finish (cairo_device_t *device);
static int l_cairo_device_finish (lua_State* L)
{
    cairo_device_t *device = get_cairo_device_t (L, 1);
    cairo_device_finish (device);
    return 0;
}

// cairo_public void
// cairo_device_destroy (cairo_device_t *device);
static int l_cairo_device_destroy (lua_State* L)
{
    cairo_device_t *device = get_cairo_device_t (L, 1);
    cairo_device_destroy (device);
    return 0;
}

// cairo_public unsigned int
// cairo_device_get_reference_count (cairo_device_t *device);
static int l_cairo_device_get_reference_count (lua_State* L)
{
    cairo_device_t *device = get_cairo_device_t (L, 1);
    int v = cairo_device_get_reference_count (device);
    lua_pushnumber(L, v);
    return 1;
}

// cairo_public void *
// cairo_device_get_user_data (cairo_device_t		 *device,
// 			    const cairo_user_data_key_t *key);
static int l_cairo_device_get_user_data (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_status_t
// cairo_device_set_user_data (cairo_device_t		 *device,
// 			    const cairo_user_data_key_t *key,
// 			    void			 *user_data,
// 			    cairo_destroy_func_t	  destroy);
static int l_cairo_device_set_user_data (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}



static const struct luaL_Reg device_f [] = {
    {"device_reference",                   l_cairo_device_reference},
    {"device_get_type",                    l_cairo_device_get_type},
    {"device_status",                      l_cairo_device_status},
    {"device_acquire",                     l_cairo_device_acquire},
    {"device_release",                     l_cairo_device_release},
    {"device_flush",                       l_cairo_device_flush},
    {"device_finish",                      l_cairo_device_finish},
    {"device_destroy",                     l_cairo_device_destroy},
    {"device_get_reference_count",         l_cairo_device_get_reference_count},
    {"device_get_user_data",               l_cairo_device_get_user_data},
    {"device_set_user_data",               l_cairo_device_set_user_data},

	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0) */



static int luaopen_lcairo_device (lua_State* L)
{
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)

    // register functions
    luaL_register(L, LUACAIRO, device_f);
    
#endif /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0) */
    return 0;
}
