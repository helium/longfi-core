# LongFi Engine

## Building

1. Generate makefile in `build/`. You only need to do this once.
   ```
   cmake -H. -Bbuild
   ```
1. Compile
   ```
   make -C build
   ```
1. Test
   ```
   make -C build test
   ```
1. Documentation
   ```
   make -C build docs
   open build/docs/html/index.html
   ```
