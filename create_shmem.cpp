#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../Utils/MDShmem.h"
#include "../Utils/ReqShmem.h"
#include "../Utils/RespShmem.h"


int main() {
    // create MDShmem
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
    for(int i = 0; i < MD_QUEUE_SIZE; i++)
    {
        md_shmem->m_queue[i].m_bid_price = 0;
        md_shmem->m_queue[i].m_ask_price = 0;
        md_shmem->m_queue[i].m_bid_quant = 0;
        md_shmem->m_queue[i].m_ask_quant = 0;
        md_shmem->m_queue[i].m_timestamp = 0;
        md_shmem->m_queue[i].m_type = md_type::NONE;
    }
    md_shmem->next_write_index = 0;

    // Unmap and close (optional, depending on use case)
    if (munmap(md_shmem, shm_size) == -1) {
        perror("munmap");
        return 1;
    }
    close(shm_fd);

    // create ReqShmem
    shm_size = sizeof(ReqShmem);
    ReqShmem* req_shmem;

    // Create or open the shared memory object
    shm_fd = shm_open(REQ_shm_name, O_CREAT | O_RDWR, 0666);
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
    req_shmem = (ReqShmem*)mmap(0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (req_shmem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // Initialize the shared data (e.g., in the first process)
    for(int i = 0; i < REQ_QUEUE_SIZE; i++)
    {
        req_shmem->m_queue[i].m_order_id = 0;
        req_shmem->m_queue[i].m_req_price = 0;
        req_shmem->m_queue[i].m_order_quant = 0;
        req_shmem->m_queue[i].m_total_fills = 0;
        req_shmem->m_queue[i].m_type = req_type::NONE;
    }
    req_shmem->next_write_index = 0;

    // Unmap and close (optional, depending on use case)
    if (munmap(req_shmem, shm_size) == -1) {
        perror("munmap");
        return 1;
    }
    close(shm_fd);

    // create RespShmem
    shm_size = sizeof(RespShmem);
    RespShmem* resp_shmem;

    // Create or open the shared memory object
    shm_fd = shm_open(RESP_shm_name, O_CREAT | O_RDWR, 0666);
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
    resp_shmem = (RespShmem*)mmap(0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (resp_shmem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // Initialize the shared data (e.g., in the first process)
    for(int i = 0; i < RESP_QUEUE_SIZE; i++)
    {
        resp_shmem->m_queue[i].m_order_id = 0;
        resp_shmem->m_queue[i].m_resp_price = 0;
        resp_shmem->m_queue[i].m_resp_quant = 0;
        resp_shmem->m_queue[i].m_type = resp_type::NONE;
    }
    resp_shmem->next_write_index = 0;

    // Unmap and close (optional, depending on use case)
    if (munmap(resp_shmem, shm_size) == -1) {
        perror("munmap");
        return 1;
    }
    close(shm_fd);

    return 0;
}