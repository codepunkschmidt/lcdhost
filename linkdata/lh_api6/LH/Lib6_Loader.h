#ifndef LH_LIB6_LOADER_H
#define LH_LIB6_LOADER_H

#include <LH/Lib6.h>

namespace LH {
namespace Lib6 {

class Object;

typedef void ( * loader_function )( Object & parent );

class Loader
{
    loader_function const fn_;
    const Loader * next_;
public:
    static const Loader *chain( Loader * new_loader = 0 );
    explicit Loader( loader_function function ) :
        fn_( function ),
        next_( chain(this) )
    {}
    void invoke( Object & parent ) const { fn_( parent ); }
    const Loader *next() const { return next_; }
};

} // namespace Lib6
} // namespace LH

#endif // LH_LIB6_LOADER_H
