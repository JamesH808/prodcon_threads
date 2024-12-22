# Producer-Consumer Simulation in C

This project implements a **Producer-Consumer Problem** simulation in C using **POSIX threads**, **semaphores**, and a **circular buffer**. The simulation features multiple producer and consumer threads interacting with a shared buffer, ensuring synchronization and mutual exclusion.

---

## Overview

The program uses:
- A **circular buffer** to store items produced by producer threads and consumed by consumer threads.
- **Mutex locks** and **semaphores** to prevent race conditions.
- **Checksum validation** to ensure data integrity between producers and consumers.

Each `BUFFER_ITEM` consists of:
1. `data[30]`: A 30-byte array of randomly generated values.
2. `cksum`: A checksum calculated over the `data` array.

---

## Key Features

1. **Multi-threaded Implementation**:
   - Multiple producer and consumer threads can run concurrently.
2. **Synchronization Mechanisms**:
   - **Mutex lock** ensures mutual exclusion while accessing the buffer.
   - **Semaphores** control buffer availability for producers and consumers.
3. **Data Validation**:
   - Consumer threads verify the integrity of consumed items using checksum.
4. **Graceful Termination**:
   - The program runs for a user-specified duration and safely terminates all threads afterward.

---

## Compilation and Execution

### Prerequisites
- GCC Compiler
- POSIX Thread Library (`pthread`)

### Compilation
```bash
gcc -pthread -o producer_consumer producer_consumer.c
```

### Execution
```bash
./producer_consumer <duration> <num_producers> <num_consumers>
```

- `<duration>`: Duration (in seconds) for the simulation to run.
- `<num_producers>`: Number of producer threads.
- `<num_consumers>`: Number of consumer threads.

---

## Code Design

### Core Components

1. **Circular Buffer**:
   - Fixed-size buffer (`BUFFER_SIZE = 10`).
   - Uses `in` and `out` pointers for insertion and removal.

2. **Synchronization**:
   - `mutex`: Protects buffer operations.
   - `empty` semaphore: Tracks available space for producers.
   - `full` semaphore: Tracks available items for consumers.

3. **Checksum Function**:
   - Calculates a 16-bit checksum for verifying data integrity.

4. **Producer**:
   - Generates random data for each `BUFFER_ITEM`.
   - Calculates and stores the checksum.
   - Adds the item to the buffer.

5. **Consumer**:
   - Retrieves `BUFFER_ITEM` from the buffer.
   - Validates the checksum and prints the result.

---

Sample Output:
[jaholt@WWWSTU-P01 assignment4]$ ./prodcon 1 2 2
producer produced 25290
consumer consumed 25290
producer produced 21900
producer produced 27777
producer produced 11883
producer produced 46021
producer produced 57183
producer produced 19040
producer produced 14630
consumer consumed 27777
consumer consumed 21900
producer produced 27436
consumer consumed 11883
consumer consumed 46021
consumer consumed 57183



- **Producer Output**: Indicates checksum of the produced item.
- **Consumer Output**: Validates and confirms checksum of consumed item.
- **Error**: Indicates checksum mismatch, a rare occurrence in this simulation.

---

## How It Works

1. **Initialization**:
   - Buffer and synchronization primitives (`sem_init`, `pthread_mutex_init`) are initialized.
   - Producer and consumer threads are created.

2. **Producer**:
   - Generates `BUFFER_ITEM` with random data.
   - Computes checksum and inserts the item into the buffer.

3. **Consumer**:
   - Retrieves `BUFFER_ITEM` from the buffer.
   - Validates the checksum against the producer's calculation.

4. **Termination**:
   - After the specified duration, threads are canceled.
   - Main thread waits for producer and consumer threads to finish using `pthread_join`.

5. **Synchronization**:
   - Semaphores ensure that producers wait when the buffer is full and consumers wait when the buffer is empty.
   - Mutex locks prevent concurrent access to the buffer.

---

This project demonstrates an efficient and synchronized multi-threaded implementation of the classic Producer-Consumer problem while ensuring data integrity.
