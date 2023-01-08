#include "libcpstresstest.h"
#include "popen2.h"
#include <bits/stdc++.h>
#include <sys/wait.h>
#include <termcolor/termcolor.hpp>
using namespace std;
using namespace cp;


StressTest::StressTest(string filename, string solFilename, function<string()> generator, long long iters, bool breakOnFail) {
    _ln = filename + "_fails.log";
    _f = ofstream(_ln);
    _s = solFilename, _g = generator;
    _fp = filename;
    _cnt = iters;
    _failBrk = breakOnFail;
}

StressTest::~StressTest() {
    _f.flush();
    _f.close();
}

/**
 * Runs the given binary, returning a pair of {output, exit code}.
 */
pair<string, int> StressTest::runBin(string name, string input) {
    // popen2 the process
    struct popen2 child = {0};
    string cmdline = "\"";
    cmdline += realpath(_fp.c_str(), NULL);
    cmdline += "\"";
    if (popen2(cmdline.c_str(), &child)) {
        throw runtime_error("[cp::StressTest] unable to spawn subject process");
    }
    // Feed input
    write(child.to_child, input.c_str(), input.length() * sizeof(char));
    // Output buffer (read in 1024-byte chunks)
    char buf[1024];
    string out = "";

    int ec = 0; // Child exit code (not used)
    waitpid(child.child_pid, &ec, 0);
    if (ec) return {"", ec};
    // Read output
    int bytesRead = 0;
    while ((bytesRead = read(child.from_child, buf, 1024)) > 0) {
        for (int i = 0; i < bytesRead; i++) out += buf[i];
    }
    // Don't exceed linux open files limit
    close(child.from_child), close(child.to_child);
    // Trim
    int begin = out.find_first_not_of(" \n"), end = out.find_last_not_of(" \n");
    if (begin == string::npos || end == string::npos) return {out, 0};
    return {out.substr(begin, end - begin + 1), 0};
}

bool StressTest::_run(long long id) {
    string input = _g(); // Generate a testcase;
    input += "\n"; // If not already
    string solution = runBin(_s, input).first;
    pair<string, int> res = runBin(_fp, input);
    string output = res.first;
    if (res.second || output != solution) {
        _f << "CASE: " << id << "\nINPUT:\n" << input << "\nEXPECT:\n" << solution << "\nGOT:\n" << (res.second ? "!R:" + to_string(res.second) : output) << "\n====================\n";
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