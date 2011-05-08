/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




// --------------------------------------------------------------------------
// Context userdata for cairo_t
// --------------------------------------------------------------------------



#if LUACAIRO_HAS_OO_INTERFACE

typedef struct _Context
{
    cairo_t* cr_;
    int      havecr_;
} Context;

#define check_Context_ud(L, idx) \
    (Context *) luaL_checkudata(L, idx, LUACAIRO ".Context.mt")

static int new_Context (lua_State *L)
{
    lua_remove(L, 1); // remove cairo.Context

  //{"create",                                   l_cairo_create},
    cairo_surface_t* target = get_cairo_surface_t (L, 1);
    cairo_t *cr = cairo_create(target);

    Context *o = (Context *) lua_newuserdata(L, sizeof(Context));
    o->cr_     = cr;
    o->havecr_ = 1;

    luaL_getmetatable(L, LUACAIRO ".Context.mt");
    lua_setmetatable(L, -2);

    return 1;
}

static int new_ContextFromLUD (lua_State *L)
{
    cairo_t *cr = (cairo_t *) check_lightuserdata(L, 1);
    int havecr  = lua_toboolean(L, 2);

    Context *o = (Context *) lua_newuserdata(L, sizeof(Context));
    o->cr_     = cr;
    o->havecr_ = havecr;

    luaL_getmetatable(L, LUACAIRO ".Context.mt");
    lua_setmetatable(L, -2);

    return 1;
}

static int gc_Context (lua_State *L)
{
    Context *o = check_Context_ud(L, 1);

    if (o->cr_ && o->havecr_)
    {
        cairo_destroy (o->cr_);
        o->cr_     = NULL;
        o->havecr_ = 0;
    }

    return 0;
}

static int tostring_Context (lua_State *L)
{
    Context *o = check_Context_ud(L, 1);
    lua_pushfstring(L, "Context (%p), cairo_t (%p)", (void*)o, (void*)o->cr_);
    return 1;
}

static const struct luaL_Reg Context_lib_f [] = {
    {"new", new_Context},
    {NULL, NULL}
};

