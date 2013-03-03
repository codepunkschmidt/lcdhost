/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




// --------------------------------------------------------------------------
// FontOptions userdata for cairo_font_options_t
// --------------------------------------------------------------------------



#if LUACAIRO_HAS_OO_INTERFACE

typedef struct _FontOptions
{
    cairo_font_options_t* fo_;
    int                   havefo_;
} FontOptions;

#define check_FontOptions_ud(L, idx) \
    (FontOptions *) luaL_checkudata(L, idx, LUACAIRO ".FontOptions.mt")

static int new_FontOptions (lua_State *L)
{
    lua_remove(L, 1); // remove cairo.FontOptions

  //{"create",                                   l_cairo_font_options_create},
    cairo_font_options_t *fo = cairo_font_options_create();

    FontOptions *o = (FontOptions *) lua_newuserdata(L, sizeof(FontOptions));
    o->fo_     = fo;
    o->havefo_ = 1;

    luaL_getmetatable(L, LUACAIRO ".FontOptions.mt");
    lua_setmetatable(L, -2);

    return 1;
}

static int new_FontOptionsFromLUD (lua_State *L)
{
    cairo_font_options_t *fo = (cairo_font_options_t *) check_lightuserdata(L, 1);
    int havefo  = lua_toboolean(L, 2);

    FontOptions *o = (FontOptions *) lua_newuserdata(L, sizeof(FontOptions));
    o->fo_     = fo;
    o->havefo_ = havefo;

    luaL_getmetatable(L, LUACAIRO ".FontOptions.mt");
    lua_setmetatable(L, -2);

    return 1;
}

static int gc_FontOptions (lua_State *L)
{
    FontOptions *o = check_FontOptions_ud(L, 1);

    if (o->fo_ && o->havefo_)
    {
        cairo_font_options_destroy (o->fo_);
        o->fo_     = NULL;
        o->havefo_ = 0;
    }

    return 0;
}

static int tostring_FontOptions (lua_State *L)
{
    FontOptions *o = check_FontOptions_ud(L, 1);
    lua_pushfstring(L, "FontOptions (%p), cairo_font_options_t (%p)", (void*)o, (void*)o->fo_);
    return 1;
}

static const struct luaL_Reg FontOptions_lib_f [] = {
    {"new", new_FontOptions},
    {NULL, NULL}
};

static const struct luaL_Reg FontOptions_lib_m [] = {
    {"__gc",                                     gc_FontOptions},
    {"__tostring",                               tostring_FontOptions},

  //{"create",                                   l_cairo_font_options_create},
    {"copy",                                     l_cairo_font_options_copy},
    {"destroy",                                  gc_FontOptions},
    {"status",                                   l_cairo_font_options_status},
    {"merge",                                    l_cairo_font_options_merge},
    {"equal",                                    l_cairo_font_options_equal},
    {"hash",                                     l_cairo_font_options_hash},
    {"set_antialias",                            l_cairo_font_options_set_antialias},
    {"get_antialias",                            l_cairo_font_options_get_antialias},
    {"set_subpixel_order",                       l_cairo_font_options_set_subpixel_order},
    {"get_subpixel_order",                       l_cairo_font_options_get_subpixel_order},
    {"set_hint_style",                           l_cairo_font_options_set_hint_style},
    {"get_hint_style",                           l_cairo_font_options_get_hint_style},
    {"set_hint_metrics",                         l_cairo_font_options_set_hint_metrics},
    {"get_hint_metrics",                         l_cairo_font_options_get_hint_metrics},

    {NULL, NULL}
};


static int luaopen_lfontoptions (lua_State *L)
{
    luaL_newmetatable(L, LUACAIRO ".FontOptions.mt");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, FontOptions_lib_m);
    luaL_register(L, LUACAIRO ".FontOptions", FontOptions_lib_f);
    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_FontOptions);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    lua_pop(L, 1);

    lua_pop(L, 1);
    return 0;
}

#endif  /* LUACAIRO_HAS_OO_INTERFACE */



static int luaopen_lcairo_ud_font_options(lua_State* L)
{
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lfontoptions (L);

#endif  /* LUACAIRO_HAS_OO_INTERFACE */
    return 0;
}
