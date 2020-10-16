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
% git clone https://github.com/broodunivermonster/Y86sim_reveiw.git
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

## TODO

- [ ] Compelet the CmakeLists.txt
- [ ] Create a Front-end

## Contributors

- [Guanduo Chen](https://github.com/Woooooow-Pro/Y86sim_reveiw) - Fudan University

##### *PS:Once upon a time, I wanted to write the Y86-Simulator by myself. Till i saw [Hakula's Simulator](https://github.com/hakula139/Y86-64-Simulator). His work is sooo pretty, like any other projects write by a Tech Company. In that case, I decide to study his code and rewrite it.*
