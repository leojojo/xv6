#include "types.h"
#include "user.h"
#include "param.h"
#include "procinfo.h"

int
main(int argc, char *argv[])
{
  enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
  struct procinfo p[NPROC];
  int n;

  n = getproc(p);
  printf(1,"n: pid name\n");
  printf(1,"%d: %d %s\n",n, p->pid, p->name);
  exit();
}
