# Build Guide

How to build EBPE project?

EBPE uses CMake. You can build like this:

```bash
cmake -B cmake-build-debug -G "Ninja" -DCMAKE_BUILD_TYPE=Debug
mkdir cmake-build-debug
cd cmake-build-debug
cmake --build . --target ebpe
```

For release builds, `Debug` -> `Release`, `cmake-build-release` -> `cmake-build-release`.
