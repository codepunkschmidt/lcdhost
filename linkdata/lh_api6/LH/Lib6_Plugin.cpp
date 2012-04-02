#include <LH/Lib6_Plugin.h>

namespace LH {
namespace Lib6 {

bool Plugin::init()
{
    if( !Object::init() ) return false;

    // load stuff registered compile-time

    return true;
}

} // namespace Lib6
} // namespace LH
