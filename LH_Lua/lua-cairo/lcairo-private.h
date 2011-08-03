#ifndef LUA_CAIRO_PRIVATE_H
#define LUA_CAIRO_PRIVATE_H


#if CAIRO_HAS_SVG_SURFACE
    #include <cairo-svg.h>
#endif
#if CAIRO_HAS_PDF_SURFACE
    #include <cairo-pdf.h>
#endif
#if CAIRO_HAS_PS_SURFACE
    #include <cairo-ps.h>
#endif
#if CAIRO_HAS_XCB_SURFACE
    #include <cairo-xcb.h>
#endif
#if CAIRO_HAS_XLIB_SURFACE
    #include <cairo-xlib.h>
#endif
#if CAIRO_HAS_WIN32_SURFACE
    #include <cairo-win32.h>
#endif
#if CAIRO_HAS_BEOS_SURFACE
    #include <cairo-beos.h>
#endif
#if CAIRO_HAS_DIRECTFB_SURFACE
    #include <cairo-directfb.h>
#endif
#if CAIRO_HAS_OS2_SURFACE
    #include <cairo-os2.h>
#endif
#if CAIRO_HAS_GLITZ_SURFACE
    #include <cairo-glitz.h>
#endif
#if CAIRO_HAS_QUARTZ_SURFACE
    #include <cairo-quartz.h>
#endif
#if CAIRO_HAS_FT_FONT
    #include <cairo-ft.h>
#endif
#if CAIRO_HAS_XML_SURFACE
    #include <cairo-xml.h>
#endif
#if CAIRO_HAS_SCRIPT_SURFACE
    #include <cairo-script.h>
#endif


