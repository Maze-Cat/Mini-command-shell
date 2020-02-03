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

void redirect(std::string line) {
  char * token;
  char * c = const_cast<char *>(line.c_str());
  char * tk[64];
  token = strtok(c, "<");  // repeat as necessary
  while (token != NULL) {
    *tk = token;
    token = strtok(NULL, "<>|");  // repeat as necessary
  }
  for (int i = 0; tk[i]; i++) {
    puts(tk[i]);
  }
}
