This is a platformer game engine in the works. It uses [SDL 2.0](https://www.libsdl.org/) for rendering, and it's written in C++14 using [modern C++ practices](https://www.amazon.com/Effective-Modern-Specific-Ways-Improve/dp/1491903996?sa-no-redirect=1&pldnSite=1). This aims to implements an efficient collision detection system, side-scrolling platformer physics, etc.

#### Reusable Header Files

I'll try to list header files here that are independently useful, and can be reused in other projects:

* [**Matrix.hpp**](https://github.com/arjun-menon/Adventure/blob/master/src/Matrix.hpp) — this header file provides a `Matrix` class with support for iterators (that can be used in [range-based for loops](https://en.cppreference.com/w/cpp/language/range-for)), as well as a `SubMatrixRef` class which can create a light-weight reference to part of a `Matrix` and which provides similarly capable iterators. Both classes support pretty-printing to `ostream`. This header depends on [GeometricPrimitives.hpp](https://github.com/arjun-menon/Adventure/blob/master/src/GeometricPrimitives.hpp).
