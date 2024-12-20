#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"
#include <time.h>


BUFFER_ITEM buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

int insert_item(BUFFER_ITEM item){
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
    	in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);

	return 0;

}

int remove_item(BUFFER_ITEM *item){
	sem_wait(&full);
    	pthread_mutex_lock(&mutex);

	*item = buffer[out];
    	out = (out + 1) % BUFFER_SIZE;

	pthread_mutex_unlock(&mutex);
        sem_post(&empty);

	return 0;

}


uint16_t checksum(char *addr, uint32_t count)
{
	register uint32_t sum = 0;
	uint16_t *buf = (uint16_t *) addr;
	// Main summing loop
	while(count > 1){
		sum = sum + *(buf)++;
		count = count - 2;
	}
	// Add left-over byte, if any
	if (count > 0){
		sum = sum + *addr;
	}
	// Fold 32-bit sum to 16 bits
	while(sum>>16){
	sum = (sum & 0xFFFF) + (sum >> 16);
	}
	return(~sum);
}

void *producer(void *param){
	BUFFER_ITEM item;
	while(true){
		sleep(rand() % 2);

		for(int i=0; i < 30 ; i++) {
			item.data[i] = rand() % 256;
		}

		item.cksum = checksum((char*)item.data, sizeof(item.data));

		if(insert_item(item) != 0){
			fprintf(stderr, "ERROR\n");
		}else{
			printf("producer produced %d\n", item.cksum);
		}
	}
	return NULL;
}

void *consumer(void *param){
     	BUFFER_ITEM item;
	while(true){
                sleep(rand() % 2);

                if(remove_item(&item)!= 0){
                        fprintf(stderr, "ERROR\n");
                }else{
			uint16_t concksum = checksum((char *)item.data, sizeof(item.data));
			if(concksum == item.cksum){
                        	printf("consumer consumed %d\n", concksum);
        		} else {
				printf("error\n");
			}
		}
	}
	return NULL;
}

void init_buffer(){

	sem_init(&empty, 0, BUFFER_SIZE);
        sem_init(&full, 0, 0);
        pthread_mutex_init(&mutex,NULL);
}

int main(int argc, char *argv[]){

	srand(time(NULL));
	int delay, numProdThreads, numConThreads;
	delay = atoi(argv[1]);
	numProdThreads = atoi(argv[2]);
	numConThreads = atoi(argv[3]);
	int res;
	int res1;
	int res2;

	init_buffer();

	pthread_t prodthread[numProdThreads];
	for(int i = 0; i <numProdThreads; i++){
		res = pthread_create(&prodthread[i], NULL, producer, NULL);
		if(res != 0){
			perror("producer thread creation failed");
			exit(EXIT_FAILURE);
		}
	}

	pthread_t conthread[numConThreads];
        for(int i = 0; i <numConThreads; i++){
                res = pthread_create(&conthread[i], NULL, consumer, NULL);
                if(res != 0){
                        perror("consumer thread creation failed");
                        exit(EXIT_FAILURE);
                }
        }

	sleep(delay);

	for (int i = 0; i < numProdThreads; i++) {
            pthread_cancel(prodthread[i]);
        }
	for (int i = 0; i < numConThreads; i++) {
            pthread_cancel(conthread[i]);
        }

	for (int i = 0; i < numProdThreads; i++) {
	    res1 = pthread_join(prodthread[i], NULL);
	    if (res1 != 0) {
	        perror("Producer thread join failed");
	        exit(EXIT_FAILURE);
	    }
	}

	for (int i = 0; i < numConThreads; i++) {
	    res2 = pthread_join(conthread[i], NULL);
	    if (res2 != 0) {
	        perror("Consumer thread join failed");
	        exit(EXIT_FAILURE);
	    }
	}



	exit(0);

}
