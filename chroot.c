#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if(argc != 3){
    printf(2, "Usage: chroot [directory] [command]\n");
    exit();
  }

  if(chroot(argv[1]) < 0)
    printf(2, "chroot: failed to set %s as root\n", argv[1]);
  else
    printf(1, "chroot: succeeded in setting %s as root\n", argv[1]);

  if (exec(argv[2], &argv[2]) < 0)
    panic("exec");

  exit();
}