///--------------------------------------------------------------------------
/// Function Declarations and Status
/// ++: ready
/// ww: ready, different usage
/// ..: partial
/// NI: not implemented
///--------------------------------------------------------------------------
/* ++ */ static int   l_cairo_version (lua_State* L);
/* ++ */ static int   l_cairo_version_string (lua_State* L);
/* ++ */ static int   l_cairo_create (lua_State* L);
/* ++ */ static int   l_cairo_reference (lua_State* L);
/* ++ */ static int   l_cairo_destroy (lua_State* L);
/* ++ */ static int   l_cairo_get_reference_count (lua_State* L);
/* NI */ static int   l_cairo_get_user_data (lua_State* L);
/* NI */ static int   l_cairo_set_user_data (lua_State* L);
/* ++ */ static int   l_cairo_save (lua_State* L);
/* ++ */ static int   l_cairo_restore (lua_State* L);
/* ++ */ static int   l_cairo_push_group (lua_State* L);
/* ++ */ static int   l_cairo_push_group_with_content (lua_State* L);
/* ++ */ static int   l_cairo_pop_group (lua_State* L);
/* ++ */ static int   l_cairo_pop_group_to_source (lua_State* L);
/* ++ */ static int   l_cairo_set_operator (lua_State* L);
/* ++ */ static int   l_cairo_set_source (lua_State* L);
/* ++ */ static int   l_cairo_set_source_rgb (lua_State* L);
/* ++ */ static int   l_cairo_set_source_rgba (lua_State* L);
/* ++ */ static int   l_cairo_set_source_surface (lua_State* L);
/* ++ */ static int   l_cairo_set_tolerance (lua_State* L);
/* ++ */ static int   l_cairo_set_antialias (lua_State* L);
/* ++ */ static int   l_cairo_set_fill_rule (lua_State* L);
/* ++ */ static int   l_cairo_set_line_width (lua_State* L);
/* ++ */ static int   l_cairo_set_line_cap (lua_State* L);
/* ++ */ static int   l_cairo_set_line_join (lua_State* L);
/* ++ */ static int   l_cairo_set_dash (lua_State* L);
/* ++ */ static int   l_cairo_set_miter_limit (lua_State* L);
/* ++ */ static int   l_cairo_translate (lua_State* L);
/* ++ */ static int   l_cairo_scale (lua_State* L);
/* ++ */ static int   l_cairo_rotate (lua_State* L);
/* ++ */ static int   l_cairo_transform (lua_State* L);
/* ++ */ static int   l_cairo_set_matrix (lua_State* L);
/* ++ */ static int   l_cairo_identity_matrix (lua_State* L);
/* ww */ static int   l_cairo_user_to_device (lua_State* L);
/* ww */ static int   l_cairo_user_to_device_distance (lua_State* L);
/* ww */ static int   l_cairo_device_to_user (lua_State* L);
/* ww */ static int   l_cairo_device_to_user_distance (lua_State* L);
/* ++ */ static int   l_cairo_new_path (lua_State* L);
/* ++ */ static int   l_cairo_move_to (lua_State* L);
/* ++ */ static int   l_cairo_new_sub_path (lua_State* L);
/* ++ */ static int   l_cairo_line_to (lua_State* L);
/* ++ */ static int   l_cairo_curve_to (lua_State* L);
/* ++ */ static int   l_cairo_arc (lua_State* L);
/* ++ */ static int   l_cairo_arc_negative (lua_State* L);
/* ++ */ static int   l_cairo_rel_move_to (lua_State* L);
/* ++ */ static int   l_cairo_rel_line_to (lua_State* L);
/* ++ */ static int   l_cairo_rel_curve_to (lua_State* L);
/* ++ */ static int   l_cairo_rectangle (lua_State* L);
/* ++ */ static int   l_cairo_close_path (lua_State* L);
/* ++ */ static int   l_cairo_paint (lua_State* L);
/* ++ */ static int   l_cairo_paint_with_alpha (lua_State* L);
/* ++ */ static int   l_cairo_mask (lua_State* L);
/* ++ */ static int   l_cairo_mask_surface (lua_State* L);
/* ++ */ static int   l_cairo_stroke (lua_State* L);
/* ++ */ static int   l_cairo_stroke_preserve (lua_State* L);
/* ++ */ static int   l_cairo_fill (lua_State* L);
/* ++ */ static int   l_cairo_fill_preserve (lua_State* L);
/* ++ */ static int   l_cairo_copy_page (lua_State* L);
/* ++ */ static int   l_cairo_show_page (lua_State* L);
/* ++ */ static int   l_cairo_in_stroke (lua_State* L);
/* ++ */ static int   l_cairo_in_fill (lua_State* L);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
/* ++ */ static int   l_cairo_in_clip (lua_State* L);
#endif
/* ww */ static int   l_cairo_stroke_extents (lua_State* L);
/* ww */ static int   l_cairo_fill_extents (lua_State* L);
/* ++ */ static int   l_cairo_reset_clip (lua_State* L);
/* ++ */ static int   l_cairo_clip (lua_State* L);
/* ++ */ static int   l_cairo_clip_preserve (lua_State* L);
/* ww */ static int   l_cairo_clip_extents (lua_State* L);
/* ++ */ static int   l_cairo_copy_clip_rectangle_list (lua_State* L);
/* ++ */ static int   l_cairo_rectangle_list_destroy (lua_State* L);
/* ++ */ static int   l_cairo_font_options_create (lua_State* L);
/* ++ */ static int   l_cairo_font_options_copy (lua_State* L);
/* ++ */ static int   l_cairo_font_options_destroy (lua_State* L);
/* ++ */ static int   l_cairo_font_options_status (lua_State* L);
/* ++ */ static int   l_cairo_font_options_merge (lua_State* L);
/* ++ */ static int   l_cairo_font_options_equal (lua_State* L);
/* ++ */ static int   l_cairo_font_options_hash (lua_State* L);
/* ++ */ static int   l_cairo_font_options_set_antialias (lua_State* L);
/* ++ */ static int   l_cairo_font_options_get_antialias (lua_State* L);
/* ++ */ static int   l_cairo_font_options_set_subpixel_order (lua_State* L);
/* ++ */ static int   l_cairo_font_options_get_subpixel_order (lua_State* L);
/* ++ */ static int   l_cairo_font_options_set_hint_style (lua_State* L);
/* ++ */ static int   l_cairo_font_options_get_hint_style (lua_State* L);
/* ++ */ static int   l_cairo_font_options_set_hint_metrics (lua_State* L);
/* ++ */ static int   l_cairo_font_options_get_hint_metrics (lua_State* L);
/* ++ */ static int   l_cairo_select_font_face (lua_State* L);
/* ++ */ static int   l_cairo_set_font_size (lua_State* L);
/* ++ */ static int   l_cairo_set_font_matrix (lua_State* L);
/* ++ */ static int   l_cairo_get_font_matrix (lua_State* L);
/* ++ */ static int   l_cairo_set_font_options (lua_State* L);
/* ++ */ static int   l_cairo_get_font_options (lua_State* L);
/* ++ */ static int   l_cairo_set_font_face (lua_State* L);
/* ++ */ static int   l_cairo_get_font_face (lua_State* L);
/* ++ */ static int   l_cairo_set_scaled_font (lua_State* L);
/* ++ */ static int   l_cairo_get_scaled_font (lua_State* L);
/* ++ */ static int   l_cairo_show_text (lua_State* L);
/* NI */ static int   l_cairo_show_glyphs (lua_State* L);
/* ++ */ static int   l_cairo_text_path (lua_State* L);
/* NI */ static int   l_cairo_glyph_path (lua_State* L);
/* ++ */ static int   l_cairo_text_extents (lua_State* L);
/* NI */ static int   l_cairo_glyph_extents (lua_State* L);
/* ++ */ static int   l_cairo_font_extents (lua_State* L);
/* ++ */ static int   l_cairo_font_face_reference (lua_State* L);
/* ++ */ static int   l_cairo_font_face_destroy (lua_State* L);
/* ++ */ static int   l_cairo_font_face_get_reference_count (lua_State* L);
/* ++ */ static int   l_cairo_font_face_status (lua_State* L);
/* ++ */ static int   l_cairo_font_face_get_type (lua_State* L);
/* NI */ static int   l_cairo_font_face_get_user_data (lua_State* L);
/* NI */ static int   l_cairo_font_face_set_user_data (lua_State* L);
/* ++ */ static int   l_cairo_scaled_font_create (lua_State* L);
/* ++ */ static int   l_cairo_scaled_font_reference (lua_State* L);
/* ++ */ static int   l_cairo_scaled_font_destroy (lua_State* L);
/* ++ */ static int   l_cairo_scaled_font_get_reference_count (lua_State* L);
/* ++ */ static int   l_cairo_scaled_font_status (lua_State* L);
/* ++ */ static int   l_cairo_scaled_font_get_type (lua_State* L);
/* NI */ static int   l_cairo_scaled_font_get_user_data (lua_State* L);
/* NI */ static int   l_cairo_scaled_font_set_user_data (lua_State* L);
/* ++ */ static int   l_cairo_scaled_font_extents (lua_State* L);
/* ++ */ static int   l_cairo_scaled_font_text_extents (lua_State* L);
/* NI */ static int   l_cairo_scaled_font_glyph_extents (lua_State* L);
/* ++ */ static int   l_cairo_scaled_font_get_font_face (lua_State* L);
/* ++ */ static int   l_cairo_scaled_font_get_font_matrix (lua_State* L);
/* ++ */ static int   l_cairo_scaled_font_get_ctm (lua_State* L);
/* ++ */ static int   l_cairo_scaled_font_get_font_options (lua_State* L);
/* ++ */ static int   l_cairo_get_operator (lua_State* L);
/* ++ */ static int   l_cairo_get_source (lua_State* L);
/* ++ */ static int   l_cairo_get_tolerance (lua_State* L);
/* ++ */ static int   l_cairo_get_antialias (lua_State* L);
/* ww */ static int   l_cairo_get_current_point (lua_State* L);
/* ++ */ static int   l_cairo_get_fill_rule (lua_State* L);
/* ++ */ static int   l_cairo_get_line_width (lua_State* L);
/* ++ */ static int   l_cairo_get_line_cap (lua_State* L);
/* ++ */ static int   l_cairo_get_line_join (lua_State* L);
/* ++ */ static int   l_cairo_get_miter_limit (lua_State* L);
/* ++ */ static int   l_cairo_get_dash_count (lua_State* L);
/* NI */ static int   l_cairo_get_dash (lua_State* L);
/* ++ */ static int   l_cairo_get_matrix (lua_State* L);
/* ++ */ static int   l_cairo_get_target (lua_State* L);
/* ++ */ static int   l_cairo_get_group_target (lua_State* L);
/* ++ */ static int   l_cairo_copy_path (lua_State* L);
/* ++ */ static int   l_cairo_copy_path_flat (lua_State* L);
/* ++ */ static int   l_cairo_append_path (lua_State* L);
/* ++ */ static int   l_cairo_path_destroy (lua_State* L);
/* ++ */ static int   l_cairo_path_status(lua_State* L);
/* ++ */ static int   l_cairo_path_num_data(lua_State* L);
/* ++ */ static int   l_cairo_path_data_header_type(lua_State* L);
/* ++ */ static int   l_cairo_path_data_header_length(lua_State* L);
/* ++ */ static int   l_cairo_path_data_point(lua_State* L);
/* ++ */ static int   l_cairo_status (lua_State* L);
/* ++ */ static int   l_cairo_status_to_string (lua_State* L);
/* ++ */ static int   l_cairo_surface_create_similar (lua_State* L);
/* ++ */ static int   l_cairo_surface_reference (lua_State* L);
/* ++ */ static int   l_cairo_surface_finish (lua_State* L);
/* ++ */ static int   l_cairo_surface_destroy (lua_State* L);
/* ++ */ static int   l_cairo_surface_get_reference_count (lua_State* L);
/* ++ */ static int   l_cairo_surface_status (lua_State* L);
/* ++ */ static int   l_cairo_surface_get_type (lua_State* L);
/* ++ */ static int   l_cairo_surface_get_content (lua_State* L);
#if CAIRO_HAS_PNG_FUNCTIONS
/* ++ */ static int   l_cairo_surface_write_to_png (lua_State* L);
/* NI */ static int   l_cairo_surface_write_to_png_stream (lua_State* L);
#endif /* CAIRO_HAS_PNG_FUNCTIONS */
/* NI */ static int   l_cairo_surface_get_user_data (lua_State* L);
/* NI */ static int   l_cairo_surface_set_user_data (lua_State* L);
/* ++ */ static int   l_cairo_surface_get_font_options (lua_State* L);
/* ++ */ static int   l_cairo_surface_flush (lua_State* L);
/* ++ */ static int   l_cairo_surface_mark_dirty (lua_State* L);
/* ++ */ static int   l_cairo_surface_mark_dirty_rectangle (lua_State* L);
/* ++ */ static int   l_cairo_surface_set_device_offset (lua_State* L);
/* ww */ static int   l_cairo_surface_get_device_offset (lua_State* L);
/* ++ */ static int   l_cairo_surface_set_fallback_resolution (lua_State* L);
/* ++ */ static int   l_cairo_image_surface_create (lua_State* L);
/* NI */ static int   l_cairo_image_surface_create_for_data (lua_State* L);
/* ++ */ static int   l_cairo_image_surface_get_data (lua_State* L);
/* ++ */ static int   l_cairo_image_surface_get_format (lua_State* L);
/* ++ */ static int   l_cairo_image_surface_get_width (lua_State* L);
/* ++ */ static int   l_cairo_image_surface_get_height (lua_State* L);
/* ++ */ static int   l_cairo_image_surface_get_stride (lua_State* L);
#if CAIRO_HAS_PNG_FUNCTIONS
/* ++ */ static int   l_cairo_image_surface_create_from_png (lua_State* L);
/* NI */ static int   l_cairo_image_surface_create_from_png_stream (lua_State* L);
#endif /* CAIRO_HAS_PNG_FUNCTIONS */
/* ++ */ static int   l_cairo_pattern_create_rgb (lua_State* L);
/* ++ */ static int   l_cairo_pattern_create_rgba (lua_State* L);
/* ++ */ static int   l_cairo_pattern_create_for_surface (lua_State* L);
/* ++ */ static int   l_cairo_pattern_create_linear (lua_State* L);
/* ++ */ static int   l_cairo_pattern_create_radial (lua_State* L);
/* ++ */ static int   l_cairo_pattern_reference (lua_State* L);
/* ++ */ static int   l_cairo_pattern_destroy (lua_State* L);
/* ++ */ static int   l_cairo_pattern_get_reference_count (lua_State* L);
/* ++ */ static int   l_cairo_pattern_status (lua_State* L);
/* NI */ static int   l_cairo_pattern_get_user_data (lua_State* L);
/* NI */ static int   l_cairo_pattern_set_user_data (lua_State* L);
/* ++ */ static int   l_cairo_pattern_get_type (lua_State* L);
/* ++ */ static int   l_cairo_pattern_add_color_stop_rgb (lua_State* L);
/* ++ */ static int   l_cairo_pattern_add_color_stop_rgba (lua_State* L);
/* ++ */ static int   l_cairo_pattern_set_matrix (lua_State* L);
/* ++ */ static int   l_cairo_pattern_get_matrix (lua_State* L);
/* ++ */ static int   l_cairo_pattern_set_extend (lua_State* L);
/* ++ */ static int   l_cairo_pattern_get_extend (lua_State* L);
/* ++ */ static int   l_cairo_pattern_set_filter (lua_State* L);
/* ++ */ static int   l_cairo_pattern_get_filter (lua_State* L);
/* ww */ static int   l_cairo_pattern_get_rgba (lua_State* L);
/* ww */ static int   l_cairo_pattern_get_surface (lua_State* L);
/* ww */ static int   l_cairo_pattern_get_color_stop_rgba (lua_State* L);
/* ww */ static int   l_cairo_pattern_get_color_stop_count (lua_State* L);
/* ww */ static int   l_cairo_pattern_get_linear_points (lua_State* L);
/* ww */ static int   l_cairo_pattern_get_radial_circles (lua_State* L);
/* ++ */ static int   l_cairo_matrix_init (lua_State* L);
/* ++ */ static int   l_cairo_matrix_init_identity (lua_State* L);
/* ++ */ static int   l_cairo_matrix_init_translate (lua_State* L);
/* ++ */ static int   l_cairo_matrix_init_scale (lua_State* L);
/* ++ */ static int   l_cairo_matrix_init_rotate (lua_State* L);
/* ++ */ static int   l_cairo_matrix_translate (lua_State* L);
/* ++ */ static int   l_cairo_matrix_scale (lua_State* L);
/* ++ */ static int   l_cairo_matrix_rotate (lua_State* L);
/* ++ */ static int   l_cairo_matrix_invert (lua_State* L);
/* ++ */ static int   l_cairo_matrix_multiply (lua_State* L);
/* ww */ static int   l_cairo_matrix_transform_distance (lua_State* L);
/* ww */ static int   l_cairo_matrix_transform_point (lua_State* L);
/* ++ */ static int   l_cairo_debug_reset_static_data (lua_State* L);

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
/* ww */ static int   l_cairo_path_extents (lua_State* L);
/* ++ */ static int   l_cairo_has_current_point (lua_State* L);
/* ++ */ static int   l_cairo_surface_copy_page (lua_State* L);
/* ++ */ static int   l_cairo_surface_show_page (lua_State* L);
/* ++ */ static int   l_cairo_format_stride_for_width (lua_State* L);
#endif

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
/* NI */ static int   l_cairo_glyph_allocate (lua_State* L);
/* NI */ static int   l_cairo_glyph_free (lua_State* L);
/* NI */ static int   l_cairo_text_cluster_allocate (lua_State* L);
/* NI */ static int   l_cairo_text_cluster_free (lua_State* L);
/* NI */ static int   l_cairo_show_text_glyphs (lua_State* L);
/* NI */ static int   l_cairo_scaled_font_text_to_glyphs (lua_State* L);
/* NI */ static int   l_cairo_scaled_font_get_scale_matrix (lua_State* L);
/* ++ */ static int   l_cairo_toy_font_face_create (lua_State* L);
/* ++ */ static int   l_cairo_toy_font_face_get_family (lua_State* L);
/* ++ */ static int   l_cairo_toy_font_face_get_slant (lua_State* L);
/* ++ */ static int   l_cairo_toy_font_face_get_weight (lua_State* L);
/* ++ */ static int   l_cairo_user_font_face_create (lua_State* L);
/* NI */ static int   l_cairo_user_font_face_set_init_func (lua_State* L);
/* NI */ static int   l_cairo_user_font_face_set_render_glyph_func (lua_State* L);
/* NI */ static int   l_cairo_user_font_face_set_text_to_glyphs_func (lua_State* L);
/* NI */ static int   l_cairo_user_font_face_set_unicode_to_glyph_func (lua_State* L);
/* NI */ static int   l_cairo_user_font_face_get_init_func (lua_State* L);
/* NI */ static int   l_cairo_user_font_face_get_render_glyph_func (lua_State* L);
/* NI */ static int   l_cairo_user_font_face_get_text_to_glyphs_func (lua_State* L);
/* NI */ static int   l_cairo_user_font_face_get_unicode_to_glyph_func (lua_State* L);
/* ++ */ static int   l_cairo_surface_get_fallback_resolution (lua_State* L);
/* ++ */ static int   l_cairo_surface_has_show_text_glyphs (lua_State* L);
#endif

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
/* NI */ static int   l_cairo_surface_create_for_rectangle (lua_State* L);
/* NI */ static int   l_cairo_surface_get_mime_data (lua_State* L);
/* NI */ static int   l_cairo_surface_set_mime_data (lua_State* L);
/* ++ */ static int   l_cairo_surface_get_device (lua_State* L);
/* NI */ static int   l_cairo_recording_surface_create (lua_State* L);
/* NI */ static int   l_cairo_recording_surface_ink_extents (lua_State* L);
/* ++ */ static int   l_cairo_region_create (lua_State* L);
/* ++ */ static int   l_cairo_region_create_rectangle (lua_State* L);
/* NI */ static int   l_cairo_region_create_rectangles (lua_State* L);
/* ++ */ static int   l_cairo_region_copy (lua_State* L);
/* ++ */ static int   l_cairo_region_reference (lua_State* L);
/* ++ */ static int   l_cairo_region_destroy (lua_State* L);
/* ++ */ static int   l_cairo_region_equal (lua_State* L);
/* ++ */ static int   l_cairo_region_status (lua_State* L);
/* ++ */ static int   l_cairo_region_get_extents (lua_State* L);
/* ++ */ static int   l_cairo_region_num_rectangles (lua_State* L);
/* ++ */ static int   l_cairo_region_get_rectangle (lua_State* L);
/* ++ */ static int   l_cairo_region_is_empty (lua_State* L);
/* ++ */ static int   l_cairo_region_contains_rectangle (lua_State* L);
/* ++ */ static int   l_cairo_region_contains_point (lua_State* L);
/* ++ */ static int   l_cairo_region_translate (lua_State* L);
/* ++ */ static int   l_cairo_region_subtract (lua_State* L);
/* ++ */ static int   l_cairo_region_subtract_rectangle (lua_State* L);
/* ++ */ static int   l_cairo_region_intersect (lua_State* L);
/* ++ */ static int   l_cairo_region_intersect_rectangle (lua_State* L);
/* ++ */ static int   l_cairo_region_union (lua_State* L);
/* ++ */ static int   l_cairo_region_union_rectangle (lua_State* L);
/* ++ */ static int   l_cairo_region_xor (lua_State* L);
/* ++ */ static int   l_cairo_region_xor_rectangle (lua_State* L);
/* ++ */ static int   l_cairo_device_reference (lua_State* L);
/* ++ */ static int   l_cairo_device_get_type (lua_State* L);
/* ++ */ static int   l_cairo_device_status (lua_State* L);
/* ++ */ static int   l_cairo_device_acquire (lua_State* L);
/* ++ */ static int   l_cairo_device_release (lua_State* L);
/* ++ */ static int   l_cairo_device_flush (lua_State* L);
/* ++ */ static int   l_cairo_device_finish (lua_State* L);
/* ++ */ static int   l_cairo_device_destroy (lua_State* L);
/* ++ */ static int   l_cairo_device_get_reference_count (lua_State* L);
/* NI */ static int   l_cairo_device_get_user_data (lua_State* L);
/* NI */ static int   l_cairo_device_set_user_data (lua_State* L);
#endif /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0) */

