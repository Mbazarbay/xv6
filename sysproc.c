#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// this is for the halt system call
int
sys_halt(void)
{
  const char s[] = "Shutdown";
  const char *p;

  outw( 0xB004, 0x0 | 0x2000 );

  for (p = s; *p != '\0'; p++)
    outb (0x8900, *p);

  return 0;
}

// this is for my_sysgetpid
int
sys_mygetpid(void)
{
  return proc->pid;
}

// ps
extern struct proc * proc_ptable(void); // copy from kernel to user
int sys_get_ps(void) {
  int n;
  char *buf;
  char *s;

  struct proc *p = '\0';
  if (argint(0, &n) < 0){
    return -1;
  }
  if (argptr(1, &buf, n) < 0){
    return -1;
  }
  s = buf;
  p = proc_ptable();

  while(buf + n > s && p->state != UNUSED){
    *(int *)s = p->state;
    s+=4;
    *(int *)s = p -> pid;
    s+=4;
    *(int *)s = p->sz;
    s+=4;
    memmove(s,p->name,16);
    s+=16;
    p++;
  }
  return 0;
}
