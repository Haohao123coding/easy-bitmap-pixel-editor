# Build Guide

How to build EBPE project?

EBPE uses CMake. You may configure options first like this:

```bash
cmake -B cmake-build -G "Ninja" -DCMAKE_BUILD_TYPE=Debug
```

You can build like this:

```bash
mkdir cmake-build
cd cmake-build
cmake --build . --target ebpe
```

For release builds, build type `Debug` -> `Release`.
