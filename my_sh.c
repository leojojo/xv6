#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define CMDLEN 64
#define STDOUT_FILENO 1

static const char PROMPT[] = " /)/)\n( 'x') ";
struct cmd {
  char *redir;
  char **argv;
};

int
my_sh_cd(struct cmd *cmd)
{
  char **args = cmd->argv;
  if (sizeof(args) < 2 || !(args[1]))
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
  char **block = malloc(sizeof(char**));

  if ((strtok(buf, '>', block)) > 1) {
    cmd->redir = block[1];
  } else {
    cmd->redir = NULL;
  }

  int n = strtok(block[0], ' ', cmd->argv);

  //debug
  //for (int i = 0; i < sizeof(args); i++) {
  //  printf(1,"%d: %s\n",i,args[i]);
  //}

  return n;
}

int
exec_cmd(struct cmd *cmd, int num_args)
{
  int pid, out_fd;
  char **args;
  
  if (!(args = malloc(sizeof(cmd))))
    panic("malloc");

  for (int i = 0; i < num_args; i++)
    args[i] = cmd->argv[i];

  if (strlen(args[0]) == 0)
    return 0;
  if (strcmp(args[0],"exit") == 0)
    exit();
  if (strcmp(args[0],"cd") == 0)
    return my_sh_cd(cmd);

  if ((pid = fork()) < 0)
    panic("fork");

  if (pid == 0) {     // child
    if (cmd->redir) {
      if (close(STDOUT_FILENO))
        panic("close");
      if ((out_fd = open(cmd->redir, O_WRONLY|O_CREATE)) < 0)
        panic("open");
      if (exec(args[0], args) < 0)
        panic("exec");
      if (close(out_fd))
        panic("close");
      if (open("console", O_RDWR))
        panic("open: console");
    } else {
      if (exec(args[0], args) < 0)
        panic("exec");
    }
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

  if (!(cmd = malloc(sizeof(cmd))))
    panic("malloc");
  cmd->argv = NULL;
  cmd->redir = NULL;

  while(1) {
    write(1,PROMPT,sizeof(PROMPT));

    get_cmd(buf);
    n = parse_cmd(buf, cmd);
    exec_cmd(cmd, n);
  }

  free(cmd);
  exit();
}
