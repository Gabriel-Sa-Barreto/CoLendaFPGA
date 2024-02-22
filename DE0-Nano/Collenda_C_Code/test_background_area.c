#include "stdio.h"
#include "io.h"
#include "sys/alt_stdio.h"
#include "altera_avalon_pio_regs.h"
#include "system.h"
#include "stdlib.h"
#include "time.h"
#include <unistd.h>
#include "lib_graphic_processor/graphic_processor.h"
#define A_BASE 0x11160
#define B_BASE 0x11130
#define X_BASE 0x11110
#define Y_BASE 0x11120
#define SELECT_BUTTON_BASE 0x110f0
#define START_BASE 0x11100
#define DIRECTION_ANALOGIC_BASE 0x11170
#define TL_BASE 0x11140
#define TR_BASE 0x11150
#define DATA_A_BASE 0x111d0
#define DATA_B_BASE 0x111c0
#define RESET_PULSECOUNTER_BASE 0x111b0
#define SCREEN_BASE 0x11180
#define WRFULL_BASE 0x11190
#define WRREG_BASE 0x111a0

/*-------------------------------------------------------------*/
/*---------------FUNCAO PRINCIPAL-------------------------*/
int main(){
	unsigned long dataA = 0x00000000;	// set background
	unsigned long dataB = 0x000000aa;
	if(IORD(WRFULL_BASE,0) == 0){                        //FIFO nao esta cheia
		IOWR_ALTERA_AVALON_PIO_DATA(WRREG_BASE,0);       //Desabilita o sinal de escrita
		IOWR_ALTERA_AVALON_PIO_DATA(DATA_A_BASE,dataA);  //Envia o dataA
		IOWR_ALTERA_AVALON_PIO_DATA(DATA_B_BASE,dataB);  //Envia o dataB
		IOWR_ALTERA_AVALON_PIO_DATA(WRREG_BASE,1);
		IOWR_ALTERA_AVALON_PIO_DATA(WRREG_BASE,0);
	}
	set_sprite(1, 100, 100, 0, 1);
	set_sprite(30, 100, 200, 3, 1);

	int opcode  = 2;
	int address = 0; 
	dataB = 0;
	while(address < 4800){
		if(IORD(WRFULL_BASE,0) == 0){                        //FIFO nao esta cheia
			dataA = 0;
			dataA = dataA | address;
			dataA = dataA << 4;
			dataA = dataA | opcode;
			if(dataB == 512){
				dataB = 0;
			}
			IOWR_ALTERA_AVALON_PIO_DATA(WRREG_BASE,0);       //Desabilita o sinal de escrita
			IOWR_ALTERA_AVALON_PIO_DATA(DATA_A_BASE,dataA);  //Envia o dataA
			IOWR_ALTERA_AVALON_PIO_DATA(DATA_B_BASE,dataB);  //Envia o dataB
			IOWR_ALTERA_AVALON_PIO_DATA(WRREG_BASE,1);
			IOWR_ALTERA_AVALON_PIO_DATA(WRREG_BASE,0);
			address++;
			dataB = dataB + 1;
		}
	}
	/*---------------------------------------------------------------*/
	return 0;
}
/*----------FIM DA FUNCAO PRINCIPAL--------------------------*/
