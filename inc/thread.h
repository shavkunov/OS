#ifndef __THREADS_H__
#define __THREADS_H__

#define THREAD_STACK_MAXSIZE 0x2000

struct structThread {
    void* stackStart;
    void* stackPointer;

    struct structThread* next;
};

typedef struct structThread thread;
typedef void (*function)(void*);

void initThreads();
void switcher();
void switchThreads(thread* other);
void lock();
void unlock();
thread* current();
thread* createThread(function f, void* arg);

#endif
