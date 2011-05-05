/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



// cairo_public int
// cairo_version (void);
static int l_cairo_version(lua_State* L)
{
    int v = cairo_version();
    lua_pushnumber(L, v);
    return 1;
}


// cairo_public const char*
// cairo_version_string (void);
static int l_cairo_version_string(lua_State* L)
{
    const char* v = cairo_version_string();
    lua_pushstring(L, v);
    return 1;
}


/* Functions for manipulating state objects */

// cairo_public cairo_t *
// cairo_create (cairo_surface_t *target);
static int l_cairo_create(lua_State* L)
{
    cairo_surface_t *target = get_cairo_surface_t (L, 1);
    cairo_t *v = cairo_create(target);
    lua_pushlightuserdata(L, v);
    return 1;
}


//FIXME should not be exposed to the user of the language binding,
//FIXME but rather used to implement memory management within the language binding
// cairo_public cairo_t *
// cairo_reference (cairo_t *cr);
static int l_cairo_reference(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_t *v = cairo_reference(cr);
    lua_pushlightuserdata(L, v);
    return 1;
}


//FIXME should not be exposed to the user of the language binding,
//FIXME but rather used to implement memory management within the language binding
// cairo_public void
// cairo_destroy (cairo_t *cr);
static int l_cairo_destroy(lua_State* L)
{
    cairo_t *cr = (cairo_t *) check_lightuserdata(L, 1);
    cairo_destroy(cr);
    return 0;
}


//FIXME should not be exposed to the user of the language binding,
//FIXME but rather used to implement memory management within the language binding
// cairo_public unsigned int
// cairo_get_reference_count (cairo_t *cr);
static int l_cairo_get_reference_count(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    unsigned int v = cairo_get_reference_count(cr);
    lua_pushnumber(L, v);
    return 1;
}


