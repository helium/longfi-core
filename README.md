![](https://github.com/helium/longfi-core/workflows/CI/badge.svg)

# LongFi Core

## Building

1. Generate makefile in `build/`. You only need to do this once.
   ```
   cmake -H. -Bbuild -DBUILD_TESTING=ON
   ```
1. Compile
   ```
   make -C build
   ```
1. Test (not applicable if configured with `-DBUILD_TESTING=OFF` in step 1)
   ```
   make -C build test
   ```
1. Documentation
   ```
   make -C build docs
   open build/docs/html/index.html
   ```
