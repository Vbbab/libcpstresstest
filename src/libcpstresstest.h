#pragma once
#include <bits/stdc++.h>
using namespace std;
namespace cp {
    class StressTest {
    public:
        StressTest(string filename, function<string(string)> solution, function<string()> generator, long long iters, bool breakOnFail=false);
        StressTest() = delete;
        ~StressTest();
        void test();
    private:
        ofstream _f;
        function<string(string)> _s;
        function<string()> _g;
        string _fp, _ln;
        long long _cnt;
        bool _run(long long id);
        bool _failBrk;
    };
}