#if CAIRO_HAS_SVG_SURFACE
/* ++ */ static int   l_cairo_svg_surface_create (lua_State* L);
/* NI */ static int   l_cairo_svg_surface_create_for_stream (lua_State* L);
/* ++ */ static int   l_cairo_svg_surface_restrict_to_version (lua_State* L);
/* NI */ static int   l_cairo_svg_get_versions (lua_State* L);
/* ++ */ static int   l_cairo_svg_version_to_string (lua_State* L);
#endif /* CAIRO_HAS_SVG_SURFACE */

#if CAIRO_HAS_PDF_SURFACE
/* ++ */ static int   l_cairo_pdf_surface_create (lua_State* L);
/* NI */ static int   l_cairo_pdf_surface_create_for_stream (lua_State* L);
/* ++ */ static int   l_cairo_pdf_surface_set_size (lua_State* L);
/* NI */ static int   l_cairo_pdf_surface_restrict_to_version (lua_State* L);
/* NI */ static int   l_cairo_pdf_get_versions (lua_State* L);
/* NI */ static int   l_cairo_pdf_version_to_string (lua_State* L);
#endif /* CAIRO_HAS_PDF_SURFACE */

#if CAIRO_HAS_PS_SURFACE
/* ++ */ static int   l_cairo_ps_surface_create (lua_State* L);
/* NI */ static int   l_cairo_ps_surface_create_for_stream (lua_State* L);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
/* ++ */ static int   l_cairo_ps_surface_restrict_to_level (lua_State* L);
/* NI */ static int   l_cairo_ps_get_levels (lua_State* L);
/* ++ */ static int   l_cairo_ps_level_to_string (lua_State* L);
/* ++ */ static int   l_cairo_ps_surface_set_eps (lua_State* L);
/* ++ */ static int   l_cairo_ps_surface_get_eps (lua_State* L);
#endif
/* ++ */ static int   l_cairo_ps_surface_set_size (lua_State* L);
/* ++ */ static int   l_cairo_ps_surface_dsc_comment (lua_State* L);
/* ++ */ static int   l_cairo_ps_surface_dsc_begin_setup (lua_State* L);
/* ++ */ static int   l_cairo_ps_surface_dsc_begin_page_setup (lua_State* L);
#endif /* CAIRO_HAS_PS_SURFACE */

