#include "ChannelMap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ChannelMap* channelMapInit(int size)
{
    struct ChannelMap* map = (struct ChannelMap*)malloc(sizeof(struct ChannelMap));
    map->size = size;
    map->list = (struct Channel**)malloc(size * sizeof(struct Channel*));
    return map;
}

void ChannelMapClear(struct ChannelMap* map)
{
    if (map != NULL)
    {
        for (int i = 0; i < map->size; ++i)
        {
            if (map->list[i] != NULL)
            {
                free(map->list[i]);
            }
        }
        free(map->list);
        map->list = NULL;
    }
    map->size = 0;
}

bool makeMapRoom(struct ChannelMap* map, int newSize, int unitSize)
{
    if (map->size < newSize)
    {
        int curSize = map->size;
        // 容量每次扩大原来的一倍
        while (curSize < newSize)
        {
            curSize *= 2;
        }
        // 扩容 realloc
        struct Channel** temp = realloc(map->list, curSize * unitSize);
        if (temp == NULL)
        {
            return false;
        }
        map->list = temp;
        // 要从curSize - map->size处，也就是新增的那一块内存空间的头部开始，往后的元素初始化为0
        memset(&map->list[map->size], 0, (curSize - map->size) * unitSize);
        map->size = curSize;
    }
    return true;
}