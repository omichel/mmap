# Memory Mapped File

This project aims at benchmarking the performance of memory mapped files across various systems (native Linux, Docker container, etc.)

## Compile

```
gcc -Wall server.c -o server
gcc -Wall client.c -o client
```

## Run locally

From a terminal, launch the server:
```
./server
```

From another terminal, time the client:
```
time ./client
```

The performance should be displayed (I am getting about 7.4 seconds on my laptop running Linux in VMware).

## Run in Docker

```
cd client
docker build -t client .
cd ..
cd server
docker build -t server .
cd ..
docker run -v mmap:/tmp/mmap server ./server &
time docker run -v mmap:/tmp/mmap client ./client
```

The performance is now 8.1 seconds on the same configuration.
