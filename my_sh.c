#include "types.h"
#include "stat.h"
#include "user.h"

#define CMDLEN 512

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
  char **tokens = malloc(CMDLEN * sizeof(char*));
  strtok(buf, ' ', tokens);

  //for (int i = 0; i < sizeof(tokens); i++) {
  //  printf(1,"%d: %s\n",i,tokens[i]);
  //}

  return tokens;
}

void
exec_cmd(char** tokens)
{
  int pid;

  if ((pid = fork()) < 0)
    panic("fork");

  if (pid == 0) {     // child
    if (exec(tokens[0], tokens) < 0)
      panic("exec");
  }
  else {              // parent
    wait();
  }
}

int
main()
{
  char buf[CMDLEN];
  char **tokens = malloc(CMDLEN * sizeof(char*));

  while(1) {
    printf(1, " /)/)\n( 'x') ");

    get_cmd(buf);
    tokens = parse_cmd(buf);
    exec_cmd(tokens);
  }
}
