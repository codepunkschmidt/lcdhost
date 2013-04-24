#ifndef LH_DECOR_H
#define LH_DECOR_H

#ifdef LH_DECOR_LIBRARY
# define LH_DECOR_EXPORT Q_DECL_EXPORT
#else
# define LH_DECOR_EXPORT Q_DECL_IMPORT
#endif

#endif // LH_DECOR_H
