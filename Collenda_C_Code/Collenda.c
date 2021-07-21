#include "stdio.h"
#include <stdbool.h>
#include "io.h"
#include "sys/alt_stdio.h"
#include "altera_avalon_pio_regs.h"
#include "system.h"
#include <unistd.h>
#define MASK_X 0b00000000001010000000000000000000
#define MASK_Y 0b00000000000000000000011000000000
#define WRFULL_BASE 0x11070
#define WRREG_BASE  0x11080
#define DATA_A_BASE 0x110b0
#define DATA_B_BASE 0x110a0
#define RESET_PULSECOUNTER_BASE 0x11090
#define SCREEN_BASE 0x11060
#define MASX_TO_SHIFT_X 0b00011111111110000000000000000000


int sendInstruction(unsigned long dataA, unsigned long dataB){
	if(IORD(WRFULL_BASE,0) == 0){                           //FIFO não está cheia
		IOWR_ALTERA_AVALON_PIO_DATA(WRREG_BASE,0);          //Desabilita o sinal de escrita
		IOWR_ALTERA_AVALON_PIO_DATA(DATA_A_BASE,dataA);  //Envia o dataA
		IOWR_ALTERA_AVALON_PIO_DATA(DATA_B_BASE,dataB);  //Envia o dataB
		IOWR_ALTERA_AVALON_PIO_DATA(WRREG_BASE,1);
		IOWR_ALTERA_AVALON_PIO_DATA(WRREG_BASE,0);
		return 1;
	}else{
		return 0;
	}
}

int main(){
	unsigned long dataB_r0   = 0b00100110010000100101100000000000;
	unsigned long dataA_r0   = 0b00000000000000000000000010000000;

	unsigned long dataB_r1   = 0b00100011001000011001000000000000;
	unsigned long dataA_r1   = 0b00000000000000000000000000010000;

	unsigned long dataB_r2   = 0b00100011001000001100100000000010;
	unsigned long dataA_r2   = 0b00000000000000000000000000100000;

	unsigned long dataB_r3   = 0b00000000000000000000000100011010;
	unsigned long dataA_r3   = 0b00000000000000111111111111110001;

	unsigned long dataB_r4   = 0b00100011001000110010000000000001;
	unsigned long dataA_r4   = 0b00000000000000000000000000110000;
	int convert = 0;
	int result = 0;
	int number_screen = 0;
	int direction_r0  = 1;
	int direction_r1  = 1;
	int direction_r2  = 1;
	int direction_r4  = 1;
	while(1){
		if(IORD(SCREEN_BASE,0) == 1){
			number_screen++;
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,1);
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,0);
		}
		if(number_screen == 2){
			number_screen = 0;
			if(IORD(WRFULL_BASE,0) == 0){                           //FIFO não está cheia
				result = sendInstruction(dataA_r0, dataB_r0);   // Envia primeira instrução
				if(result == 1){
					convert = dataB_r0 & MASX_TO_SHIFT_X;
					convert = convert >> 19;
					//printf("[INFO] Convert %d\n", convert);
					if(convert > 610)  direction_r0 = 0;
					if(convert > 800)  direction_r0 = 1;
					if(direction_r0 == 1){
						//printf("[INFO] Dados de r0 Inseridos\n");
						dataB_r0 = dataB_r0 + MASK_X;
					}else{
						dataB_r0 = dataB_r0 - MASK_X; 
					}
				}
				
				result = sendInstruction(dataA_r1, dataB_r1);   // Envia segunda instrução
				if(result == 1){
					convert = dataB_r1 & MASX_TO_SHIFT_X;
					convert = convert >> 19;
					//printf("[INFO] Convert %d\n", convert);
					if(convert > 610)   direction_r1 = 0;
					if(convert > 800)   direction_r1 = 1;
					if(direction_r1 == 1){
						dataB_r1 = dataB_r1 + MASK_X;
					}else{
						dataB_r1 = dataB_r1 - MASK_X; 
					}
				}
				
				result = sendInstruction(dataA_r2, dataB_r2);   // Envia segunda instrução
				if(result == 1){
					convert = dataB_r2 & MASX_TO_SHIFT_X;
					convert = convert >> 19;
					//printf("[INFO] Convert %d\n", convert);
					if(convert > 610)   direction_r2 = 0;
					if(convert > 800)   direction_r2 = 1;
					if(direction_r2 == 1){
						dataB_r2 = dataB_r2 + MASK_X;
					}else{
						dataB_r2 = dataB_r2 - MASK_X; 
					}
				}
				
				result = sendInstruction(dataA_r4, dataB_r4);   // Envia segunda instrução
				if(result == 1){
					convert = dataB_r4 & MASX_TO_SHIFT_X;
					convert = convert >> 19;
					//printf("[INFO] Convert %d\n", convert);
					if(convert > 610)   direction_r4 = 0;
					if(convert > 800)   direction_r4 = 1;
					if(direction_r4 == 1){
						dataB_r4 = dataB_r4 + MASK_X;
					}else{
						dataB_r4 = dataB_r4 - MASK_X; 
					}
				}
				break;
				//result = sendInstruction(dataA_r3, dataB_r3);  // Envia quarta instrução
				//if(result == 1){
				//	printf("[INFO] Dados de r3 Inseridos\n");
				//}
			}else{
				IOWR_ALTERA_AVALON_PIO_DATA(WRREG_BASE,0);         //Desabilita o sinal de escrita
				printf("[INFO] FILA CHEIA\n");
			}
		}
	}
}

