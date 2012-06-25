/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



// --------------------------------------------------------------------------
// getters
// --------------------------------------------------------------------------


static cairo_font_extents_t* get_cairo_font_extents_t (lua_State *L, int idx)
{
    return check_FontExtents_ud(L, idx);
}



static cairo_text_extents_t* get_cairo_text_extents_t (lua_State *L, int idx)
{
    return check_TextExtents_ud(L, idx);
}



static cairo_matrix_t* get_cairo_matrix_t (lua_State *L, int idx)
{
    return check_Matrix_ud(L, idx);
}


static cairo_t* get_cairo_t (lua_State *L, int idx)
{
    int t = lua_type(L, idx);

    if (t == LUA_TLIGHTUSERDATA)
    {
        return (cairo_t *) check_lightuserdata(L, idx);
    }
#if LUACAIRO_HAS_OO_INTERFACE
    else if (t == LUA_TUSERDATA)
    {
        Context *o = check_Context_ud(L, idx);
        return o->cr_;

    }
#endif
    luaL_error(L, "cairo_t expected!");
    return NULL;
}


static cairo_surface_t* get_cairo_surface_t (lua_State *L, int idx)
{
    int t = lua_type(L, idx);

    if (t == LUA_TLIGHTUSERDATA)
    {
        return (cairo_surface_t *) check_lightuserdata(L, idx);
    }
#if LUACAIRO_HAS_OO_INTERFACE
    else if (t == LUA_TUSERDATA)
    {
        Surface *o = check_Surface_ud(L, idx);
        return o->cs_;

    }
#endif
    luaL_error(L, "cairo_surface_t expected!");
    return NULL;
}


static cairo_pattern_t* get_cairo_pattern_t (lua_State *L, int idx)
{
    int t = lua_type(L, idx);

    if (t == LUA_TLIGHTUSERDATA)
    {
        return (cairo_pattern_t *) check_lightuserdata(L, idx);
    }
#if LUACAIRO_HAS_OO_INTERFACE
    else if (t == LUA_TUSERDATA)
    {
        lua_Pattern *o = check_Pattern_ud(L, idx);
        return o->cp_;

    }
#endif
    luaL_error(L, "cairo_pattern_t expected!");
    return NULL;
}


static cairo_font_options_t* get_cairo_font_options_t (lua_State *L, int idx)
{
    int t = lua_type(L, idx);

    if (t == LUA_TLIGHTUSERDATA)
    {
        return (cairo_font_options_t *) check_lightuserdata(L, idx);
    }
#if LUACAIRO_HAS_OO_INTERFACE
    else if (t == LUA_TUSERDATA)
    {
        FontOptions *o = check_FontOptions_ud(L, idx);
        return o->fo_;

    }
#endif
    luaL_error(L, "cairo_font_options_t expected!");
    return NULL;
}


static cairo_font_face_t* get_cairo_font_face_t (lua_State *L, int idx)
{
    int t = lua_type(L, idx);

    if (t == LUA_TLIGHTUSERDATA)
    {
        return (cairo_font_face_t *) check_lightuserdata(L, idx);
    }
#if LUACAIRO_HAS_OO_INTERFACE
    else if (t == LUA_TUSERDATA)
    {
        FontFace *o = check_FontFace_ud (L, idx);
        return o->ff_;

    }
#endif
    luaL_error(L, "cairo_font_face_t expected!");
    return NULL;
}


static cairo_scaled_font_t* get_cairo_scaled_font_t (lua_State *L, int idx)
{
    int t = lua_type(L, idx);

    if (t == LUA_TLIGHTUSERDATA)
    {
        return (cairo_scaled_font_t *) check_lightuserdata(L, idx);
    }
#if LUACAIRO_HAS_OO_INTERFACE
    else if (t == LUA_TUSERDATA)
    {
        ScaledFont *o = check_ScaledFont_ud(L, idx);
        return o->sf_;

    }
#endif
    luaL_error(L, "cairo_scaled_font_t expected!");
    return NULL;
}


static cairo_path_t* get_cairo_path_t (lua_State *L, int idx)
{
    int t = lua_type(L, idx);

    if (t == LUA_TLIGHTUSERDATA)
    {
        return (cairo_path_t *) check_lightuserdata(L, idx);
    }
#if LUACAIRO_HAS_OO_INTERFACE
    else if (t == LUA_TUSERDATA)
    {
        Path *o = check_Path_ud(L, idx);
        return o->path_;

    }
#endif
    luaL_error(L, "cairo_path_t expected!");
    return NULL;
}


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
static cairo_rectangle_int_t* get_cairo_rectangle_int_t (lua_State *L, int idx)
{
    return check_RectangleInt_ud(L, idx);
}


static cairo_region_t* get_cairo_region_t (lua_State *L, int idx)
{
    int t = lua_type(L, idx);

    if (t == LUA_TLIGHTUSERDATA)
    {
        return (cairo_region_t *) check_lightuserdata(L, idx);
    }
#if LUACAIRO_HAS_OO_INTERFACE
    else if (t == LUA_TUSERDATA)
    {
        Region *o = check_Region_ud(L, idx);
        return o->reg_;

    }
#endif
    luaL_error(L, "cairo_region_t expected!");
    return NULL;
}


static cairo_device_t* get_cairo_device_t (lua_State *L, int idx)
{
    int t = lua_type(L, idx);

    if (t == LUA_TLIGHTUSERDATA)
    {
        return (cairo_device_t *) check_lightuserdata(L, idx);
    }
#if LUACAIRO_HAS_OO_INTERFACE
    else if (t == LUA_TUSERDATA)
    {
        Device *o = check_Device_ud(L, idx);
        return o->dev_;

    }
#endif
    luaL_error(L, "cairo_device_t expected!");
    return NULL;
}
#endif /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0) */