// cairo_public void *
// cairo_get_user_data (cairo_t *cr,
//              const cairo_user_data_key_t *key);
static int l_cairo_get_user_data(lua_State* L)
{
    //cairo_t *cr = get_cairo_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public cairo_status_t
// cairo_set_user_data (cairo_t *cr,
//              const cairo_user_data_key_t *key,
//              void *user_data,
//              cairo_destroy_func_t destroy);
static int l_cairo_set_user_data(lua_State* L)
{
    //cairo_t *cr = get_cairo_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public void
// cairo_save (cairo_t *cr);
static int l_cairo_save(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_save(cr);
    return 0;
}


// cairo_public void
// cairo_restore (cairo_t *cr);
static int l_cairo_restore(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_restore(cr);
    return 0;
}


// cairo_public void
// cairo_push_group (cairo_t *cr);
static int l_cairo_push_group(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_push_group(cr);
    return 0;
}


// cairo_public void
// cairo_push_group_with_content (cairo_t *cr,
//          cairo_content_t content);
static int l_cairo_push_group_with_content(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_content_t content = (cairo_content_t) luaL_checkinteger(L, 2);
    cairo_push_group_with_content(cr, content);
    return 0;
}


// cairo_public cairo_pattern_t *
// cairo_pop_group (cairo_t *cr);
static int l_cairo_pop_group(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_pattern_t *v = cairo_pop_group(cr);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public void
// cairo_pop_group_to_source (cairo_t *cr);
static int l_cairo_pop_group_to_source(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_pop_group_to_source(cr);
    return 0;
}


/* Modify state  */

// cairo_public void
// cairo_set_operator (cairo_t *cr,
//              cairo_operator_t op);
static int l_cairo_set_operator(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_operator_t op = (cairo_operator_t) luaL_checkinteger(L, 2);
    cairo_set_operator(cr, op);
    return 0;
}


// cairo_public void
// cairo_set_source (cairo_t *cr,
//              cairo_pattern_t *source);
static int l_cairo_set_source(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_pattern_t *source = get_cairo_pattern_t(L, 2);
    cairo_set_source(cr, source);
    return 0;
}


// cairo_public void
// cairo_set_source_rgb (cairo_t *cr,
//              double red, double green, double blue);
static int l_cairo_set_source_rgb(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double red = luaL_checknumber(L, 2);
    double green = luaL_checknumber(L, 3);
    double blue = luaL_checknumber(L, 4);
    cairo_set_source_rgb(cr, red, green, blue);
    return 0;
}


// cairo_public void
// cairo_set_source_rgba (cairo_t *cr,
//                double red, double green, double blue,
//                double alpha);
static int l_cairo_set_source_rgba(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double red = luaL_checknumber(L, 2);
    double green = luaL_checknumber(L, 3);
    double blue = luaL_checknumber(L, 4);
    double alpha = luaL_checknumber(L, 5);
    cairo_set_source_rgba(cr, red, green, blue, alpha);
    return 0;
}


// cairo_public void
// cairo_set_source_surface (cairo_t *cr,
//               cairo_surface_t *surface,
//               double x,
//               double y);
static int l_cairo_set_source_surface(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_surface_t *surface = get_cairo_surface_t (L, 2);
    double x = luaL_checknumber(L, 3);
    double y = luaL_checknumber(L, 4);
    cairo_set_source_surface(cr, surface, x, y);
    return 0;
}


// cairo_public void
// cairo_set_tolerance (cairo_t *cr,
//              double tolerance);
static int l_cairo_set_tolerance(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double tolerance = luaL_checknumber(L, 2);
    cairo_set_tolerance(cr, tolerance);
    return 0;
}


// cairo_public void
// cairo_set_antialias (cairo_t *cr,
//              cairo_antialias_t antialias);
static int l_cairo_set_antialias(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_antialias_t antialias = (cairo_antialias_t) luaL_checkinteger(L, 2);
    cairo_set_antialias(cr, antialias);
    return 0;
}


// cairo_public void
// cairo_set_fill_rule (cairo_t *cr,
//              cairo_fill_rule_t fill_rule);
static int l_cairo_set_fill_rule(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_fill_rule_t fill_rule = (cairo_fill_rule_t) luaL_checkinteger(L, 2);
    cairo_set_fill_rule(cr, fill_rule);
    return 0;
}


// cairo_public void
// cairo_set_line_width (cairo_t *cr,
//              double width);
static int l_cairo_set_line_width(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double width = luaL_checknumber(L, 2);
    cairo_set_line_width(cr, width);
    return 0;
}


// cairo_public void
// cairo_set_line_cap (cairo_t *cr,
//              cairo_line_cap_t line_cap);
static int l_cairo_set_line_cap(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_line_cap_t line_cap = (cairo_line_cap_t) luaL_checkinteger(L, 2);
    cairo_set_line_cap(cr, line_cap);
    return 0;
}


// cairo_public void
// cairo_set_line_join (cairo_t *cr,
//              cairo_line_join_t line_join);
static int l_cairo_set_line_join(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_line_join_t line_join = (cairo_line_join_t) luaL_checkinteger(L, 2);
    cairo_set_line_join(cr, line_join);
    return 0;
}


// cairo_public void
// cairo_set_dash (cairo_t *cr,
//         const double *dashes,
//         int num_dashes,
//         double offset);
static int l_cairo_set_dash(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);

    switch (lua_type(L, 2)) {
    case LUA_TNUMBER: {
            double dash = luaL_checknumber(L, 2);
            int	num_dashes = 1; //luaL_checkinteger(L, 3);
            double offset = luaL_checknumber(L, 4);
            cairo_set_dash(cr, &dash, num_dashes, offset);
        }
        break;
    case LUA_TTABLE: { // Borrowed from Chris Osgood's binding
            int num_dashes = (int)lua_objlen(L, 2);

            double *dashes = malloc(num_dashes * sizeof(double));
            if (dashes == NULL) luaL_error(L, "memory error");

            int i;
            for (i = 0; i < num_dashes; i++) {
                lua_rawgeti(L, 2, i+1);
                dashes[i] = lua_tonumber(L, -1);
                lua_pop(L, 1);
            }

            double offset = luaL_checknumber(L, 4);
            cairo_set_dash(cr, dashes, num_dashes, offset);
            free(dashes);
        }
        break;
    default:
        luaL_error(L, "expected a number or number array(table)!");
    }
    return 0;
}


// cairo_public void
// cairo_set_miter_limit (cairo_t *cr,
//              double limit);
static int l_cairo_set_miter_limit(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double limit = luaL_checknumber(L, 2);
    cairo_set_miter_limit(cr, limit);
    return 0;
}


// cairo_public void
// cairo_translate (cairo_t *cr,
//              double tx, double ty);
static int l_cairo_translate(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double tx = luaL_checknumber(L, 2);
    double ty = luaL_checknumber(L, 3);
    cairo_translate(cr, tx, ty);
    return 0;
}


// cairo_public void
// cairo_scale (cairo_t *cr,
//              double sx, double sy);
static int l_cairo_scale(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double sx = luaL_checknumber(L, 2);
    double sy = luaL_checknumber(L, 3);
    cairo_scale(cr, sx, sy);
    return 0;
}


// cairo_public void
// cairo_rotate (cairo_t *cr,
//              double angle);
static int l_cairo_rotate(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double angle = luaL_checknumber(L, 2);
    cairo_rotate(cr, angle);
    return 0;
}


// cairo_public void
// cairo_transform (cairo_t *cr,
//          const cairo_matrix_t *matrix);
static int l_cairo_transform(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    const cairo_matrix_t *matrix = get_cairo_matrix_t (L, 2);
    cairo_transform (cr, matrix);
    return 0;
}


// cairo_public void
// cairo_set_matrix (cairo_t *cr,
//           const cairo_matrix_t *matrix);
static int l_cairo_set_matrix(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    const cairo_matrix_t *matrix = get_cairo_matrix_t (L, 2);
    cairo_set_matrix (cr, matrix);
    return 0;
}


// cairo_public void
// cairo_identity_matrix (cairo_t *cr);
static int l_cairo_identity_matrix(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_identity_matrix(cr);
    return 0;
}


// cairo_public void
// cairo_user_to_device (cairo_t *cr,
//              double *x, double *y);
static int l_cairo_user_to_device(lua_State* L)
{
    /// WARN:different usage
    cairo_t *cr = get_cairo_t (L, 1);
    double x = luaL_checknumber(L, 2);
    double y = luaL_checknumber(L, 3);
    cairo_user_to_device(cr, &x, &y);
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    return 2;
}


// cairo_public void
// cairo_user_to_device_distance (cairo_t *cr,
//              double *dx, double *dy);
static int l_cairo_user_to_device_distance(lua_State* L)
{
    /// WARN:different usage
    cairo_t *cr = get_cairo_t (L, 1);
    double dx = luaL_checknumber(L, 2);
    double dy = luaL_checknumber(L, 3);
    cairo_user_to_device_distance(cr, &dx, &dy);
    lua_pushnumber(L, dx);
    lua_pushnumber(L, dy);
    return 2;
}


// cairo_public void
// cairo_device_to_user (cairo_t *cr,
//              double *x, double *y);
static int l_cairo_device_to_user(lua_State* L)
{
    /// WARN:different usage
    cairo_t *cr = get_cairo_t (L, 1);
    double x = luaL_checknumber(L, 2);
    double y = luaL_checknumber(L, 3);
    cairo_device_to_user(cr, &x, &y);
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    return 2;
}


// cairo_public void
// cairo_device_to_user_distance (cairo_t *cr,
//              double *dx, double *dy);
static int l_cairo_device_to_user_distance(lua_State* L)
{
    /// WARN:different usage
    cairo_t *cr = get_cairo_t (L, 1);
    double dx = luaL_checknumber(L, 2);
    double dy = luaL_checknumber(L, 3);
    cairo_device_to_user_distance(cr, &dx, &dy);
    lua_pushnumber(L, dx);
    lua_pushnumber(L, dy);
    return 2;
}




/* Painting functions */

// cairo_public void
// cairo_paint (cairo_t *cr);
static int l_cairo_paint(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_paint(cr);
    return 0;
}


// cairo_public void
// cairo_paint_with_alpha (cairo_t *cr,
//             double   alpha);
static int l_cairo_paint_with_alpha(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double alpha = luaL_checknumber(L, 2);
    cairo_paint_with_alpha(cr, alpha);
    return 0;
}


// cairo_public void
// cairo_mask (cairo_t *cr,
//         cairo_pattern_t *pattern);
static int l_cairo_mask(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_pattern_t *pattern = get_cairo_pattern_t (L, 2);
    cairo_mask(cr, pattern);
    return 0;
}


// cairo_public void
// cairo_mask_surface (cairo_t *cr,
//             cairo_surface_t *surface,
//             double           surface_x,
//             double           surface_y);
static int l_cairo_mask_surface(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_surface_t *surface = get_cairo_surface_t (L, 2);
    double surface_x = luaL_checknumber(L, 3);
    double surface_y = luaL_checknumber(L, 4);
    cairo_mask_surface(cr, surface, surface_x, surface_y);
    return 0;
}


// cairo_public void
// cairo_stroke (cairo_t *cr);
static int l_cairo_stroke(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_stroke(cr);
    return 0;
}


// cairo_public void
// cairo_stroke_preserve (cairo_t *cr);
static int l_cairo_stroke_preserve(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_stroke_preserve(cr);
    return 0;
}


// cairo_public void
// cairo_fill (cairo_t *cr);
static int l_cairo_fill(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_fill(cr);
    return 0;
}


// cairo_public void
// cairo_fill_preserve (cairo_t *cr);
static int l_cairo_fill_preserve(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_fill_preserve(cr);
    return 0;
}


// cairo_public void
// cairo_copy_page (cairo_t *cr);
static int l_cairo_copy_page(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_copy_page(cr);
    return 0;
}


// cairo_public void
// cairo_show_page (cairo_t *cr);
static int l_cairo_show_page(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_show_page(cr);
    return 0;
}


/* Insideness testing */

// cairo_public cairo_bool_t
// cairo_in_stroke (cairo_t *cr,
//              double x, double y);
static int l_cairo_in_stroke(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double x = luaL_checknumber(L, 2);
    double y = luaL_checknumber(L, 3);
    cairo_bool_t v = cairo_in_stroke(cr, x, y);
    lua_pushboolean(L, v);
    return 1;
}


// cairo_public cairo_bool_t
// cairo_in_fill (cairo_t *cr,
//              double x, double y);
static int l_cairo_in_fill(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double x = luaL_checknumber(L, 2);
    double y = luaL_checknumber(L, 3);
    cairo_bool_t v = cairo_in_fill(cr, x, y);
    lua_pushboolean(L, v);
    return 1;
}


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
// cairo_public cairo_bool_t
// cairo_in_clip (cairo_t *cr, double x, double y);
static int l_cairo_in_clip(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double x = luaL_checknumber(L, 2);
    double y = luaL_checknumber(L, 3);
    cairo_bool_t v = cairo_in_clip(cr, x, y);
    lua_pushboolean(L, v);
    return 1;
}
#endif


/* Rectangular extents */

// cairo_public void
// cairo_stroke_extents (cairo_t *cr,
//               double *x1, double *y1,
//               double *x2, double *y2);
static int l_cairo_stroke_extents(lua_State* L)
{
    /// WARN:different usage
    cairo_t *cr = get_cairo_t (L, 1);
    double x1 = luaL_optnumber(L, 2, 0.0);
    double y1 = luaL_optnumber(L, 3, 0.0);
    double x2 = luaL_optnumber(L, 4, 0.0);
    double y2 = luaL_optnumber(L, 5, 0.0);
    cairo_stroke_extents(cr, &x1, &y1, &x2, &y2);
    lua_pushnumber(L, x1);
    lua_pushnumber(L, y1);
    lua_pushnumber(L, x2);
    lua_pushnumber(L, y2);
    return 4;
}


// cairo_public void
// cairo_fill_extents (cairo_t *cr,
//             double *x1, double *y1,
//             double *x2, double *y2);
static int l_cairo_fill_extents(lua_State* L)
{
    /// WARN:different usage
    cairo_t *cr = get_cairo_t (L, 1);
    double x1 = luaL_optnumber(L, 2, 0.0);
    double y1 = luaL_optnumber(L, 3, 0.0);
    double x2 = luaL_optnumber(L, 4, 0.0);
    double y2 = luaL_optnumber(L, 5, 0.0);
    cairo_fill_extents(cr, &x1, &y1, &x2, &y2);
    lua_pushnumber(L, x1);
    lua_pushnumber(L, y1);
    lua_pushnumber(L, x2);
    lua_pushnumber(L, y2);
    return 4;
}


/* Clipping */

// cairo_public void
// cairo_reset_clip (cairo_t *cr);
static int l_cairo_reset_clip(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_reset_clip(cr);
    return 0;
}


// cairo_public void
// cairo_clip (cairo_t *cr);
static int l_cairo_clip(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_clip(cr);
    return 0;
}


// cairo_public void
// cairo_clip_preserve (cairo_t *cr);
static int l_cairo_clip_preserve(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_clip_preserve(cr);
    return 0;
}


// cairo_public void
// cairo_clip_extents (cairo_t *cr,
//             double *x1, double *y1,
//             double *x2, double *y2);
static int l_cairo_clip_extents(lua_State* L)
{
    /// WARN:different usage
    cairo_t *cr = get_cairo_t (L, 1);
    double x1 = luaL_optnumber(L, 2, 0.0);
    double y1 = luaL_optnumber(L, 3, 0.0);
    double x2 = luaL_optnumber(L, 4, 0.0);
    double y2 = luaL_optnumber(L, 5, 0.0);
    cairo_clip_extents(cr, &x1, &y1, &x2, &y2);
    lua_pushnumber(L, x1);
    lua_pushnumber(L, y1);
    lua_pushnumber(L, x2);
    lua_pushnumber(L, y2);
    return 4;
}


// cairo_public cairo_rectangle_list_t *
// cairo_copy_clip_rectangle_list (cairo_t *cr);
static int l_cairo_copy_clip_rectangle_list(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_rectangle_list_t *v = cairo_copy_clip_rectangle_list(cr);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public void
// cairo_rectangle_list_destroy (cairo_rectangle_list_t *rectangle_list);
static int l_cairo_rectangle_list_destroy(lua_State* L)
{
    cairo_rectangle_list_t *rectangle_list = (cairo_rectangle_list_t *) check_lightuserdata(L, 1);
    cairo_rectangle_list_destroy(rectangle_list);
    return 0;
}


/* Font/Text functions */

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
// cairo_public cairo_glyph_t *
// cairo_glyph_allocate (int num_glyphs);
static int l_cairo_glyph_allocate(lua_State* L)
{
    //remove_Context(L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}
#endif


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
// cairo_public void
// cairo_glyph_free (cairo_glyph_t *glyphs);
static int l_cairo_glyph_free(lua_State* L)
{
    //remove_Context(L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}
#endif


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
// cairo_public cairo_text_cluster_t *
// cairo_text_cluster_allocate (int num_clusters);
static int l_cairo_text_cluster_allocate(lua_State* L)
{
    //remove_Context(L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}
#endif


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
// cairo_public void
// cairo_text_cluster_free (cairo_text_cluster_t *clusters);
static int l_cairo_text_cluster_free(lua_State* L)
{
    //remove_Context(L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}
#endif



/* This interface is for dealing with text as text, not caring about the
   font object inside the the cairo_t. */

// cairo_public void
// cairo_select_font_face (cairo_t *cr,
//             const char           *family,
//             cairo_font_slant_t   slant,
//             cairo_font_weight_t  weight);
static int l_cairo_select_font_face(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    const char *family = luaL_checkstring(L, 2);
    cairo_font_slant_t slant = (cairo_font_slant_t) luaL_checkinteger(L, 3);
    cairo_font_weight_t weight = (cairo_font_weight_t) luaL_checkinteger(L, 4);
    cairo_select_font_face(cr, family, slant, weight);
    return 0;
}


// cairo_public void
// cairo_set_font_size (cairo_t *cr, double size);
static int l_cairo_set_font_size(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double size = luaL_checknumber(L, 2);
    cairo_set_font_size(cr, size);
    return 0;
}


// cairo_public void
// cairo_set_font_matrix (cairo_t *cr,
//                const cairo_matrix_t *matrix);
static int l_cairo_set_font_matrix(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    const cairo_matrix_t *matrix = get_cairo_matrix_t (L, 2);
    cairo_set_font_matrix (cr, matrix);
    return 0;
}


// cairo_public void
// cairo_get_font_matrix (cairo_t *cr,
//                cairo_matrix_t *matrix);
static int l_cairo_get_font_matrix(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_matrix_t *matrix = get_cairo_matrix_t (L, 2);
    cairo_get_font_matrix (cr, matrix);
    return 0;
}


// cairo_public void
// cairo_set_font_options (cairo_t *cr,
//             const cairo_font_options_t *options);
static int l_cairo_set_font_options(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    const cairo_font_options_t *options = get_cairo_font_options_t (L, 2);
    cairo_set_font_options(cr, options);
    return 0;
}


// cairo_public void
// cairo_get_font_options (cairo_t *cr,
//             cairo_font_options_t *options);
static int l_cairo_get_font_options(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_font_options_t *options = get_cairo_font_options_t (L, 2);
    cairo_get_font_options (cr, options);
    return 0;
}


// cairo_public void
// cairo_set_font_face (cairo_t *cr,
//              cairo_font_face_t *font_face);
static int l_cairo_set_font_face(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_font_face_t *font_face = get_cairo_font_face_t (L, 2);
    cairo_set_font_face (cr, font_face);
    return 0;
}


// cairo_public cairo_font_face_t *
// cairo_get_font_face (cairo_t *cr);
static int l_cairo_get_font_face(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_font_face_t *v = cairo_get_font_face (cr);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public void
// cairo_set_scaled_font (cairo_t *cr,
//                const cairo_scaled_font_t *scaled_font);
static int l_cairo_set_scaled_font(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    const cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 2);
    cairo_set_scaled_font (cr, scaled_font);
    return 0;
}


// cairo_public cairo_scaled_font_t *
// cairo_get_scaled_font (cairo_t *cr);
static int l_cairo_get_scaled_font(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_scaled_font_t *v = cairo_get_scaled_font (cr);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public void
// cairo_show_text (cairo_t *cr,
//              const char *utf8);
static int l_cairo_show_text(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    const char *utf8 = luaL_checkstring(L, 2);
    cairo_show_text (cr, utf8);
    return 0;
}


// cairo_public void
// cairo_show_glyphs (cairo_t *cr,
//              const cairo_glyph_t *glyphs,
//              int num_glyphs);
static int l_cairo_show_glyphs(lua_State* L)
{
    //cairo_t *cr = get_cairo_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
// cairo_public void
// cairo_show_text_glyphs (cairo_t *cr,
//             const char *utf8,
//             int utf8_len,
//             const cairo_glyph_t *glyphs,
//             int num_glyphs,
//             const cairo_text_cluster_t *clusters,
//             int num_clusters,
//             cairo_text_cluster_flags_t cluster_flags);
static int l_cairo_show_text_glyphs(lua_State* L)
{
    //cairo_t *cr = get_cairo_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}
#endif


// cairo_public void
// cairo_text_path  (cairo_t *cr,
//              const char *utf8);
static int l_cairo_text_path(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    const char *utf8 = luaL_checkstring(L, 2);
    cairo_text_path (cr, utf8);
    return 0;
}


// cairo_public void
// cairo_glyph_path (cairo_t *cr,
//              const cairo_glyph_t *glyphs,
//              int num_glyphs);
static int l_cairo_glyph_path(lua_State* L)
{
    //cairo_t *cr = get_cairo_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public void
// cairo_text_extents (cairo_t *cr,
//             const char *utf8,
//             cairo_text_extents_t *extents);
static int l_cairo_text_extents(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    const char *utf8 = luaL_checkstring(L, 2);
    cairo_text_extents_t *extents = get_cairo_text_extents_t (L, 3);
    cairo_text_extents (cr, utf8, extents);
    return 0;
}


// cairo_public void
// cairo_glyph_extents (cairo_t *cr,
//              const cairo_glyph_t *glyphs,
//              int num_glyphs,
//              cairo_text_extents_t *extents);
static int l_cairo_glyph_extents(lua_State* L)
{
    //cairo_t *cr = get_cairo_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public void
// cairo_font_extents (cairo_t *cr,
//             cairo_font_extents_t *extents);
static int l_cairo_font_extents(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_font_extents_t *extents = get_cairo_font_extents_t (L, 2);
    cairo_font_extents (cr, extents);
    return 0;
}




/* Query functions */

// cairo_public cairo_operator_t
// cairo_get_operator (cairo_t *cr);
static int l_cairo_get_operator(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_operator_t v = cairo_get_operator(cr);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public cairo_pattern_t *
// cairo_get_source (cairo_t *cr);
static int l_cairo_get_source(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_pattern_t *v = cairo_get_source(cr);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public double
// cairo_get_tolerance (cairo_t *cr);
static int l_cairo_get_tolerance(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double v = cairo_get_tolerance(cr);
    lua_pushnumber(L, v);
    return 1;
}


// cairo_public cairo_antialias_t
// cairo_get_antialias (cairo_t *cr);
static int l_cairo_get_antialias(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_antialias_t v = cairo_get_antialias(cr);
    lua_pushinteger(L, v);
    return 1;
}


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
// cairo_public cairo_bool_t
// cairo_has_current_point (cairo_t *cr);
static int l_cairo_has_current_point(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_bool_t v = cairo_has_current_point (cr);
    lua_pushboolean(L, v);
    return 1;
}
#endif


// cairo_public void
// cairo_get_current_point (cairo_t *cr,
//              double *x, double *y);
static int l_cairo_get_current_point(lua_State* L)
{
    /// WARN:different usage
    cairo_t *cr = get_cairo_t (L, 1);
    double x = luaL_optnumber(L, 2, 0.0);
    double y = luaL_optnumber(L, 3, 0.0);
    cairo_get_current_point (cr, &x, &y);
    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    return 2;
}


// cairo_public cairo_fill_rule_t
// cairo_get_fill_rule (cairo_t *cr);
static int l_cairo_get_fill_rule(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_fill_rule_t v = cairo_get_fill_rule(cr);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public double
// cairo_get_line_width (cairo_t *cr);
static int l_cairo_get_line_width(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double v = cairo_get_line_width(cr);
    lua_pushnumber(L, v);
    return 1;
}


// cairo_public cairo_line_cap_t
// cairo_get_line_cap (cairo_t *cr);
static int l_cairo_get_line_cap(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_line_cap_t v = cairo_get_line_cap(cr);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public cairo_line_join_t
// cairo_get_line_join (cairo_t *cr);
static int l_cairo_get_line_join(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_line_join_t v = cairo_get_line_join(cr);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public double
// cairo_get_miter_limit (cairo_t *cr);
static int l_cairo_get_miter_limit(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double v = cairo_get_miter_limit(cr);
    lua_pushnumber(L, v);
    return 1;
}


// cairo_public int
// cairo_get_dash_count (cairo_t *cr);
static int l_cairo_get_dash_count(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    int v = cairo_get_dash_count(cr);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public void
// cairo_get_dash (cairo_t *cr,
//              double *dashes, double *offset);
static int l_cairo_get_dash(lua_State* L)
{
    //cairo_t *cr = get_cairo_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public void
// cairo_get_matrix (cairo_t *cr,
//              cairo_matrix_t *matrix);
static int l_cairo_get_matrix(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_matrix_t *matrix = get_cairo_matrix_t (L, 2);
    cairo_get_matrix (cr, matrix);
    return 0;
}


// cairo_public cairo_surface_t *
// cairo_get_target (cairo_t *cr);
static int l_cairo_get_target(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_surface_t *v = cairo_get_target(cr);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public cairo_surface_t *
// cairo_get_group_target (cairo_t *cr);
static int l_cairo_get_group_target(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_surface_t *v = cairo_get_group_target(cr);
    lua_pushlightuserdata(L, v);
    return 1;
}



/* Error status queries */

// cairo_public cairo_status_t
// cairo_status (cairo_t *cr);
static int l_cairo_status(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_status_t v = cairo_status(cr);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public const char *
// cairo_status_to_string (cairo_status_t status);
static int l_cairo_status_to_string(lua_State* L)
{
    remove_Context(L, 1); // if called via Context userdata
    cairo_status_t status = (cairo_status_t) luaL_checkinteger(L, 1);
    const char *v = cairo_status_to_string(status);
    lua_pushstring(L, v);
    return 1;
}


/* Functions to be used while debugging (not intended for use in production code) */
// cairo_public void
// cairo_debug_reset_static_data (void);
static int l_cairo_debug_reset_static_data(lua_State* L)
{
    //remove_Context(L, 1); // if called via Context userdata
    cairo_debug_reset_static_data ();
    return 0;
}



static const struct luaL_Reg context_f [] = {
    // Cairo API
    {"version",                                  l_cairo_version},
    {"version_string",                           l_cairo_version_string},
    {"create",                                   l_cairo_create},
    {"reference",                                l_cairo_reference},
    {"destroy",                                  l_cairo_destroy},
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
    {"status",                                   l_cairo_status},
    {"status_to_string",                         l_cairo_status_to_string},
    {"debug_reset_static_data",                  l_cairo_debug_reset_static_data},

	{NULL, NULL} /* sentinel */
};



static int luaopen_lcairo_context(lua_State* L)
{
    // register functions
    luaL_register(L, LUACAIRO, context_f);

    return 0;
}
