#include <stdio.h>
#include "sys/alt_timestamp.h"
#include "alt_types.h"
#include <unistd.h>


//800 pulsos horizontais
//524 pulsos verticais
//total de 419,200.000 pulsos por tela == 419,200.000 x 20ns = 8.38ms

int main (void){
	alt_u32 time1;
	alt_u32 time2;
	alt_u32 time3;
	int a = 0;
	alt_timestamp_start();
	time1 = alt_timestamp();
	if(a == 0){
		time2 = alt_timestamp();
		printf("time1: %u\n", (unsigned int) (time1));
		printf("time2: %u\n", (unsigned int) (time2));
		printf("time-sub: %u\n", (unsigned int) (time2-time1));
	}
	alt_timestamp_start();
	time3 = alt_timestamp();
	printf("time3: %u\n", (unsigned int) (time3));
	printf("time frequency: %u", (unsigned int) alt_timestamp_freq());
}
