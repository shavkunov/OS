#include <thread.h>
#include <buddy.h>
#include <print.h>

struct threadFrame  {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t rbx;
    uint64_t rbp;
  
    uint64_t addr;
} __attribute__((packed));

typedef struct threadFrame frame;

extern void __origin(void);
extern void switch_threads(void** old_sp, void* new_sp);

thread* currentThread;
thread mainThread;

void initThreads() {
    currentThread = &mainThread;

    currentThread->stackStart = NULL;
    currentThread->stackPointer = NULL;
}

void execute(function f, void* arg) {
    f(arg);
    while (1);
}

thread* createThread(function f, void* arg) {
    lock();
    thread* nthread = (thread*) buddyAlloc(sizeof(thread));

    nthread->stackStart = buddyAlloc(THREAD_STACK_MAXSIZE);
    nthread->stackPointer = (void*) ((uint64_t)nthread->stackStart + THREAD_STACK_MAXSIZE);
    nthread->stackPointer = (void*) ((uint64_t)nthread->stackPointer - sizeof(frame));
    nthread->next = currentThread->next;
    currentThread->next = nthread;

    frame* frame = nthread->stackPointer;
    
    frame->r15 = (uint64_t) f;
    frame->r14 = (uint64_t) arg;
    frame->r13 = 0;
    frame->r12 = 0;
    frame->rbx = 0;
    frame->rbp = 0;
    frame->addr = (uint64_t)(&__origin);
    
    unlock();
    return nthread;
}

void lock() {
    __asm__("cli"); // disable interruptions
}

void unlock() {
    __asm__("sti"); // enable interruptions
}

void switcher() {
    switchThreads(currentThread->next);
}

void switchThreads(thread* t) {
    if (t == currentThread) {
        return;
    }

    thread* prev = currentThread;
    currentThread = t;
  
    switch_threads(&prev->stackPointer, t->stackPointer);
}

