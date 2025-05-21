#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

const char* shm_name = "/my_shared_memory";
const int shm_size = 1024;

int main(int argc, char* argv[]) {
    int shm_fd;
    void* shm_ptr;

    if (argc == 1) {
        // Parent process (writer)
        shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, shm_size);
        shm_ptr = mmap(0, shm_size, PROT_WRITE, MAP_SHARED, shm_fd, 0);
        close(shm_fd);

        const char* message = "Hello from parent!";
        strcpy((char*)shm_ptr, message);
        std::cout << "Parent wrote: " << message << std::endl;

        // Create child process
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            execv(argv[0], argv);
        } else {
            // Parent process waits for child
            wait(nullptr);

            // Unmap and unlink shared memory
            munmap(shm_ptr, shm_size);
            shm_unlink(shm_name);
        }
    } else {
        // Child process (reader)
        shm_fd = shm_open(shm_name, O_RDONLY, 0666);
        shm_ptr = mmap(0, shm_size, PROT_READ, MAP_SHARED, shm_fd, 0);
        close(shm_fd);

        std::cout << "Child read: " << (char*)shm_ptr << std::endl;
        munmap(shm_ptr, shm_size);
    }

    return 0;
}