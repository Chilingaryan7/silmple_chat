# Simple Multi-Client Chat Application in C

This project implements a simple multi-client chat application using TCP sockets and POSIX threads in C. It consists of a server that can handle multiple clients and clients that can send/receive messages concurrently.

---

##  Features

- Multi-threaded server supporting up to 16 simultaneous clients.
- Each client runs two threads: one for reading user input and sending messages, and another for receiving messages.
- Messages sent by a client are broadcast to all other connected clients.
- Clients can disconnect gracefully using the `exit` command.

---
 Requirements
	•	GCC compiler
	•	Linux/Unix environment
	•	POSIX threads (pthread)
	•	Basic understanding of C sockets

##  File Structure

- `server.c` – Implements the server logic.
- `clients.c` – Implements the client logic.

---

## ⚙ Compilation

Use `gcc` to compile the server and client:

```bash
gcc -Wall -pthread server.c -o server
gcc -Wall -pthread clients.c -o client


./server <port>


./client <port>
