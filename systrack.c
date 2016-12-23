#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/reg.h> // access to ORIG_RAX / RAX
#include <assert.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/syscall.h>

#include "src/utils.h"
#include "src/inputs.h"
#include "src/outputs.h"

int main (int argc, char **argv) {
    int i = 0;
    int retval = 0;
    
    retval = setup();
    assert(retval == 0);
    retval = 0;
    retval = load_syscall_data();
    assert(retval == 0);
    retval = 0;

    if (argc < 2) {
        fprintf(stdout, "usage summary:\n");
        print_opts();
        return EXIT_SUCCESS;
    } else {
        fprintf(stdout, "%s", "starting. process output follows:\n");
        retval = get_historical_avg(argc, argv);

        pid_t child = fork();
        if (child == -1) {
            log_issue(strerror(errno));
            return EXIT_FAILURE;
        } 
        if (child == 0) {
            int len = strlen(argv[0]) + 1;
            char progname[len];
            memset(progname, '\0', len);
            strncpy(progname, argv[1], len);
            progname[len] = '\0';

            for (i = 0; i < argc; ++i) {
                argv[i] = argv[i+1];
                if (argv[i] == NULL) {
                    argv[i] = NULL;
                    break;
                }
            }

            ptrace(PTRACE_TRACEME);
            raise(SIGSTOP);
            execvp(progname, argv);
        } else {
            int status = 0;
            long orig_rax = 0;
            waitpid(child, &status, 0);
            retval = ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);
            if (retval == -1) {
                fprintf(stdout, "-1 returned from PTRACE_SETOPTIONS, error: %s\n", strerror(errno));
            }
            while (1) {
                errno = 0;
                retval = ptrace(PTRACE_SYSCALL, child, 0, 0);
                if (retval == -1) {
                    // ESRCH = process gone, for when i forget in future
                    if (errno == ESRCH) {
                        break;
                    }
                    fprintf(stdout, "-1 returned from PTRACE_SYSCALL, error: %s\n", strerror(errno));
                }
                waitpid(child, &status, 0);
                if (WIFSTOPPED(status)) {
                    if (WSTOPSIG(status) & 0x80) {
                        errno = 0;
                        orig_rax = ptrace(PTRACE_PEEKUSER, child, sizeof(long) * ORIG_RAX); // as per reg.h, orig = 15
                        if (orig_rax == -1) {
                            log_issue("encountered a problem in getting ORIG_RAX; syscall value.");
                            return EXIT_FAILURE;
                        }
                        retval = increment_record_for_(orig_rax);
                        assert(retval == 0);
                        orig_rax = 0;
                    }
                }
            }
            call *tmp = head;
            fprintf(stdout, "\n* syscalls made by process:\n");
            int total = 0;
            while (tmp) {
                if (tmp->uses > 0) {
                    fprintf(stdout, "%s (%d): %d\n", tmp->cname, tmp->value, tmp->uses);
                    total += tmp->uses;
                }
                tmp = tmp->next;
            }
            data.total += total;
            data.runs += 1;
            fprintf(stdout, "average number of calls made for this command: %d, from %d runs\n", (data.total / data.runs), data.runs);
            retval = update_avg();
            assert(retval == 0);
            kill(child, SIGKILL);
        }
    }
    return EXIT_SUCCESS;
}
