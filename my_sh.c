#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"

#define CMDLEN 64

struct cmd {
  char **argv;
};

int
my_sh_cd(struct cmd *cmd)
{
  char **args = cmd->argv;
  if (sizeof(args) < 2 || args[1] == NULL)
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

int
parse_cmd(char* buf, struct cmd *cmd)
{
  int n = strtok(buf, ' ', cmd->argv);

  //debug
  //for (int i = 0; i < sizeof(args); i++) {
  //  printf(1,"%d: %s\n",i,args[i]);
  //}

  return n;
}

int
exec_cmd(struct cmd *cmd, int num_args)
{
  int pid;
  char **args;
  
  if ((args = malloc(sizeof(cmd))) == NULL)
    panic("malloc");

  for (int i = 0; i < num_args; i++)
    args[i] = cmd->argv[i];

  if (strcmp(args[0],"cd") == 0)
    return my_sh_cd(cmd);

  if ((pid = fork()) < 0)
    panic("fork");

  if (pid == 0) {     // child
    if (exec(args[0], args) < 0)
      panic("exec");
  }
  else {              // parent
    wait();
  }

  free(args);
  return 0;
}

int
main()
{
  char buf[CMDLEN];
  struct cmd *cmd;
  int n;

  if ((cmd = malloc(sizeof(cmd))) == NULL)
    panic("malloc");

  while(1) {
    printf(1, " /)/)\n( 'x') ");

    get_cmd(buf);
    n = parse_cmd(buf, cmd);
    exec_cmd(cmd, n);
  }

  free(cmd);
  exit();
}
