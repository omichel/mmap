// gcc -Wall client.c -o client

// run the server from a terminal: ./server
// time the client from another terminal: time ./client

#include <fcntl.h> // open
#include <stdio.h> // printf
#include <string.h> // memset
#include <sys/mman.h> // mmap
#include <unistd.h> // close

#include "../common.h"

int main(int argc, char *argv[]) {
  int fd = open(MMAP_FILENAME, O_RDWR, 0666);
  if (fd < 0) {
    perror("Failed to open the file");
    return 1;
  }
  char *memory = mmap(NULL, MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (memory < 0) {
    perror("Failed to map memory to file");
    return 1;
  }

  // we can now close the file
  if (close(fd) < 0) {
    perror("Failed to close the file");
    return 1;
  }

  for(int i = 0; i < MMAP_STEPS; i++) {
    while(memory[0] != 's'); // wait for server to be ready
    // check that '*' character were received
    char answer = (memory[MMAP_SIZE / 2 + 1] == '*') ? '#' : '?';
    // write to memory
    memset(memory + 1, answer, MMAP_SIZE - 1);
    // notify
    memset(memory, 'c', 1);  // client is done
  }

  // unmap the memory
  if (munmap(memory, MMAP_SIZE) < 0) {
    perror("Failed to unmap the file");
    return 1;
  }

  return 0;
}
