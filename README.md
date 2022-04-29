# Memory Mapped File

This project aims at benchmarking the performance of memory mapped files across various systems (native Linux, Docker container, etc.).

It creates a memory mapped file of 200,000 bytes and fills it with `*` characters on server side.
The client checks that at least one `*` character was received and writes back `#` characters.
The server checks that at least one `#` character was received and repeats writing `*` characters up to 1 million times.

## Compile

```
gcc -Wall server.c -o server
gcc -Wall client.c -o client
```

## Run locally

From a terminal, launch the server:
```
mkdir /tmp/mmap
./server
```

From another terminal, time the client:
```
time ./client
```

The performance should be displayed (I am getting about 8.33 seconds on my laptop running Linux in VMware).

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

The performance is now 8.47 seconds on the same configuration.

## Conclusion

Memory mapped files are the fastest IPC between two processes and the overhead of running them across docker containers is negligible.
