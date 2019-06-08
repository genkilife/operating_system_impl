#ifndef __USERSYNC_H__
#define __USERSYNC_H__
struct thread_spinlock {
  uint locked;       // Is the lock held?
  // For debugging:
  char name[32];        // Name of lock.
};

struct thread_mutex {
  uint locked;       // Is the lock held?
  // For debugging:
  char name[32];        // Name of lock.
};

struct thread_cond {
  uint cond;
};
#endif