#if CAIRO_HAS_XCB_SURFACE
/* NI */ static int   l_cairo_xcb_surface_create (lua_State* L);
/* NI */ static int   l_cairo_xcb_surface_create_for_bitmap (lua_State* L);
/* ++ */ static int   l_cairo_xcb_surface_set_size (lua_State* L);
#if CAIRO_HAS_XLIB_XRENDER_SURFACE
/* NI */ static int   l_cairo_xcb_surface_create_with_xrender_format (lua_State* L);
/* NI */ static int   l_cairo_xlib_surface_get_xrender_format (lua_State* L);
#endif
#endif /* CAIRO_HAS_XCB_SURFACE */

#if CAIRO_HAS_XLIB_SURFACE
/* NI */ static int   l_cairo_xlib_surface_create (lua_State* L);
/* NI */ static int   l_cairo_xlib_surface_create_for_bitmap (lua_State* L);
/* ++ */ static int   l_cairo_xlib_surface_set_size (lua_State* L);
/* NI */ static int   l_cairo_xlib_surface_set_drawable (lua_State* L);
/* ++ */ static int   l_cairo_xlib_surface_get_display (lua_State* L);
/* NI */ static int   l_cairo_xlib_surface_get_drawable (lua_State* L);
/* ++ */ static int   l_cairo_xlib_surface_get_screen (lua_State* L);
/* ++ */ static int   l_cairo_xlib_surface_get_visual (lua_State* L);
/* ++ */ static int   l_cairo_xlib_surface_get_depth (lua_State* L);
/* ++ */ static int   l_cairo_xlib_surface_get_width (lua_State* L);
/* ++ */ static int   l_cairo_xlib_surface_get_height (lua_State* L);
#if CAIRO_HAS_XLIB_XRENDER_SURFACE
/* NI */ static int   l_cairo_xlib_surface_create_with_xrender_format (lua_State* L);
#endif
/* NI */ static int   l_cairo_xcb_device_debug_cap_xshm_version (lua_State* L);
/* NI */ static int   l_cairo_xcb_device_debug_cap_xrender_version (lua_State* L);
#endif /* CAIRO_HAS_XLIB_SURFACE */

