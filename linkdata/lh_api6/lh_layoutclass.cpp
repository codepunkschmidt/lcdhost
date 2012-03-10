#include "lh_layoutclass.h"
#include "lh_layoutitem.h"

using namespace lh::api6;

void layoutclass::destroy( layoutitem * w )
{
    delete w;
}
