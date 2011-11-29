#include "lh_widget.h"
#include "lh_item.h"

using namespace lh::api6;

void widget::destroy( item * w )
{
    delete w;
}
