#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int
main(int argc, char *argv[])
{
  if(argc > 1){
    printf(1, "mtop: doesn't need input argument\n");
    exit();
  }
  int avail_mem = memtop();
  printf(1, "mtop: available memory is %d\n", avail_mem);

  if(getmeminfo() != 0){
	printf(1, "Fail to get memory info\n");
  }
  exit();
}
