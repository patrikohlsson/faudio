
#ifndef _FA_UTIL_ARCH
#define _FA_UTIL_ARCH

#if _WIN32 || _WIN64
#   if _WIN64
#       define FA_ARCH_BITS 64
#       define FA_ARCH_PTRSIZE 8
#   else
#       define FA_ARCH_BITS 32
#       define FA_ARCH_PTRSIZE 4
#   endif
#endif

// Check GCC
#if __APPLE__
#   if __x86_64__ || __ppc64__
#       define FA_ARCH_BITS 64
#       define FA_ARCH_PTRSIZE 8
#   else
#       define FA_ARCH_BITS 32
#       define FA_ARCH_PTRSIZE 4
#   endif
#endif

#endif // _FA_UTIL_ARCH

