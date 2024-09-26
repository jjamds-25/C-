#pragma once
#include <stdbool.h>

// 定义函数指针
typedef int(*handleFunc)(void* arg);

// 定义文件描述符的读写事件
enum FDEvent
{
    TimeOut = 0x01,
    ReadEvent = 0x02,
    WriteEvent = 0x04
};

struct Channel
{
    // 文件描述符
    int fd;
    // 事件 ReadEvent or WriteEvent
    int events;
    // 回调函数
    // 读回调，如果监听到新的客户端连接到达，也就是events = ReadEvent触发了，那就需要调用读回调，如建立TcpConnection。
    handleFunc readCallback;
    handleFunc writeCallback;
    handleFunc destroyCallback;
    // 回调函数的参数
    void* arg;
};

// 初始化一个Channel
struct Channel* channelInit(int fd, int events, handleFunc readFunc, handleFunc writeFunc, handleFunc destroyFunc, void* arg);;
// 修改fd的写事件(检测 or 不检测)，可以设置该写事件被检测或不被检测。从而实现服务器被动发送数据？就是把channel->events改一下。
void writeEventEnable(struct Channel* channel, bool flag);
// 判断是否需要检测文件描述符的写事件
bool isWriteEventEnable(struct Channel* channel);