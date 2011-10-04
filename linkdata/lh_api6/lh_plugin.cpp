#include "lh_plugin.h"

using namespace lh::api6;

bool plugin::init()
{
    if( !object::init() ) return false;

    // load stuff registered compile-time

    return true;
}
