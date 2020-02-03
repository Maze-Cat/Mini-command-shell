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

#include "exec.cpp"
#include "parseBack.cpp"
#include "redir.cpp"
#include "setenv.cpp"

#define ARGS 100
/*
Prompt input,parse, and execute 
*/
void makeShell() {
  char * argv[ARGS]; /* the command line argument      */
  std::string line;
  char * buffer;
  std::map<std::string, std::string> ht;

  if ((buffer = getcwd(NULL, 0)) == NULL) {
    perror("getcwd error");
  }

  while (1) {
    //display the prompt
    std::cout << "myShell:" << buffer << "$";

    if (!std::getline(std::cin, line)) {
      std::cout << std::endl;
      free(buffer);
      ht.clear();
      exit(0);
    }
    if (line == "") {
      continue;
    }
    if (std::cin.eof()) {
      free(buffer);
      ht.clear();
      exit(0);
    }
    char * cstr = new char[line.length() + 1];

    parse(line, cstr, argv);
    if (strcmp(argv[0], "cd") == 0) {
      if (argv[1]) {
        int rc = chdir(argv[1]);
        if (rc != 0) {
          perror("chdir");
        }
        else {
          free(buffer);
          if ((buffer = getcwd(NULL, 0)) == NULL) {
            perror("getcwd error");
          }
        }
      }
    }

    //exit
    else if (strcmp(argv[0], "exit") == 0) {
      free(buffer);
      delete[] cstr;
      ht.clear();
      exit(0);
    }
    else if (strcmp(argv[0], "set") == 0 || strcmp(argv[0], "export") == 0 ||
             strcmp(argv[0], "inc") == 0) {
      changeVar(argv, ht);
    }
    else if (strchr(argv[0], '$')) {
      replaceVar(argv[0], ht);
    }
    else if (line.find("<") != line.npos || line.find(">") != line.npos ||
             line.find("2>") != line.npos) {
      redirect(line);
    }

    else {
      execute(line, argv);
    }
    delete[] cstr;
  }
  free(buffer);
}
int main() {
  makeShell();
  return EXIT_SUCCESS;
}