#if CAIRO_HAS_WIN32_SURFACE
/* ++ */ static int   l_cairo_win32_surface_create (lua_State* L);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
/* ++ */ static int   l_cairo_win32_printing_surface_create (lua_State* L);
#endif
/* ++ */ static int   l_cairo_win32_surface_create_with_ddb (lua_State* L);
/* ++ */ static int   l_cairo_win32_surface_create_with_dib (lua_State* L);
/* ++ */ static int   l_cairo_win32_surface_get_dc (lua_State* L);
/* ++ */ static int   l_cairo_win32_surface_get_image (lua_State* L);
#if CAIRO_HAS_WIN32_FONT
/* ++ */ static int   l_cairo_win32_font_face_create_for_logfontw (lua_State* L);
/* ++ */ static int   l_cairo_win32_font_face_create_for_hfont (lua_State* L);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
/* ++ */ static int   l_cairo_win32_font_face_create_for_logfontw_hfont (lua_State* L);
#endif
/* ++ */ static int   l_cairo_win32_scaled_font_select_font (lua_State* L);
/* ++ */ static int   l_cairo_win32_scaled_font_done_font (lua_State* L);
/* ++ */ static int   l_cairo_win32_scaled_font_get_metrics_factor (lua_State* L);
/* ++ */ static int   l_cairo_win32_scaled_font_get_logical_to_device (lua_State* L);
/* ++ */ static int   l_cairo_win32_scaled_font_get_device_to_logical (lua_State* L);
#endif /* CAIRO_HAS_WIN32_FONT */
#endif /* CAIRO_HAS_WIN32_SURFACE */

