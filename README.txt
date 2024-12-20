Sample output:
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

Im not sure if the producer and consumer are supposed to have an order when they print but I just tried to make sure that whatever was produced got consumed, which I believe happened in this example.
One problem I had was the threads not exiting after joining them but after using pthread_cancel it worked. My adding to buffer functions use global vairbales in and out to access available array elements
to add random 8 bit values to the buffer. And Im printing out the values of the checksum in consumer and item.cksum in producer for better accuracy because those are the values being compared.
And if they don't match in the consumer side, I throw an error. 
