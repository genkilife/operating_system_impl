struct stat;
struct rtcdate;
struct thread_spinlock;
struct thread_mutex;
struct thread_cond;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
int memtop(void);
int getmeminfo(void);
int thread_create(void*, void*, void*);
int thread_join(void);
int thread_exit(void);
int sleep_and_release_mutex(void*, void*);
int user_cond_wakeup(void*);

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);

// user function

// Spinlock
void thread_spin_init(struct thread_spinlock *lk);
void thread_spin_lock(struct thread_spinlock *lk);
void thread_spin_unlock(struct thread_spinlock *lk);

// Mutex
void thread_mutex_init(struct thread_mutex *lk);
void thread_mutex_lock(struct thread_mutex *lk);
void thread_mutex_unlock(struct thread_mutex *lk);

uint thread_xchg(volatile uint *addr, uint newval);
int thread_holding(struct thread_spinlock *lock);

// Condition variable
void thread_cond_init(struct thread_cond*);
void thread_cond_signal(struct thread_cond*);
void thread_cond_wait(struct thread_cond*, struct thread_mutex*);
