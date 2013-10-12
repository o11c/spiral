#ifndef PROPERTY_HPP
#define PROPERTY_HPP

#include "diag.hpp"
#include "coercion.hpp"

#include <cstddef>

// GCC gets confused by pragmas inside a macro
#if HAVE_GCC(0, 0, 0)
        DIAG_IGNORE(-Winvalid-offsetof)
#endif

#define IMPL_REF_PROPERTY(C, T, v)                                      \
        DIAG_PUSH()                                                     \
        DIAG_IGN_PRAGMAS                                                \
        DIAG_IGNORE(-Wunused-private-field)                             \
        T _phony[0];                                                    \
        DIAG_POP()                                                      \
        DIAG_PUSH()                                                     \
        DIAG_IGNORE(-Winvalid-offsetof)                                 \
        static size_t offset() { return offsetof(C, v); }               \
        DIAG_POP()                                                      \
        C *mut_this()                                                   \
        { return reinterpret_cast<C *>(reinterpret_cast<char *>(this) - offset()); } \
        const C *const_this() const                                     \
        { return reinterpret_cast<const C *>(reinterpret_cast<const char *>(this) - offset()); } \
        T& mut_ref()                                                    \
        { return const_cast<T&>(mut_this()->_ref_##v()); }              \
        const T& const_ref() const                                      \
        { return const_this()->_ref_##v(); }                            \
    public:

// A property that uses unary plus to overcome the coercion problem.
#define PROPERTY(C, T, v)                                               \
    class _property_##v : public coercion::operators<_property_##v, T>  \
    {                                                                   \
    IMPL_REF_PROPERTY(C, T, v)                                          \
        T& operator = (T r) { return mut_ref() = r; }                   \
        operator T() const { return const_ref(); }                      \
    } v;                                                                \
    const T& _ref_##v() const

#endif //PROPERTY_HPP
