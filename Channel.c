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
        // WriteEvent = 0x40;���д�¼����������ӣ������ĺ�λ�ǲ���1������Ϊ1��
        //1 0 0 0 0 0 0 0 ��д
        //      |
        //0 0 0 0 1 0 0 0
        //      =
        //1 0 0 0 1 0 0 0 д
        channel->events |= WriteEvent;
    }
    else
    {
        // �����д�¼���
        //1 0 0 0 1 0 0 0 д
        //      &
        //1 1 1 1 0 1 1 1
        //      =
        //1 0 0 0 0 0 0 0 ��д
        channel->events = channel->events & ~WriteEvent;
    }
}

bool isWriteEventEnable(struct Channel* channel)
{
    return channel->events & WriteEvent;
}
