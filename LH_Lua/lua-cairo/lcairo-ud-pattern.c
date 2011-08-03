/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




///--------------------------------------------------------------------------
/// Pattern userdata for cairo_pattern_t
///--------------------------------------------------------------------------



#if LUACAIRO_HAS_OO_INTERFACE

typedef struct _Pattern
{
    cairo_pattern_t* cp_;
    int              havecp_;
    int              typecp_;
} lua_Pattern;

static lua_Pattern* check_Pattern_ud(lua_State *L, int idx)
{
    lua_Pattern* cp = NULL;
    //if (!cp) cp = (Pattern*) get_userdata(L, idx, LUACAIRO ".Pattern.mt");
    if (!cp) cp = (lua_Pattern*) get_userdata(L, idx, LUACAIRO ".SolidPattern.mt");
    if (!cp) cp = (lua_Pattern*) get_userdata(L, idx, LUACAIRO ".SurfacePattern.mt");
    //if (!cp) cp = (Pattern*) get_userdata(L, idx, LUACAIRO ".Gradient.mt");
    if (!cp) cp = (lua_Pattern*) get_userdata(L, idx, LUACAIRO ".LinearGradient.mt");
    if (!cp) cp = (lua_Pattern*) get_userdata(L, idx, LUACAIRO ".RadialGradient.mt");
    if (!cp) luaL_typerror(L, idx, LUACAIRO ".Pattern");
    return cp;
}

static int new_Pattern (lua_State *L, const char *tname, cairo_pattern_t* cp, int typecp, int havecp)
{
    lua_Pattern *o = (lua_Pattern *) lua_newuserdata(L, sizeof(lua_Pattern));
    o->cp_     = cp;
    o->havecp_ = havecp;
    o->typecp_ = typecp;

    luaL_getmetatable(L, tname);
    lua_setmetatable(L, -2);

    return 1;
}

static int new_PatternFromLUD (lua_State *L)
{
    cairo_pattern_t *cp = (cairo_pattern_t *) check_lightuserdata(L, 1);
    int havecp          = lua_toboolean(L, 2);
    int typecp          = cairo_pattern_get_type(cp);

    switch (typecp) {
    case CAIRO_PATTERN_TYPE_SOLID:   return new_Pattern(L, LUACAIRO ".SolidPattern.mt", cp, typecp, havecp);
    case CAIRO_PATTERN_TYPE_SURFACE: return new_Pattern(L, LUACAIRO ".SurfacePattern.mt", cp, typecp, havecp);
    case CAIRO_PATTERN_TYPE_LINEAR:  return new_Pattern(L, LUACAIRO ".LinearGradient.mt", cp, typecp, havecp);
    case CAIRO_PATTERN_TYPE_RADIAL:  return new_Pattern(L, LUACAIRO ".RadialGradient.mt", cp, typecp, havecp);
    }

    return 0;
}

static int gc_Pattern (lua_State *L)
{
    lua_Pattern *o = check_Pattern_ud(L, 1);

    if (o->cp_ && o->havecp_)
    {
        cairo_pattern_destroy(o->cp_);
        o->cp_     = NULL;
        o->havecp_ = 0;
        o->typecp_ = 0;
    }

    return 0;
}

static int tostring_Pattern (lua_State *L)
{
    lua_Pattern *o = check_Pattern_ud(L, 1);
    lua_pushfstring(L, "Pattern (%p), cairo_pattern_t (%p), Type:%d", (void*)o, (void*)o->cp_, o->typecp_);
    return 1;
}



static int luaopen_lpattern (lua_State *L)
{
    return 0;
}


#endif  /* LUACAIRO_HAS_OO_INTERFACE */



static int luaopen_lcairo_ud_pattern(lua_State* L)
{
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lpattern (L);

#endif  /* LUACAIRO_HAS_OO_INTERFACE */
    return 0;
}
