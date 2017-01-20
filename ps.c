#include "types.h"
#include "stat.h"
#include "user.h"
#include "struct_proc.h"

#define NPROC 64

int
main(int argc, char **argv) {
  struct proc ptable[NPROC];
  struct proc *p;
  int check;

  check = get_ps(NPROC, &ptable);
  if (check == -1) {
    printf(1, "Couldn't populate the ptable");
  }
  printf(1, "PID  SZ    STATE       CMD\n");
  p = &ptable[0];

  while (p != &ptable[NPROC - 1] && p->state != UNUSED) {
    printf(1, "%d   %d ", p->pid, p->sz);
    if (p->state == SLEEPING) {
      printf(1, "%s", "sleep");
    }
    if (p->state == RUNNING) {
      printf(1, "%s", "run");
    }
    printf(1, "        %s\n", p->name);
    p++;
  }
  exit();
}

