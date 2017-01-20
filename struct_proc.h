enum states { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

struct proc {
  enum states state;
  int pid;
  int sz;
  char name[16];
};