#if CAIRO_HAS_BEOS_SURFACE
/* ++ */ static int   l_cairo_beos_surface_create (lua_State* L);
/* ++ */ static int   l_cairo_beos_surface_create_for_bitmap (lua_State* L);
#endif /* CAIRO_HAS_BEOS_SURFACE */

#if CAIRO_HAS_DIRECTFB_SURFACE
/* ++ */ static int   l_cairo_directfb_surface_create (lua_State* L);
#endif /*CAIRO_HAS_DIRECTFB_SURFACE*/

#if CAIRO_HAS_OS2_SURFACE
/* ++ */ static int   l_cairo_os2_init (lua_State* L);
/* ++ */ static int   l_cairo_os2_fini (lua_State* L);
/* NI */ static int   l_cairo_os2_surface_create (lua_State* L);
/* NI */ static int   l_cairo_os2_surface_set_hwnd (lua_State* L);
/* ++ */ static int   l_cairo_os2_surface_set_size (lua_State* L);
/* NI */ static int   l_cairo_os2_surface_refresh_window (lua_State* L);
/* ++ */ static int   l_cairo_os2_surface_set_manual_window_refresh (lua_State* L);
/* ++ */ static int   l_cairo_os2_surface_get_manual_window_refresh (lua_State* L);
/* NI */ static int   l_cairo_os2_surface_create_for_window (lua_State* L);
/* NI */ static int   l_cairo_os2_surface_get_hps (lua_State* L);
/* NI */ static int   l_cairo_os2_surface_set_hps (lua_State* L);
#endif /* CAIRO_HAS_OS2_SURFACE */

