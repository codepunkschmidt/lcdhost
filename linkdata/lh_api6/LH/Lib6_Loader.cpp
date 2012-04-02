#include <LH/Lib6_Loader.h>

namespace LH {
namespace Lib6 {

const Loader * Loader::chain( Loader * new_loader )
{
    static Loader * first_ = 0;
    if( new_loader )
    {
        new_loader->next_ = first_;
        first_ = new_loader;
    }
    return first_;
}

} // namespace Lib6
} // namespace LH
