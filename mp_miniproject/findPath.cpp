 #include <dirent.h>
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

//sperate path
void splitPath(char * line, char ** sp) {
  while (*line != '\0') {
    /* if not the end of line ....... */
    while (*line == ':') {
      *line++ = '\0'; /* replace white spaces with 0    */
    }
    *sp++ = line; /* save the argument position     */
    while (*line != '\0' && *line != ':') {
      line++; /* skip the argument until meet a space  */
    }
  }
  *sp = NULL; /* mark the end of argument list  */
}

std::string find(char * sp, const std::string str) {
  DIR * dir;
  struct dirent * ptr;
  std::string nothing = "";

  if ((dir = opendir(sp)) == NULL) {
    perror("Open dir error...");
    exit(1);
  }
  while ((ptr = readdir(dir)) != NULL) {
    std::string filename = ptr->d_name;
    //std::cout << filename << std::endl;
    /*
    if (ptr->d_type == DT_DIR) {
      ///link file
      const std::string oldDir(sp);
      std::string newDir = oldDir + filename;
      find(sp, newDir, argv);
    }
    */

    if (ptr->d_type == DT_REG) {
      //regular file
      //if found the file
      if (filename.compare(str) == 0) {
        const std::string oldDir(sp);
        std::string newDir = oldDir + "/" + filename;
        //  std::cout << newDir << std::endl;
        return newDir;
      }
    }
  }
  closedir(dir);
  return nothing;
}

void envcheck(std::string s, char ** argv) {
  //set current path to environment variable
  char * ECE551PATH = getenv("PATH");
  char * sp[64];
  setenv("PATH", ECE551PATH, 1);
  extern char ** environ;

  splitPath(ECE551PATH, sp);
  const char * line = s.c_str();
  if (!strchr(line, '/')) {
    //user type command without '/'
    //search each directory for the program
    //if(find program)
    //execute(argv);
    //else{
    //print("Command %s not found\n",argv);
    //}
    bool findFile = false;
    for (int i = 0; sp[i]; i++) {
      const std::string str(argv[0]);

      std::string f = find(sp[i], str);
      if (!f.empty()) {
        findFile = true;
        const char * c = f.c_str();
        //std::cout << "New Exec is " << f << std::endl;
        if (execve(c, argv, environ) < 0) {
          printf("ERROR:execve failed");
          exit(EXIT_FAILURE);
        }
      }
    }
    if (!findFile) {
      std::cout << "Command " << argv[0] << " not found" << std::endl;
    }
  }
  else {
    if (execve(argv[0], argv, environ) < 0) {
      printf("ERROR:execve failed");
      exit(EXIT_FAILURE);
    }
  }
}
