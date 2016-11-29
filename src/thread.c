#include <thread.h>
#include <slab_allocator.h>
#include <interrupt.h>
#include <print.h>
#include <ints.h>

struct threadFrame  {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t rbx;
    uint64_t rbp;
    uint64_t addr;
} __attribute__((packed));

extern void origin(void);
extern void switch_threads(void** old_sp, void* new_sp);

struct slabAllocator* threadSlab;
struct slabAllocator*  stackSlab;
thread mainThread;
thread* currentThread;

void initThreads() {
    threadSlab = initSlab(sizeof(thread));
    stackSlab = initSlab(THREAD_STACK_SIZE);
    currentThread = &mainThread;

    currentThread->stackStart = NULL;
    currentThread->stackPointer = NULL;
    currentThread->stateFlag = STATE_RUN;
    currentThread->prev = currentThread;
    currentThread->next = currentThread;
}

void callSwitcher() {
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

uint32_t cnt = 0;

void lock() {
    disable_ints(); // disable interruptions
    cnt++;
}

void unlock() {
    cnt--;

    if (cnt == 0) {
        enable_ints(); // enable interruptions
    }
}

void execute(function f, void* arg) {
    enable_ints();

    f(arg);

    lock();
    currentThread->stateFlag = STATE_JOIN;
    unlock();

    while (1);
}

thread* threadCreate(function f, void* arg) {
    lock();
    thread* nthread = (thread*) slabAlloc(&threadSlab);

    nthread->stackStart = slabAlloc(&stackSlab);
    nthread->stackPointer = (void*) ((uint64_t) nthread->stackStart + THREAD_STACK_SIZE);
    nthread->stackPointer = (void*) ((uint64_t) nthread->stackPointer - sizeof(struct threadFrame));
    nthread->stateFlag = STATE_RUN;
    nthread->next = currentThread->next;
    nthread->prev = currentThread;
    nthread->prev->next = nthread;
    nthread->next->prev = nthread;

    struct threadFrame* frame = nthread->stackPointer;

    frame->r15 = (uint64_t) f;
    frame->r14 = (uint64_t) arg;
    frame->r13 = 0;
    frame->r12 = 0;
    frame->rbx = 0;
    frame->rbp = 0;
    frame->addr = (uint64_t) (&origin);

    unlock(); 
    return nthread;
}

void join(thread* t) {
    while (1) {
        lock();
        if (t->stateFlag != STATE_JOIN) {
            unlock();
            continue;
        }
        
        break;
    }

    t->prev->next = t->next;
    t->next->prev = t->prev;
    slabFree(&stackSlab, t->stackStart);
    slabFree(&threadSlab, t);
    unlock();
}
