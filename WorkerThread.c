#include "WorkerThread.h"
#include <stdio.h>

int workerThreadInit(struct WorkerThread* thread, int index)
{
    thread->evLoop = NULL;
    thread->threadID = 0;
    sprintf(thread->name, "SubThread-%d", index);
    pthread_mutex_init(&thread->mutex, NULL);
    pthread_cond_init(&thread->cond, NULL);
    return 0;
}

// 子线程的回调函数,实例化一个eventLoop并让它运行
void* subThreadRunning(void* arg)
{
    struct WorkerThread* thread = (struct WorkerThread*)arg;
    pthread_mutex_lock(&thread->mutex);
    thread->evLoop = eventLoopInitEx(thread->name);
    pthread_mutex_unlock(&thread->mutex);
    pthread_cond_signal(&thread->cond);
    eventLoopRun(thread->evLoop);
    return NULL;
}

void workerThreadRun(struct WorkerThread* thread)
{
    // 创建子线程
    pthread_create(&thread->threadID, NULL, subThreadRunning, thread);
    // 当pthread_create创建完子线程后，如果eventLoop还没有初始化完，那么当主线程想要向eventloop里添加事件就会失败。所以需要阻塞一下。
    // 阻塞主线程, 让当前函数不会直接结束
    pthread_mutex_lock(&thread->mutex);
    while (thread->evLoop == NULL)
    {
        pthread_cond_wait(&thread->cond, &thread->mutex);
    }
    pthread_mutex_unlock(&thread->mutex);
}
