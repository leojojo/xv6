#include "types.h"
#include "stat.h"
#include "user.h"

int
main() {
  char buf[512];
  int cc;

  while((cc = read(0, buf, 512)) > 0) {
    printf(1, buf);
  }
  exit();
}
