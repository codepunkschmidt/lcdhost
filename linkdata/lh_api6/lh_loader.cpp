#include "lh_loader.h"

const lh::api6::loader * lh::api6::loader::chain( loader * new_loader )
{
    static loader * first_ = 0;
    if( new_loader )
    {
        new_loader->next_ = first_;
        first_ = new_loader;
    }
    return first_;
}
