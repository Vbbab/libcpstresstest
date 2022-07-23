#include "libcpstresstest.h"
#include "popen2.h"
#include <bits/stdc++.h>
#include <sys/wait.h>
#include <termcolor/termcolor.hpp>
using namespace std;
using namespace cp;


StressTest::StressTest(string filename, function<string(string)> solution, function<string()> generator, long long iters, bool breakOnFail) {
    _ln = filename + "_fails.log";
    _f = ofstream(_ln);
    _s = solution, _g = generator;
    _fp = filename;
    _cnt = iters;
    _failBrk = breakOnFail;
}

StressTest::~StressTest() {
    _f.flush();
    _f.close();
}

bool StressTest::_run(long long id) {
    string input = _g(); // Generate a testcase;
    input += "\n"; // If not already
    string solution = _s(input);
    // Trim a bit
    size_t begin = solution.find_first_not_of(" \n"), end = solution.find_last_not_of(" \n");
    solution = solution.substr(begin, end - begin + 1);

    // popen2 the process
    struct popen2 child = {0};
    if (popen2(realpath(_fp.c_str(), NULL), &child)) {
        throw runtime_error("[cp::StressTest] unable to spawn subject process");
    }
    // Feed that over
    write(child.to_child, input.c_str(), input.length() * sizeof(char));
    // Read stuff over in 1-KiB chunks
    char buf[1024];
    string output = "";
    int exitCode = 0; // Unused
    waitpid(child.child_pid, &exitCode, 0);
    // Read in 1024-byte chunks
    int bytesRead = 0;
    while ((bytesRead = read(child.from_child, buf, 1024)) > 0) {
        for(int i = 0; i < bytesRead; i++) output += buf[i];
    }
    // Don't make our pipes overflow (!!!)
    close(child.to_child);
    close(child.from_child);
    // Finally do a bit of trimming
    begin = output.find_first_not_of(" \n"), end = output.find_last_not_of(" \n");
    output = output.substr(begin, end - begin + 1);
    if (output != solution) {
        _f << "CASE: " << id << "\nINPUT:\n" << input << "\nEXPECT:\n" << solution << "\nGOT:\n" << output << "\n====================\n";
        return false;
    }
    return true;
}

void StressTest::test() {
    long long fails = 0;
    for(long long i = 1; i <= _cnt; i++) {
        if (!_run(i)) fails++;
        cout << "[cp::StressTest] " << i << "/" << _cnt << " (" << i * 100 / _cnt << "%) F: ";
        if (fails) cout << termcolor::red;
        else cout << termcolor::green;
        cout << fails << (i != _cnt ? "\t\t\t\r" : "\n");
        cout << termcolor::reset;
        if (fails && _failBrk) {
            cout << "\n";
            break;
        }
    }
    cout << "[cp::StressTest] done, results written to " << _ln << endl;
}