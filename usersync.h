#ifndef __USERSYNC_H__
#define __USERSYNC_H__

#define NULL 0

struct thread_spinlock {
  uint locked;       // Is the lock held?
  // For debugging:
  char name[32];        // Name of lock.
};

#define MUTEX_INITIALIZER 0
struct thread_mutex {
  uint locked;       // Is the lock held?
  // For debugging:
  char name[32];        // Name of lock.
};

#define THREAD_COND_INITIALIZER 0
#define THREAD_COND_ACTIVE 1
struct thread_cond {
  uint cond;
};
#endif
