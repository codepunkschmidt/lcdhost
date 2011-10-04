#ifndef LH_API6_LH_LOADER_H
#define LH_API6_LH_LOADER_H

namespace lh {
namespace api6 {

class object;

typedef void ( * loader_function )( object & parent );

class loader
{
    loader_function const fn_;
    const loader * next_;
public:
    static const loader *chain( loader * new_loader = 0 );
    explicit loader( loader_function function ) :
        fn_( function ),
        next_( chain(this) )
    {}
    void invoke( object & parent ) const { fn_( parent ); }
    const loader *next() const { return next_; }
};

} // namespace api6
} // namespace lh

#endif // LH_API6_LH_LOADER_H
