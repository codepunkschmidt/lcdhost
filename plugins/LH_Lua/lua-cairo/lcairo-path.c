/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif


/* Path creation functions */

// cairo_public void
// cairo_new_path (cairo_t *cr);
static int l_cairo_new_path(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_new_path(cr);
    return 0;
}


// cairo_public void
// cairo_move_to (cairo_t *cr,
//              double x, double y);
static int l_cairo_move_to(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double x = luaL_checknumber(L, 2);
    double y = luaL_checknumber(L, 3);
    cairo_move_to(cr, x, y);
    return 0;
}


// cairo_public void
// cairo_new_sub_path (cairo_t *cr);
static int l_cairo_new_sub_path(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_new_sub_path(cr);
    return 0;
}


// cairo_public void
// cairo_line_to (cairo_t *cr,
//              double x, double y);
static int l_cairo_line_to(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double x = luaL_checknumber(L, 2);
    double y = luaL_checknumber(L, 3);
    cairo_line_to(cr, x, y);
    return 0;
}


// cairo_public void
// cairo_curve_to (cairo_t *cr,
//         double x1, double y1,
//         double x2, double y2,
//         double x3, double y3);
static int l_cairo_curve_to(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double x1 = luaL_checknumber(L, 2);
    double y1 = luaL_checknumber(L, 3);
    double x2 = luaL_checknumber(L, 4);
    double y2 = luaL_checknumber(L, 5);
    double x3 = luaL_checknumber(L, 6);
    double y3 = luaL_checknumber(L, 7);
    cairo_curve_to(cr, x1, y1, x2, y2, x3, y3);
    return 0;
}


// cairo_public void
// cairo_arc (cairo_t *cr,
//        double xc, double yc,
//        double radius,
//        double angle1, double angle2);
static int l_cairo_arc(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double xc = luaL_checknumber(L, 2);
    double yc = luaL_checknumber(L, 3);
    double radius = luaL_checknumber(L, 4);
    double angle1 = luaL_checknumber(L, 5);
    double angle2 = luaL_checknumber(L, 6);
    cairo_arc(cr, xc, yc, radius, angle1, angle2);
    return 0;
}


// cairo_public void
// cairo_arc_negative (cairo_t *cr,
//             double xc, double yc,
//             double radius,
//             double angle1, double angle2);
static int l_cairo_arc_negative(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double xc = luaL_checknumber(L, 2);
    double yc = luaL_checknumber(L, 3);
    double radius = luaL_checknumber(L, 4);
    double angle1 = luaL_checknumber(L, 5);
    double angle2 = luaL_checknumber(L, 6);
    cairo_arc_negative(cr, xc, yc, radius, angle1, angle2);
    return 0;
}


// cairo_public void
// cairo_rel_move_to (cairo_t *cr,
//              double dx, double dy);
static int l_cairo_rel_move_to(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double dx = luaL_checknumber(L, 2);
    double dy = luaL_checknumber(L, 3);
    cairo_rel_move_to(cr, dx, dy);
    return 0;
}


// cairo_public void
// cairo_rel_line_to (cairo_t *cr,
//              double dx, double dy);
static int l_cairo_rel_line_to(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double dx = luaL_checknumber(L, 2);
    double dy = luaL_checknumber(L, 3);
    cairo_rel_line_to(cr, dx, dy);
    return 0;
}


// cairo_public void
// cairo_rel_curve_to (cairo_t *cr,
//             double dx1, double dy1,
//             double dx2, double dy2,
//             double dx3, double dy3);
static int l_cairo_rel_curve_to(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double dx1 = luaL_checknumber(L, 2);
    double dy1 = luaL_checknumber(L, 3);
    double dx2 = luaL_checknumber(L, 4);
    double dy2 = luaL_checknumber(L, 5);
    double dx3 = luaL_checknumber(L, 6);
    double dy3 = luaL_checknumber(L, 7);
    cairo_rel_curve_to(cr, dx1, dy1, dx2, dy2, dx3, dy3);
    return 0;
}


// cairo_public void
// cairo_rectangle (cairo_t *cr,
//          double x, double y,
//          double width, double height);
static int l_cairo_rectangle(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    double x = luaL_checknumber(L, 2);
    double y = luaL_checknumber(L, 3);
    double width = luaL_checknumber(L, 4);
    double height = luaL_checknumber(L, 5);
    cairo_rectangle(cr, x, y, width, height);
    return 0;
}


// cairo_public void
// cairo_close_path (cairo_t *cr);
static int l_cairo_close_path(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_close_path(cr);
    return 0;
}


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
// cairo_public void
// cairo_path_extents (cairo_t *cr,
//             double *x1, double *y1,
//             double *x2, double *y2);
static int l_cairo_path_extents(lua_State* L)
{
    /// WARN:different usage
    cairo_t *cr = get_cairo_t (L, 1);
    double x1 = luaL_optnumber(L, 2, 0.0);
    double y1 = luaL_optnumber(L, 3, 0.0);
    double x2 = luaL_optnumber(L, 4, 0.0);
    double y2 = luaL_optnumber(L, 5, 0.0);
    cairo_path_extents (cr, &x1, &y1, &x2, &y2);
    lua_pushnumber(L, x1);
    lua_pushnumber(L, y1);
    lua_pushnumber(L, x2);
    lua_pushnumber(L, y2);
    return 4;
}
#endif


