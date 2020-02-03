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
void parse2char(std::string line, char * res) {
  char * c = new char[line.length() + 1];
  char * cstart = c;
  std::strcpy(c, line.c_str());

  //if find multiple blanks
  bool findBlank = false;

  while (*c != '\0') {
    if (*c == '\\') {
      char * curr = c;
      curr++;
      if (*curr == ' ') {
        *res = ' ';
        res++;
        c++;
      }
      c++;
    }
    else if (*c == ' ' || *c == '\n' || *c == '\t') {
      if (!findBlank) {
        *res++ = '!';
      }
      findBlank = true;
      c++;
    }
    else {
      findBlank = false;
      *res = *c;
      res++;
      c++;
    }
  }
  *res = '\0';
  c = cstart;
  delete[] c;
}

void parse2arg(char * res, char ** argv) {
  *argv++ = res;
  char * cut = std::strchr(res, '!');
  while (cut) {
    *cut = '\0';
    cut++;
    *argv = cut;
    argv++;
    cut = std::strchr(cut, '!');
  }
  *argv = NULL;
}

void parse(std::string line, char * cstr, char ** argv) {
  parse2char(line, cstr);
  parse2arg(cstr, argv);
}
