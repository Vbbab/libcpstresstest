#pragma once
#include <bits/stdc++.h>
using namespace std;
namespace cp {
    class StressTest {
    public:
        StressTest(string filename, string solFilename, function<string()> generator, long long iters, bool breakOnFail=false);
        StressTest() = delete;
        ~StressTest();
        void test();
        pair<string, int> runBin(string name, string input);
    private:
        ofstream _f;
        string _s;
        function<string()> _g;
        string _fp, _ln;
        long long _cnt;
        bool _run(long long id);
        bool _failBrk;
    };
}