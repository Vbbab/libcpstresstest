/**
 * Quick and dirty implementation of a bi-directional popen() (a.k.a. "popen2")
 * Author: Vbbab
 */
#pragma once
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <libexplain/pipe.h>

struct popen2 {
    pid_t child_pid;
    int from_child, to_child;
};

int popen2(const char* cmdline, struct popen2* childInfo) {
    pid_t p;
    int pipe_stdin[2], pipe_stdout[2];

    if (pipe(pipe_stdin)) {
        cout << "[popen2] " << explain_pipe(pipe_stdin) << endl;
        return -1;
    }
    if (pipe(pipe_stdout))  {
        cout << "[popen2] " << explain_pipe(pipe_stdout) << endl;
    }
    p = fork(); // Create a clone of our process which we replace with the process to be tested
    if (p < 0) return p; // Failure
    if (!p) { // We are child
        close(pipe_stdin[1]); // We don't want *them* to control *our* stdin
        dup2(pipe_stdin[0], 0);
        close(pipe_stdout[0]); // We don't need to control *their* stdout
        dup2(pipe_stdout[1], 1);
        // Replace forked image:
        execl("/bin/bash", "bash", "-c", cmdline, NULL);
        perror("execl"); exit(99); // If execl() succeeded, we wouldn't have our code control anymore
    }

    childInfo->child_pid = p;
    childInfo->to_child = pipe_stdin[1];
    childInfo->from_child = pipe_stdout[0];
    close(pipe_stdin[0]);
    close(pipe_stdout[1]);
    return 0;
}