static const struct luaL_Reg Context_lib_m [] = {
    {"__gc",                                     gc_Context},
    {"__tostring",                               tostring_Context},

    {"version",                                  l_cairo_version},
    {"version_string",                           l_cairo_version_string},
  //{"create",                                   l_cairo_create},
    {"reference",                                l_cairo_reference},
    {"destroy",                                  gc_Context},
    {"get_reference_count",                      l_cairo_get_reference_count},
    {"get_user_data",                            l_cairo_get_user_data},
    {"set_user_data",                            l_cairo_set_user_data},
    {"save",                                     l_cairo_save},
    {"restore",                                  l_cairo_restore},
    {"push_group",                               l_cairo_push_group},
    {"push_group_with_content",                  l_cairo_push_group_with_content},
    {"pop_group",                                l_cairo_pop_group},
    {"pop_group_to_source",                      l_cairo_pop_group_to_source},
    {"set_operator",                             l_cairo_set_operator},
    {"set_source",                               l_cairo_set_source},
    {"set_source_rgb",                           l_cairo_set_source_rgb},
    {"set_source_rgba",                          l_cairo_set_source_rgba},
    {"set_source_surface",                       l_cairo_set_source_surface},
    {"set_tolerance",                            l_cairo_set_tolerance},
    {"set_antialias",                            l_cairo_set_antialias},
    {"set_fill_rule",                            l_cairo_set_fill_rule},
    {"set_line_width",                           l_cairo_set_line_width},
    {"set_line_cap",                             l_cairo_set_line_cap},
    {"set_line_join",                            l_cairo_set_line_join},
    {"set_dash",                                 l_cairo_set_dash},
    {"set_miter_limit",                          l_cairo_set_miter_limit},
    {"translate",                                l_cairo_translate},
    {"scale",                                    l_cairo_scale},
    {"rotate",                                   l_cairo_rotate},
    {"transform",                                l_cairo_transform},
    {"set_matrix",                               l_cairo_set_matrix},
    {"identity_matrix",                          l_cairo_identity_matrix},
    {"user_to_device",                           l_cairo_user_to_device},
    {"user_to_device_distance",                  l_cairo_user_to_device_distance},
    {"device_to_user",                           l_cairo_device_to_user},
    {"device_to_user_distance",                  l_cairo_device_to_user_distance},
    {"new_path",                                 l_cairo_new_path},
    {"move_to",                                  l_cairo_move_to},
    {"new_sub_path",                             l_cairo_new_sub_path},
    {"line_to",                                  l_cairo_line_to},
    {"curve_to",                                 l_cairo_curve_to},
    {"arc",                                      l_cairo_arc},
    {"arc_negative",                             l_cairo_arc_negative},
    {"rel_move_to",                              l_cairo_rel_move_to},
    {"rel_line_to",                              l_cairo_rel_line_to},
    {"rel_curve_to",                             l_cairo_rel_curve_to},
    {"rectangle",                                l_cairo_rectangle},
    {"close_path",                               l_cairo_close_path},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    {"path_extents",                             l_cairo_path_extents},
#endif
    {"paint",                                    l_cairo_paint},
    {"paint_with_alpha",                         l_cairo_paint_with_alpha},
    {"mask",                                     l_cairo_mask},
    {"mask_surface",                             l_cairo_mask_surface},
    {"stroke",                                   l_cairo_stroke},
    {"stroke_preserve",                          l_cairo_stroke_preserve},
    {"fill",                                     l_cairo_fill},
    {"fill_preserve",                            l_cairo_fill_preserve},
    {"copy_page",                                l_cairo_copy_page},
    {"show_page",                                l_cairo_show_page},
    {"in_stroke",                                l_cairo_in_stroke},
    {"in_fill",                                  l_cairo_in_fill},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
    {"in_clip",                                  l_cairo_in_clip},
#endif
    {"stroke_extents",                           l_cairo_stroke_extents},
    {"fill_extents",                             l_cairo_fill_extents},
    {"reset_clip",                               l_cairo_reset_clip},
    {"clip",                                     l_cairo_clip},
    {"clip_preserve",                            l_cairo_clip_preserve},
    {"clip_extents",                             l_cairo_clip_extents},
    {"copy_clip_rectangle_list",                 l_cairo_copy_clip_rectangle_list},
    {"rectangle_list_destroy",                   l_cairo_rectangle_list_destroy},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    {"glyph_allocate",                           l_cairo_glyph_allocate},
    {"glyph_free",                               l_cairo_glyph_free},
    {"text_cluster_allocate",                    l_cairo_text_cluster_allocate},
    {"text_cluster_free",                        l_cairo_text_cluster_free},
#endif
    {"select_font_face",                         l_cairo_select_font_face},
    {"set_font_size",                            l_cairo_set_font_size},
    {"set_font_matrix",                          l_cairo_set_font_matrix},
    {"get_font_matrix",                          l_cairo_get_font_matrix},
    {"set_font_options",                         l_cairo_set_font_options},
    {"get_font_options",                         l_cairo_get_font_options},
    {"set_font_face",                            l_cairo_set_font_face},
    {"get_font_face",                            l_cairo_get_font_face},
    {"set_scaled_font",                          l_cairo_set_scaled_font},
    {"get_scaled_font",                          l_cairo_get_scaled_font},
    {"show_text",                                l_cairo_show_text},
    {"show_glyphs",                              l_cairo_show_glyphs},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    {"show_text_glyphs",                         l_cairo_show_text_glyphs},
#endif
    {"text_path",                                l_cairo_text_path},
    {"glyph_path",                               l_cairo_glyph_path},
    {"text_extents",                             l_cairo_text_extents},
    {"glyph_extents",                            l_cairo_glyph_extents},
    {"font_extents",                             l_cairo_font_extents},
    {"get_operator",                             l_cairo_get_operator},
    {"get_source",                               l_cairo_get_source},
    {"get_tolerance",                            l_cairo_get_tolerance},
    {"get_antialias",                            l_cairo_get_antialias},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    {"has_current_point",                        l_cairo_has_current_point},
#endif
    {"get_current_point",                        l_cairo_get_current_point},
    {"get_fill_rule",                            l_cairo_get_fill_rule},
    {"get_line_width",                           l_cairo_get_line_width},
    {"get_line_cap",                             l_cairo_get_line_cap},
    {"get_line_join",                            l_cairo_get_line_join},
    {"get_miter_limit",                          l_cairo_get_miter_limit},
    {"get_dash_count",                           l_cairo_get_dash_count},
    {"get_dash",                                 l_cairo_get_dash},
    {"get_matrix",                               l_cairo_get_matrix},
    {"get_target",                               l_cairo_get_target},
    {"get_group_target",                         l_cairo_get_group_target},
    {"copy_path",                                l_cairo_copy_path},
    {"copy_path_flat",                           l_cairo_copy_path_flat},
    {"append_path",                              l_cairo_append_path},
    {"path_destroy",                             l_cairo_path_destroy},
    {"status",                                   l_cairo_status},
    {"status_to_string",                         l_cairo_status_to_string},
  //{"debug_reset_static_data",                  l_cairo_debug_reset_static_data},

    {NULL, NULL}
};


static int luaopen_lcontext (lua_State *L)
{
    luaL_newmetatable(L, LUACAIRO ".Context.mt");          // mt
    lua_pushvalue(L, -1);                                  // mt mt
    lua_setfield(L, -2, "__index");                        // mt       (mt.__index = mt)
    luaL_register(L, NULL, Context_lib_m);                 // mt
    luaL_register(L, LUACAIRO ".Context", Context_lib_f);  // mt ct
    lua_newtable(L);                                       // mt ct t
    lua_pushstring(L, "__call");                           // mt ct t "__call"
    lua_pushcfunction(L, new_Context);                     // mt ct t "__call" fun
    lua_settable(L, -3);                                   // mt ct t  (t.__call = fun)
    lua_setmetatable(L, -2);                               // mt ct    (ct.meta = t)
    lua_pop(L, 1);                                         // mt

    lua_pop(L, 1);
    return 0;
}

#endif  /* LUACAIRO_HAS_OO_INTERFACE */



static int luaopen_lcairo_ud_context(lua_State* L)
{
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lcontext (L);

#endif /* LUACAIRO_HAS_OO_INTERFACE */
    return 0;
}
