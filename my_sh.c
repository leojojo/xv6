#include "types.h"
#include "stat.h"
#include "user.h"

void
getcmd(char* buf)
{
  int cp;

  gets(buf, 512);
  cp = index(buf, '\n');
  buf[cp] = '\0';
}

void
parsecmd(char* buf, char** args)
{
  *args = buf;
}

void
execcmd(char** args)
{
  int pid;

  if ((pid = fork()) < 0)
    panic("fork");

  if (pid == 0) {     // child
    if (exec(*args, args) < 0)
      panic("exec");
  }
  else {      // parent
    wait();
  }
}

int
main()
{
  char buf[512];
  char *args[512];

  while(1) {
    printf(1, " /)/)\n( 'x') ");

    getcmd(buf);
    parsecmd(buf, args);
    execcmd(args);
  }
}
