#include "types.h"
#include "stat.h"
#include "user.h"

void
panic(char *s)
{
  printf(2, "panic: %s\n", s);
  exit();
}

int
index(char *s, char c)
{
  for(int i=0; i <= strlen(s); i++) {
    if (s[i] == c)
      return i;
  }
  return -1;
}

int
main()
{
  char buf[512];
  char *exec_cmd[512];
  int cp;
  int pid;

  while(1) {
    printf(1, " /)/)\n( 'x') ");

    gets(buf, 512);
    cp = index(buf, '\n');
    buf[cp] = '\0';

    if ((pid = fork()) < 0)
      panic("fork");

    if (pid == 0) {     // child
      *exec_cmd = buf;
      if (exec(*exec_cmd, exec_cmd) < 0)
        panic("exec");
    }
    else {      // parent
      wait();
    }
  }
}
