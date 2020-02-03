#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

void replaceVar(char * old, std::map<std::string, std::string> & ht) {
  //input var

  char * temp = new char[strlen(old)];
  int itemp = 0;
  char * a = old;

  for (int i = 0;; i++) {
    if ((*(a + i) >= '0' && *(a + i) <= '9') ||
        ((*(a + i) >= 'a' && *(a + i) <= 'z') || ((*(a + i) >= 'A') && (*(a + i) <= 'Z'))) ||
        *(a + i) == '_') {
      *(temp + itemp) = *(a + i);
      itemp++;
    }
    else {
      std::ostringstream ss;
      for (int j = 0; j < itemp; j++) {
        ss << temp[j];
      }
      std::map<std::string, std::string>::iterator it;
      if ((it = ht.find(ss.str())) != ht.end()) {
        std::cout << it->second;
      }

      itemp = 0;
      if (!*(a + i)) {
        break;
      }
      if (*(a + i) == '$') {
        continue;
      }
      std::cout << *(a + i);
    }
  }
  std::cout << std::endl;
  delete[] temp;
}
void changeVar(char ** argv, std::map<std::string, std::string> & ht) {
  int sw = 0;
  if (strcmp(argv[0], "set") == 0) {
    sw = 1;
  }
  else if (strcmp(argv[0], "export") == 0) {
    sw = 2;
  }
  else if (strcmp(argv[0], "inc") == 0) {
    sw = 3;
  }
  switch (sw) {
      //set var value
    case 1: {
      if (argv[1] && argv[2]) {
        //make sure variable name is legal
        //alphabet+number+_
        char * it = argv[1];
        while (*it) {
          if ((*it >= '0' && *it <= '9') || *it == '_' || (*it >= 'A' && *it <= 'Z') ||
              (*it >= 'a' && *it <= 'z')) {
          }
          else {
            perror("ERROR:illegal variable name");
            break;
          }
          it++;
        }
        const std::string n(argv[1]);
        const std::string v(argv[2]);
        ht[n] = v;
      }
      else {
        perror("ERROR:setVar--not enough arguments");
      }
      break;
    }
      //export var

    case 2: {
      std::map<std::string, std::string>::iterator it;
      if ((it = ht.find(argv[1])) != ht.end()) {
        std::string res = it->first + "=" + it->second;
        char * c = const_cast<char *>(res.c_str());
        if (argv[2] != getenv(argv[1])) {
          putenv(c);
        }
      }
      else {
        perror("ERROR:no variable to export");
      }

      break;
    }
    case 3: {
      std::map<std::string, std::string>::iterator it;
      //find if have key
      if ((it = ht.find(argv[1])) != ht.end()) {
        //check if value is decimal number
        std::ostringstream ss;
        char * value = const_cast<char *>(it->second.c_str());

        char * isnum;
        long converted = strtol(value, &isnum, 10);
        if (converted > 0) {
          converted++;
          ss << converted;
          it->second = ss.str();
        }
        else {
          ss << 1;
          it->second = ss.str();
        }
      }
      break;
    }
    default: {}
  }
}
