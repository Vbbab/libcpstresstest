# libcpstresstest
Simple stress-testing library for cp

## Building
**NOTE:** This library is only meant to be used on Linux distributions! It has been tested on Ubuntu 20.04.
<!--
1. Install `libexplain` with headers (used for error logging):
```
sudo apt-get install libexplain-dev
```
-->
1. Build with CMake:
```sh
cd 'PATH_TO_LIBCPSTRESSTEST (the very same folder that contains this README!)'
mkdir build
cd build
cmake ..
make
```

2. If everything went successfully, a static `libcpstresstest.a` should be built that you can then link with your tests!

## Basic usage
```cpp
/** Example stress test for a program that adds two ints given from input
  * Author: Vbbab
  */
#include <libcpstresstest.h> // Obviously found in `include/`!
#include <bits/stdc++.h>     // g++-only header which is very useful in cp
using namespace std;
mt19937 gen(0x69420); // Nice seed :)
uniform_int_distribution<> dist(1, 100000); // We'll use this to generate testcases
/**
  * Generates an input testcase.
  * You can do whatever you want here, but just return a string that'll be passed into the program.
  * NOTE: this doesn't have to be terminated with a newline, libcpstresstest will automatically add a newline at the end. But you can do it if you want.
  */
string generator() {
  int a = dist(gen), b = dist(gen);
  return to_string(a) + " " + to_string(b);
}

/**
  * The 'model' solution. This takes in a string input and returns an output which will be used to judge the program's output.
  * NOTE: right now, only whitespace-stripped string matching is immplemented. Token comparisons and custom verifier functions will be implemented later.
  */
string solution(string input) {
  stringstream sin(input);
  ostringstream sout;
  int a, b;
  sin >> a >> b;
  sout << a + b << endl;
  return sout.str();
}

int main() {
  cp::StressTest s = cp::StressTest(/* name of binary to be tested: */ "add2ints", /* solution: */ solution, /* generator: */ generator, /* number of tests to run (long long): */ 100000, /* [OPTIONAL] break on fail? (default false): */ false);
  s.test(); // Begin testing! (Blocks execution, will print stuff)
  // Fail log is saved at binaryName_fails.log (i.e. add2ints_fails.log)
}
```