// cairo_public cairo_path_t *
// cairo_copy_path (cairo_t *cr);
static int l_cairo_copy_path(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_path_t *v = cairo_copy_path (cr);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public cairo_path_t *
// cairo_copy_path_flat (cairo_t *cr);
static int l_cairo_copy_path_flat(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    cairo_path_t *v = cairo_copy_path_flat (cr);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public void
// cairo_append_path (cairo_t		*cr,
//            const cairo_path_t	*path);
static int l_cairo_append_path(lua_State* L)
{
    cairo_t *cr = get_cairo_t (L, 1);
    const cairo_path_t *path = get_cairo_path_t (L, 2);
    cairo_append_path (cr, path);
    return 0;
}


// cairo_public void
// cairo_path_destroy (cairo_path_t *path);
static int l_cairo_path_destroy(lua_State* L)
{
    remove_Context(L, 1); // if called via Context userdata
    cairo_path_t *path = get_cairo_path_t (L, 1);
    cairo_path_destroy (path);
    return 0;
}



/* path data access (functions are not part of the Cairo API) */


/*
typedef struct cairo_path {
    cairo_status_t status;
    cairo_path_data_t *data;
    int num_data;
} cairo_path_t;
*/

// local status = cairo.path_status(path)
static int l_cairo_path_status(lua_State* L)
{
    cairo_path_t *path = get_cairo_path_t (L, 1);

    cairo_status_t status = path->status;

    lua_pushnumber(L, status);
    return 1;
}


// local numdata = cairo.path_num_data(path)
static int l_cairo_path_num_data(lua_State* L)
{
    cairo_path_t *path = get_cairo_path_t (L, 1);

    int num_data = path->num_data;

    lua_pushinteger(L, num_data);
    return 1;
}


/*
typedef enum _cairo_path_data_type {
    CAIRO_PATH_MOVE_TO,
    CAIRO_PATH_LINE_TO,
    CAIRO_PATH_CURVE_TO,
    CAIRO_PATH_CLOSE_PATH
} cairo_path_data_type_t;
*/

/*
typedef union _cairo_path_data_t cairo_path_data_t;
union _cairo_path_data_t {
    struct {
	cairo_path_data_type_t type;
	int length;
    } header;
    struct {
	double x, y;
    } point;
};
*/

// local headertype = cairo.path_data_header_type(path, i)
static int l_cairo_path_data_header_type(lua_State* L)
{
    cairo_path_t *path = get_cairo_path_t (L, 1);
    int i = luaL_checkinteger(L, 2);

    int num_data = path->num_data;
    if (i < 0 || i >= num_data)
    {
        luaL_error(L, "index error");
        return 0;
    }

    cairo_path_data_t* data = &path->data[i];
    cairo_path_data_type_t t = data->header.type;

    lua_pushinteger(L, t);
    return 1;
}

// local headerlength = cairo.path_data_header_length(path, i)
static int l_cairo_path_data_header_length(lua_State* L)
{
    cairo_path_t *path = get_cairo_path_t (L, 1);
    int i = luaL_checkinteger(L, 2);

    int num_data = path->num_data;
    if (i < 0 || i >= num_data)
    {
        luaL_error(L, "index error");
        return 0;
    }

    cairo_path_data_t* data = &path->data[i];
    int length = data->header.length;

    lua_pushinteger(L, length);
    return 1;
}

// local x,y = cairo.path_data_point(path, i, 1)
static int l_cairo_path_data_point(lua_State* L)
{
    cairo_path_t *path = get_cairo_path_t (L, 1);
    int i = luaL_checkinteger(L, 2);
    int pi = luaL_checkinteger(L, 3);

    int num_data = path->num_data;
    if (i < 0 || i >= num_data)
    {
        luaL_error(L, "index error");
        return 0;
    }

    cairo_path_data_t* data = &path->data[i];

/*
 * As of cairo 1.4, cairo does not mind if there are more elements in
 * a portion of the path than needed.  Such elements can be used by
 * users of the cairo API to hold extra values in the path data
 * structure.  For this reason, it is recommended that applications
 * always use data->header.length to
 * iterate over the path data, instead of hardcoding the number of
 * elements for each element type.
*/
    // int maxpi = 0;
    // switch (data->header.type) {
    // case CAIRO_PATH_MOVE_TO:
    //     maxpi = 1;
    //     break;
    // case CAIRO_PATH_LINE_TO:
    //     maxpi = 1;
    //     break;
    // case CAIRO_PATH_CURVE_TO:
    //     maxpi = 3;
    //     break;
    // case CAIRO_PATH_CLOSE_PATH:
    //     break;
    // }
    // if (pi > maxpi)
    // {
    //     luaL_error(L, "point index error");
    //     return 0;
    // }

    double x = data[pi].point.x;
    double y = data[pi].point.y;

    lua_pushnumber(L, x);
    lua_pushnumber(L, y);
    return 2;
}



static const struct luaL_Reg path_f [] = {
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
    {"copy_path",                                l_cairo_copy_path},
    {"copy_path_flat",                           l_cairo_copy_path_flat},
    {"append_path",                              l_cairo_append_path},
    {"path_destroy",                             l_cairo_path_destroy},

    {"path_status",                              l_cairo_path_status},
    {"path_num_data",                            l_cairo_path_num_data},
    {"path_data_header_type",                    l_cairo_path_data_header_type},
    {"path_data_header_length",                  l_cairo_path_data_header_length},
    {"path_data_point",                          l_cairo_path_data_point},

	{NULL, NULL} /* sentinel */
};


static int luaopen_lcairo_path(lua_State* L)
{
    // register functions
    luaL_register(L, LUACAIRO, path_f);

    return 0;
}
