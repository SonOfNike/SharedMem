#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Define the structure containing the array
struct SharedData {
    int array[10];
    int counter;
};

int main() {
    const char* shm_name = "/my_shared_memory";
    int shm_fd;
    size_t shm_size = sizeof(SharedData);
    SharedData* shared_data;

    // Create or open the shared memory object
    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
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
    shared_data = (SharedData*)mmap(0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // Initialize the shared data (e.g., in the first process)
    for (int i = 0; i < 10; ++i) {
        shared_data->array[i] = i * 2;
    }
    shared_data->counter = 0;

    // Example: Increment the counter in shared memory
    shared_data->counter++;
    std::cout << "Counter value: " << shared_data->counter << std::endl;

    // Unmap and close (optional, depending on use case)
    if (munmap(shared_data, shm_size) == -1) {
        perror("munmap");
        return 1;
    }
    close(shm_fd);

    // Remove the shared memory object (if this is the last process)
    // shm_unlink(shm_name);

    return 0;
}