#if CAIRO_HAS_GLITZ_SURFACE
/* ++ */ static int   l_cairo_glitz_surface_create (lua_State* L);
#endif /* CAIRO_HAS_GLITZ_SURFACE */

#if CAIRO_HAS_QUARTZ_IMAGE_SURFACE
/* NI */ static int   l_cairo_quartz_image_surface_create (lua_State* L);
/* NI */ static int   l_cairo_quartz_image_surface_get_image (lua_State* L);
#endif /* CAIRO_HAS_QUARTZ_IMAGE_SURFACE */

#if CAIRO_HAS_QUARTZ_SURFACE
/* ++ */ static int   l_cairo_quartz_surface_create (lua_State* L);
/* NI */ static int   l_cairo_quartz_surface_create_for_cg_context (lua_State* L);
/* NI */ static int   l_cairo_quartz_surface_get_cg_context (lua_State* L);
#if CAIRO_HAS_QUARTZ_FONT
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
/* NI */ static int   l_cairo_quartz_font_face_create_for_cgfont (lua_State* L);
/* NI */ static int   l_cairo_quartz_font_face_create_for_atsu_font_id (lua_State* L);
#endif
#endif /* CAIRO_HAS_QUARTZ_FONT */
#endif /* CAIRO_HAS_QUARTZ_SURFACE */

#if CAIRO_HAS_FT_FONT
/* NI */ static int   l_cairo_ft_font_face_create_for_pattern (lua_State* L);
/* NI */ static int   l_cairo_ft_font_options_substitute (lua_State* L);
/* NI */ static int   l_cairo_ft_font_face_create_for_ft_face (lua_State* L);
/* NI */ static int   l_cairo_ft_scaled_font_lock_face (lua_State* L);
/* NI */ static int   l_cairo_ft_scaled_font_unlock_face (lua_State* L);
#endif /* CAIRO_HAS_FT_FONT */

#if CAIRO_HAS_DRM_SURFACE
/* NI */ static int   l_cairo_drm_device_get (lua_State* L);
/* NI */ static int   l_cairo_drm_device_get_for_fd (lua_State* L);
/* NI */ static int   l_cairo_drm_device_default (lua_State* L);
/* NI */ static int   l_cairo_drm_device_get_fd (lua_State* L);
/* NI */ static int   l_cairo_drm_device_throttle (lua_State* L);
/* NI */ static int   l_cairo_drm_surface_create (lua_State* L);
/* NI */ static int   l_cairo_drm_surface_create_for_name (lua_State* L);
/* NI */ static int   l_cairo_drm_surface_create_from_cacheable_image (lua_State* L);
/* NI */ static int   l_cairo_drm_surface_enable_scan_out (lua_State* L);
/* NI */ static int   l_cairo_drm_surface_get_handle (lua_State* L);
/* NI */ static int   l_cairo_drm_surface_get_name (lua_State* L);
/* NI */ static int   l_cairo_drm_surface_get_format (lua_State* L);
/* NI */ static int   l_cairo_drm_surface_get_width (lua_State* L);
/* NI */ static int   l_cairo_drm_surface_get_height (lua_State* L);
/* NI */ static int   l_cairo_drm_surface_get_stride (lua_State* L);
/* NI */ static int   l_cairo_drm_surface_map_to_image (lua_State* L);
/* NI */ static int   l_cairo_drm_surface_unmap (lua_State* L);
#endif /* CAIRO_HAS_DRM_SURFACE */

#if CAIRO_HAS_DRM_XR_FUNCTIONS
/* NI */ static int   l_cairo_drm_xr_enable (lua_State* L);
/* NI */ static int   l_cairo_drm_xr_pixmap_link_bo (lua_State* L);
/* NI */ static int   l_cairo_drm_xr_pixmap_unlink_bo (lua_State* L);
#endif /* CAIRO_HAS_DRM_XR_FUNCTIONS */

