#include "types.h"
#include "stat.h"
#include "user.h"

#define CMDLEN 512

int
my_sh_cd(char **args)
{
  if (sizeof(args) < 2)
    panic("cd needs argument");

  if (chdir(args[1]) != 0) {
    printf(2, "cd failed\n");
    return 1;
  }

  return 0;
}

void
get_cmd(char* buf)
{
  char* end_cmd;

  gets(buf, CMDLEN);
  end_cmd = strchr(buf, '\n');
  memset(end_cmd,'\0',1);
}

char**
parse_cmd(char* buf)
{
  char **args = malloc(CMDLEN * sizeof(char*));
  strtok(buf, ' ', args);

  //debug
  //for (int i = 0; i < sizeof(args); i++) {
  //  printf(1,"%d: %s\n",i,args[i]);
  //}

  return args;
}

int
exec_cmd(char** args)
{
  int pid;

  if (strcmp(args[0],"cd") == 0)
    return my_sh_cd(args);

  if ((pid = fork()) < 0)
    panic("fork");

  if (pid == 0) {     // child
    if (exec(args[0], args) < 0)
      panic("exec");
  }
  else {              // parent
    wait();
  }

  return 0;
}

int
main()
{
  char buf[CMDLEN];
  char **args = malloc(CMDLEN * sizeof(char*));

  while(1) {
    printf(1, " /)/)\n( 'x') ");

    get_cmd(buf);
    args = parse_cmd(buf);
    exec_cmd(args);
  }
}
