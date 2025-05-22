#pragma once

#include "../Utils/globals.h"
#include "../Utils/MDupdate.h"

struct MDShmem{
    int next_write_index = 0;
    MDupdate queue[MD_QUEUE_SIZE];
};