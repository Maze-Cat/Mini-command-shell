#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>

#include "findPath.cpp"

/*
   Execute Function:
   Fork a child process, if success, execute commands in child process;
   Report status in parent process.
*/
void execute(std::string line, char ** argv) {
  pid_t pid, w;
  int wstatus;
  int pipefd[2];
  int ifpip = pipe(pipefd);

  if (ifpip == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  if ((pid = fork()) < 0) { /* fork a child process           */
    perror("fork");
    exit(EXIT_FAILURE);
  }
  else if (pid == 0) { /* for the child process:         */
    envcheck(line, argv);
  }

  else {
    do {
      w = waitpid(pid, &wstatus, WUNTRACED | WCONTINUED);
      if (w == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
      }
      if (WIFEXITED(wstatus)) {
        printf("Program exited with status %d\n", WEXITSTATUS(wstatus));
      }
      else if (WIFSIGNALED(wstatus)) {
        printf("Program was killed by signal %d\n", WTERMSIG(wstatus));
      }
    } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
  }
}

void execute(char ** argv) {
  int pipefd[2];
  pid_t cpid, w;
  char buf;
  int wstatus;

  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if ((cpid = fork()) < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  else if (cpid == 0) { /* Child reads from pipe */
    close(pipefd[1]);   /* Close unused write end */
    while (read(pipefd[0], &buf, 1) > 0)
      write(STDOUT_FILENO, &buf, 1);
    write(STDOUT_FILENO, "\n", 1);
    close(pipefd[0]);
    exit(EXIT_SUCCESS);
  }
  else {              /* Parent writes argv[1] to pipe */
    close(pipefd[0]); /* Close unused read end */
    write(pipefd[1], argv[1], strlen(argv[1]));
    close(pipefd[1]); /* Reader will see EOF */
    do {
      w = waitpid(cpid, &wstatus, WUNTRACED | WCONTINUED);
      if (w == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
      }
      if (WIFEXITED(wstatus)) {
        printf("Program exited with status %d\n", WEXITSTATUS(wstatus));
      }
      else if (WIFSIGNALED(wstatus)) {
        printf("Program was killed by signal %d\n", WTERMSIG(wstatus));
      }
    } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
  }
}
