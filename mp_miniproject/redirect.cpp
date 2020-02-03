#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_COUNT 10  // 10 child processes max per command line
int main() {
  static char line[80];
  static char * token;
  static int commands, pipecount;

  while (1) {
    int i;
    int fd[MAX_COUNT][2];
    int pid[MAX_COUNT];

    putchar('>');  // the shell prompt

    // wait for input -- will use a read() syscall on STDIN_FILENO
    readline(line);

    // parse command line, count commands, args and shell characters
    token = strtok(line, " ");  // repeat as necessary

    commands = ...;   // number of commands counted
    pipecount = ...;  // number of pipes

    /* Note: really need to know where pipes and shell characters are in the
 line input. */

    /* Create up to pipecount pipes */
    for (i = 0; i < pipecount; i++)
      pipe(fd[i]);

    for (i = 0; i < commands; i++) {
      if ((pid[i] = fork()) == 0) {  // child

        if (i == 0) {  // first command

          // redirect stdout to pipe input
          if (pipecount) {
            dup2(fd[i][1], STDOUT_FILENO);
            close(fd[i][0]);

            // Close all other unused pipe fds
          }
        }
        else if (i == commands - 1) {  // last command

          // redirect pipe output to stdin
          if (pipecount) {
            dup2(fd[i - 1][0], STDIN_FILENO);
            close(fd[i - 1][1]);

            // Close all other unused pipe fds
          }
        }
        else {  // nested command

          // redirect both stdout and stdin
          if (pipecount) {
            dup2(fd[i - 1][0], STDIN_FILENO);
            dup2(fd[i][1], STDOUT_FILENO);
            close(fd[i - 1][1]);
            close(fd[i][0]);

            // Close all other unused pipe fds
          }
        }

        // execute command in child -- don't close fds on exec
        execve(token, argv, envp);
      }

      // Here, you really need to be more careful about where in the
      // shell command sequence pipes are used
      pipecount--;

      if (!pipecount)
      // Close all pipe fds in parent
    }

    // Suspend shell while child processes are running
    // Only do this if not backgrounded command string
    for (i = 0; i < commands; i++)
      waitpid(pid[i]);
  }
  return 0;
}
