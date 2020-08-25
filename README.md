# Y86 SIMULATOR

This project was written in C++ for reviewing the CSAPP. (Maybe just for fun...)  

All of the tests in test/object directory should work.

## Compiling

### Prerequisites

To setup the environment, **You Need**:

- [GCC](https://gcc.gnu.org/releases.html) 5.0 or above
- [CMake](https://cmake.org/download) 3.1 or above

*Compiling is straight forward with CMAKE, just run CMAKE on the project directory with cmake, and you will get the Makefile.*

For example:

```zsh
% git clone ?
% cd Y86sim_review
% mkdir build
% cd build
% cmake ../src
% make -j4
```

## Running

Just add the path of the test after the excutable file(./test)

```zsh
% ./test ../test/object/ret-hazard.yo
```