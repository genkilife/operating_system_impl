#include "types.h"
#include "stat.h"
#include "user.h"
#include "usersync.h"

struct Queue {
   struct thread_cond cv;
   struct thread_mutex m;
   void *ptr;
} queue;

struct Send_args{
  struct Queue* q;
  void* p;
};

void* send(struct Send_args*);
void* recv(struct Queue *q);

int main(int argc, char *argv[]) {

  void *s1, *s2;
  int t1, t2, r1, r2;

  s1 = malloc(4096);
  s2 = malloc(4096);

  // Initialize
  thread_cond_init(&queue.cv);
  thread_mutex_init(&queue.m);
  queue.ptr = NULL;

  struct Send_args send_args;
  send_args.q = &queue;
  send_args.p = NULL;

  t2 = thread_create(recv, (void*)&queue, s2); 

  t1 = thread_create(send, (void*)&send_args, s1);

  printf(1, "send pid: %d, recv pid: %d\n", t1, t2);

  r1 = thread_join();
  r2 = thread_join();

  printf(1, "Threads finished: (%d):%d, (%d):%d\n", 
      t1, r1, t2, r2);
  exit();
}

// Thread 1 (sender)
void* send(struct Send_args* args)
{
   struct Queue *q = args->q;
   void *p = args->p;

   thread_mutex_lock(&q->m);
   while(q->ptr != 0);
   q->ptr = p;
   printf(1, "send: before signal condition variable\n");
   thread_cond_signal(&q->cv);
   thread_mutex_unlock(&q->m);
   return p;
}

// Thread 2 (receiver)

void* recv(struct Queue *q)
{
  void *p;

  thread_mutex_lock(&q->m);

  while((p = q->ptr) == 0){
    printf(1, "recv: before waiting condition variable\n");
    thread_cond_wait(&q->cv, &q->m);
    printf(1, "recv: after recieve condition variable\n");
  }
  q->ptr = 0;

  thread_mutex_unlock(&q->m);
  return p;
}
