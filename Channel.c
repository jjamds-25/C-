#include "Channel.h"
#include <stdlib.h>

struct Channel* channelInit(int fd, int events, handleFunc readFunc, 
    handleFunc writeFunc, handleFunc destroyFunc, void* arg)
{
    struct Channel* channel = (struct Channel*)malloc(sizeof(struct Channel));
    channel->arg = arg;
    channel->fd = fd;
    channel->events = events;
    channel->readCallback = readFunc;
    channel->writeCallback = writeFunc;
    channel->destroyCallback = destroyFunc;
    return channel;
}

void writeEventEnable(struct Channel* channel, bool flag)
{
    if (flag)
    {
        // WriteEvent = 0x40;添加写事件，或就是添加，不管四号位是不是1，都置为1。
        //1 0 0 0 0 0 0 0 不写
        //      |
        //0 0 0 0 1 0 0 0
        //      =
        //1 0 0 0 1 0 0 0 写
        channel->events |= WriteEvent;
    }
    else
    {
        // 不添加写事件，
        //1 0 0 0 1 0 0 0 写
        //      &
        //1 1 1 1 0 1 1 1
        //      =
        //1 0 0 0 0 0 0 0 不写
        channel->events = channel->events & ~WriteEvent;
    }
}

bool isWriteEventEnable(struct Channel* channel)
{
    return channel->events & WriteEvent;
}
