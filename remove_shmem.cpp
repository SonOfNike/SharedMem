#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "MDshmem.h"
#include "ReqShmem.h"


int main() {
    int shm_fd;
    size_t shm_size = sizeof(MDShmem);
    MDShmem* md_shmem;

    // Create or open the shared memory object
    shm_fd = shm_open(MD_shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    // Configure the size of the shared memory object
    if (ftruncate(shm_fd, shm_size) == -1) {
        perror("ftruncate");
        return 1;
    }

    // Map the shared memory object into the process's address space
    md_shmem = (MDShmem*)mmap(0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (md_shmem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // Initialize the shared data (e.g., in the first process)
    for (int i = 0; i < MD_QUEUE_SIZE; ++i) {
        md_shmem->array[i] = i * 2;
    }
    md_shmem->counter = 0;

    // Unmap and close (optional, depending on use case)
    if (munmap(shared_data, shm_size) == -1) {
        perror("munmap");
        return 1;
    }
    close(shm_fd);

    Remove the shared memory object (if this is the last process)
    shm_unlink(shm_name);

    return 0;
}