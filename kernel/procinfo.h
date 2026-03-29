enum my_procstate { MY_UNUSED, MY_USED, MY_SLEEPING, MY_RUNNABLE, MY_RUNNING, MY_ZOMBIE };


struct procinfo {
  int pid;
  char name[16];
  enum my_procstate state;
  int parent;
};
