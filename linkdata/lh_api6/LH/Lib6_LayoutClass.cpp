#include <LH/Lib6_LayoutClass.h>
#include <LH/Lib6_LayoutItem.h>

namespace LH {
namespace Lib6 {

void LayoutClass::destroy( LayoutItem * w )
{
    delete w;
}

} // namespace Lib6
} // namespace LH
