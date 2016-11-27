#ifndef __THREADS_H__
#define __THREADS_H__

#include <stdint.h>

#define THREAD_STACK_SIZE 0x2000

#define STATE_RUN 0
#define STATE_JOIN 1

struct Thread {
    void* stackStart;
    void* stackPointer;

    uint32_t stateFlag;
    struct Thread* prev;
    struct Thread* next;
    uint32_t cnt; // locks amount
};

typedef struct Thread thread;
typedef void (*function)(void*);

void initThreads();
void callSwitcher();
void switchThreads(thread* t);
void lock();
void unlock();
void join(thread* t);
thread* threadCreate(function f, void* arg);

#endif

