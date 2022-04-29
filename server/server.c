// gcc -Wall server.c -o server

#include <fcntl.h> // open
#include <stdio.h> // printf
#include <string.h> // memset
#include <sys/mman.h> // mmap
#include <unistd.h> // close

#include "../common.h"

int main(int argc, char *argv[]) {
  // create file
  int fd = open(MMAP_FILENAME, O_RDWR | O_CREAT, 0666);
  if (fd < 0) {
    perror("Failed to open the file");
    return 1;
  }

  // set size to file
  lseek(fd, MMAP_SIZE - 1, SEEK_SET);
  if (write(fd, "", 1) < 1) {
    perror("Failed to write to the file");
    return 1;
  }
  lseek(fd, 0, SEEK_SET);

  // map file to memory
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
    // write to memory
    memset(memory + 1, '*', MMAP_SIZE - 1);
    // notify
    memset(memory, 's', 1);  // server is done
    // wait for answer
    while(memory[0] != 'c'); // wait for client answer
  }

  // unmap the memory
  if (munmap(memory, MMAP_SIZE) < 0) {
    perror("Failed to unmap the file");
    return 1;
  }

  // delete the file
  if (unlink(MMAP_FILENAME) < 0) {
    perror("Failed to unlink the file");
    return 1;
  }

  return 0;
}
