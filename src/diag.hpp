#ifndef DIAG_HPP
#define DIAG_HPP

# ifdef __clang__
// this might not work on Mac OS X, their toolchain is really screwed up
#  define HAVE_GCC(major, minor, patch) (0)
#  define REALLY_HAVE_CLANG(major, minor, patch)                    \
    (__clang_major__ > (major)                                      \
            or (__clang_major__ == (major)                          \
                and (__clang_minor__ > (minor)                      \
                    or (__clang_minor__ == (minor)                  \
                        and __clang_patchlevel__ >= (patch)))))
# elif defined __GNUC__
#  define HAVE_GCC(major, minor, patch)                             \
    (__GNUC__ > (major)                                             \
            or (__GNUC__ == (major)                                 \
                and (__GNUC_MINOR__ > (minor)                       \
                    or (__GNUC_MINOR__ == (minor)                   \
                        and __GNUC_PATCHLEVEL__ >= (patch)))))
#  define HAVE_CLANG(major, minor, patch) (0)
# endif

# if HAVE_CLANG(0, 0, 0) or HAVE_GCC(4, 6, 0)
#  define PRAGMA(prag)      _Pragma(#prag)
#  define DIAG_PUSH()       PRAGMA(GCC diagnostic push)
#  if HAVE_CLANG(0, 0, 0)
// clang does not support the fine-grained -Wpragmas
#   define DIAG_IGN_PRAGMAS  DIAG_IGNORE(-Wunknown-pragmas)
#  else
#   define DIAG_IGN_PRAGMAS  DIAG_IGNORE(-Wpragmas)
#  endif
#  define DIAG_IGNORE(wo)   PRAGMA(GCC diagnostic ignored #wo)
#  define DIAG_WARN(wo)     PRAGMA(GCC diagnostic warning #wo)
#  define DIAG_ERROR(wo)    PRAGMA(GCC diagnostic error #wo)
#  define DIAG_POP()        PRAGMA(GCC diagnostic pop)
# else
# error "don't have recent clang or gcc"
#  define DIAG_PUSH()       /*nothing*/
#  define DIAG_IGNORE(wo)   /*nothing*/
#  define DIAG_WARN(wo)     /*nothing*/
#  define DIAG_ERROR(wo)    /*nothing*/
#  define DIAG_POP()        /*nothing*/
# endif

#endif //DIAG_HPP
