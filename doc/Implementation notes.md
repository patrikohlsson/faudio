
Implementation notes
====================

## Library organization

For C++ code we follow the conventions of the Boost libraries. There is a top namespace ::scl in which every declaration resides. Inside this namespace, there are subnamespaces corresponding to functionality. 

Code directly related to the audio processing resides in the ::scl::audio namespace hierarchy, while generic and utility code reside in their own namespaces. Some of these namespaces are simply alisases for standard library or third-party implementations, for example `scl::thread` is an alias for `std` if supported or `boost` otherwise.

## C++0x support

The Audio Engine requires a relatively complete implementation of the C++0x standard (see supported compilers). 

### Missing standard library features

On some compilers third-party libraries (i.e. Boost) are used in place of the unsupported standard libraries. These can be replaced by the standard library when all compilers have implemented it. The `scl/thread` and `scl/atomic` directories are candidates for replacement (see above).

### Concepts

C++0x does not support concepts, although these are needed for serious template programming. We use Boost Concept Check as a rough replacement. If proper templates are ever added to the C++ language, the relevant concept checking code could be replaced by proper definitions. All concept checks reside in `scl/concept` and `scl/audio/concept`.

## Including and linking

Linking C++ based libraries is badly supported on most operating system: instead we use the header only approach. The C++ library is header only, while the C library is contains precisely the C++ library headers and some C wrapper code. 

Building a C++ application based on the library amounts to including the right headers, add application-specific code, and defining a suitable main function. Building a C application amounts to including the C headers, adding application-specific code and a main function, and linking (statically or dynamically) with the C library. 

Building an application in another language is similar to building a C application, except that the relevant language binding will be used in place of the C headers.
