#ifndef BUFER_H
#define BUFFER_H
/* buffer.h */
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
typedef struct buffer_item {
	uint8_t  data[30];
	uint16_t cksum;
} BUFFER_ITEM;

#define BUFFER_SIZE 10
int insert_item(BUFFER_ITEM item);
int remove_item(BUFFER_ITEM *item);
uint16_t checksum(char *addr, uint32_t count);
void init_buffer();
sem_t empty;
sem_t full;
pthread_mutex_t mutex;

#endif 