#if CAIRO_HAS_GL_SURFACE
/* NI */ static int   l_cairo_gl_surface_create (lua_State* L);
/* NI */ static int   l_cairo_gl_surface_create_for_texture (lua_State* L);
/* NI */ static int   l_cairo_gl_surface_set_size (lua_State* L);
/* NI */ static int   l_cairo_gl_surface_get_width (lua_State* L);
/* NI */ static int   l_cairo_gl_surface_get_height (lua_State* L);
/* NI */ static int   l_cairo_gl_surface_swapbuffers (lua_State* L);
#if CAIRO_HAS_GLX_FUNCTIONS
/* NI */ static int   l_cairo_glx_device_create (lua_State* L);
/* NI */ static int   l_cairo_glx_device_get_display (lua_State* L);
/* NI */ static int   l_cairo_glx_device_get_context (lua_State* L);
/* NI */ static int   l_cairo_gl_surface_create_for_window (lua_State* L);
#endif
#if CAIRO_HAS_WGL_FUNCTIONS
/* NI */ static int   l_cairo_wgl_device_create (lua_State* L);
/* NI */ static int   l_cairo_wgl_device_get_context (lua_State* L);
/* NI */ static int   l_cairo_gl_surface_create_for_dc (lua_State* L);
#endif
#if CAIRO_HAS_EGL_FUNCTIONS
/* NI */ static int   l_cairo_egl_device_create (lua_State* L);
/* NI */ static int   l_cairo_gl_surface_create_for_egl (lua_State* L);
#endif
#endif /* CAIRO_HAS_GL_SURFACE */

#if CAIRO_HAS_QT_SURFACE
/* NI */ static int   l_cairo_qt_surface_create (lua_State* L);
/* NI */ static int   l_cairo_qt_surface_create_with_qimage (lua_State* L);
/* NI */ static int   l_cairo_qt_surface_create_with_qpixmap (lua_State* L);
/* NI */ static int   l_cairo_qt_surface_get_qpainter (lua_State* L);
/* NI */ static int   l_cairo_qt_surface_get_image (lua_State* L);
/* NI */ static int   l_cairo_qt_surface_get_qimage (lua_State* L);
#endif /* CAIRO_HAS_QT_SURFACE */

#if CAIRO_HAS_SCRIPT_SURFACE
/* NI */ static int   l_cairo_script_create (lua_State* L);
/* NI */ static int   l_cairo_script_create_for_stream (lua_State* L);
/* NI */ static int   l_cairo_script_write_comment (lua_State* L);
/* NI */ static int   l_cairo_script_set_mode (lua_State* L);
/* NI */ static int   l_cairo_script_get_mode (lua_State* L);
/* NI */ static int   l_cairo_script_surface_create (lua_State* L);
/* NI */ static int   l_cairo_script_surface_create_for_target (lua_State* L);
/* NI */ static int   l_cairo_script_from_recording_surface (lua_State* L);
#endif /*CAIRO_HAS_SCRIPT_SURFACE*/

#if CAIRO_HAS_SKIA_SURFACE
/* NI */ static int   l_cairo_skia_surface_create (lua_State* L);
/* NI */ static int   l_cairo_skia_surface_create_for_data (lua_State* L);
/* NI */ static int   l_cairo_skia_surface_get_data (lua_State* L);
/* NI */ static int   l_cairo_skia_surface_get_format (lua_State* L);
/* NI */ static int   l_cairo_skia_surface_get_width (lua_State* L);
/* NI */ static int   l_cairo_skia_surface_get_height (lua_State* L);
/* NI */ static int   l_cairo_skia_surface_get_stride (lua_State* L);
/* NI */ static int   l_cairo_skia_surface_get_image (lua_State* L);
#endif /* CAIRO_HAS_SKIA_SURFACE */

#if CAIRO_HAS_TEE_SURFACE
/* NI */ static int   l_cairo_tee_surface_create (lua_State* L);
/* NI */ static int   l_cairo_tee_surface_add (lua_State* L);
/* NI */ static int   l_cairo_tee_surface_remove (lua_State* L);
/* NI */ static int   l_cairo_tee_surface_index (lua_State* L);
#endif /*CAIRO_HAS_TEE_SURFACE*/

#if CAIRO_HAS_VG_SURFACE
#if CAIRO_HAS_GLX_FUNCTIONS
/* NI */ static int   l_cairo_vg_context_create_for_glx (lua_State* L);
#endif
#if CAIRO_HAS_EGL_FUNCTIONS
/* NI */ static int   l_cairo_vg_context_create_for_egl (lua_State* L);
#endif
/* NI */ static int   l_cairo_vg_context_status (lua_State* L);
/* NI */ static int   l_cairo_vg_context_destroy (lua_State* L);
/* NI */ static int   l_cairo_vg_surface_create (lua_State* L);
/* NI */ static int   l_cairo_vg_surface_create_for_image (lua_State* L);
/* NI */ static int   l_cairo_vg_surface_get_image (lua_State* L);
/* NI */ static int   l_cairo_vg_surface_get_format (lua_State* L);
/* NI */ static int   l_cairo_vg_surface_get_height (lua_State* L);
/* NI */ static int   l_cairo_vg_surface_get_width (lua_State* L);
#endif /* CAIRO_HAS_VG_SURFACE */

#if CAIRO_HAS_XML_SURFACE
/* NI */ static int   l_cairo_xml_create (lua_State* L);
/* NI */ static int   l_cairo_xml_create_for_stream (lua_State* L);
/* NI */ static int   l_cairo_xml_surface_create (lua_State* L);
/* NI */ static int   l_cairo_xml_for_recording_surface (lua_State* L);
#endif /* CAIRO_HAS_XML_SURFACE */
// --------------------------------------------------------------------------


#endif /* LUA_CAIRO_PRIVATE_H */
