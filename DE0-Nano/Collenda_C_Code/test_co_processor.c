#include "stdio.h"
#include "altera_avalon_pio_regs.h"
#include "system.h"
#include "stdlib.h"
#include "time.h"
#include "lib_graphic_processor/graphic_processor.h"
#define DATA_A_BASE 0x111d0
#define DATA_B_BASE 0x111c0
#define RESET_PULSECOUNTER_BASE 0x111b0
#define SCREEN_BASE 0x11180
#define WRFULL_BASE 0x11190
#define WRREG_BASE 0x111a0
/*
INSTRUCTION FORMAT:
	instruction[8:0]   = X do ponto base
	instruction[17:9]  = Y do ponto base  
	instruction[21:18] = Fator multiplicativo da base e altura do pol��gono.
	instruction[30:22] = Cor
	instruction[31]    = Forma 
*/
/*-------------------PROTOTIPO DAS FUNCOES-------------------------*/
/*-----------------------------------------------------------------*/
void wait(int limit);
int built_instruction(int address, int opcode, int color, int form, int mult, int ref_point_x, int ref_point_y);
int sendInstructionToProcessor(unsigned long dataA, unsigned long dataB);

int main(){
	unsigned long dataA = 0;
	unsigned long dataB = 0;
	int address = 0;
	int opcode  = 0b0011;
	int color   = 0;
	int form    = 0;
	int mult    = 0;
	int ref_point_x = 0;
	int ref_point_y = 0;

	Sprite tree_1;
	Sprite tree_2;
	Sprite tree_3;
	Sprite tree_4;

	Sprite rock_1;
	Sprite rock_2;
	Sprite rock_3;
	Sprite rock_4;

	Sprite tree_5;
	Sprite tree_6;
	Sprite tree_7;
	Sprite tree_8;

	Sprite rock_5;
	Sprite rock_6;
	Sprite rock_7;
	Sprite rock_8;

	tree_1.data_register	 = 1; tree_1.step_y  = 10;
	tree_2.data_register  	 = 2; tree_2.step_y  = 10;
	tree_3.data_register  	 = 3; tree_3.step_y  = 10;
	tree_4.data_register  	 = 4; tree_4.step_y  = 10;

	rock_1.data_register  	 = 5; rock_1.step_y  = 10;
	rock_2.data_register  	 = 6; rock_2.step_y  = 10;
	rock_3.data_register  	 = 7; rock_3.step_y  = 10;
	rock_4.data_register  	 = 8; rock_4.step_y  = 10;

	tree_1.coord_x  = 10; tree_1.coord_y  = 50;  tree_1.offset  = 7; tree_1.ativo  = 1; tree_1.direction  = DOWN;
	tree_2.coord_x  = 50; tree_2.coord_y  = 200; tree_2.offset  = 7; tree_2.ativo  = 1; tree_2.direction  = DOWN;
	
	rock_1.coord_x  = 20; rock_1.coord_y  = 450; rock_1.offset  = 6; rock_1.ativo  = 1; rock_1.direction  = DOWN;
	rock_2.coord_x  = 30; rock_2.coord_y  = 300; rock_2.offset  = 6; rock_2.ativo  = 1; rock_2.direction  = DOWN;

	tree_3.coord_x  = 580; tree_3.coord_y  = 50;  tree_3.offset  = 7; tree_3.ativo  = 1; tree_3.direction  = DOWN;
	tree_4.coord_x  = 600; tree_4.coord_y  = 200; tree_4.offset  = 7; tree_4.ativo  = 1; tree_4.direction  = DOWN;
	
	rock_3.coord_x  = 590; rock_3.coord_y  = 450; rock_3.offset  = 6; rock_3.ativo  = 1; rock_3.direction  = DOWN;
	rock_4.coord_x  = 570; rock_4.coord_y  = 300; rock_4.offset  = 6; rock_4.ativo  = 1; rock_4.direction  = DOWN;

	//tree_3.coord_x  = 20; tree_3.coord_y  = 50;  tree_3.offset  = 3; tree_3.ativo  = 1; tree_3.direction  = DOWN;
	//tree_4.coord_x  = 30; tree_4.coord_y  = 50;  tree_4.offset  = 3; tree_4.ativo  = 1; tree_4.direction  = DOWN;
		
	//rock_3.coord_x  = 10; rock_3.coord_y  = 150; rock_3.offset  = 1; rock_3.ativo  = 1; rock_3.direction  = DOWN;
	//rock_4.coord_x  = 10; rock_4.coord_y  = 150; rock_4.offset  = 1; rock_4.ativo  = 1; rock_4.direction  = DOWN;

	// Cor de Fundo Da Tela	
	set_background_color(0, 0b111, 0);
	/* ============================= Desenho do Carro ===================================== */
	/* =============================== Rodas de Cima ======================================= */
	address     = 14;
	ref_point_x = 298;
	ref_point_y = 190;
	mult        = 1;
	form        = 0;
	color       = 0b000111100;
	dataA = address;
	dataA = dataA << 4;
	dataA = dataA | opcode;
	dataB = built_instruction(address, opcode, color, form, mult, ref_point_x, ref_point_y);
	sendInstructionToProcessor(dataA, dataB);

	address 	= 13;
	ref_point_x = 362;
	ref_point_y = 190;
	mult        = 1;
	form        = 0;
	color       = 0b000111100;
	dataA = address;
	dataA = dataA << 4;
	dataA = dataA | opcode;
	dataB = built_instruction(address, opcode, color, form, mult, ref_point_x, ref_point_y);
	sendInstructionToProcessor(dataA, dataB);
	/* =======================================================================================*/
	/* =============================== Rodas de Baixo ======================================= */
	address     = 12;
	ref_point_x = 273;
	ref_point_y = 300;
	mult        = 2;
	form        = 0;
	color       = 0b000111100;
	dataA = address;
	dataA = dataA << 4;
	dataA = dataA | opcode;
	dataB = built_instruction(address, opcode, color, form, mult, ref_point_x, ref_point_y);
	sendInstructionToProcessor(dataA, dataB);

	address 	= 11;
	ref_point_x = 387;
	ref_point_y = 300;
	mult        = 2;
	form        = 0;
	color       = 0b000111100;
	dataA = address;
	dataA = dataA << 4;
	dataA = dataA | opcode;
	dataB = built_instruction(address, opcode, color, form, mult, ref_point_x, ref_point_y);
	sendInstructionToProcessor(dataA, dataB);
	/* =======================================================================================*/
	/* ===================================== Corpo ========================================== */
	address 	= 10;
	ref_point_x = 330;
	ref_point_y = 224;
	mult        = 6;
	form        = 1;
	color       = 0b111111111;
	dataA = address;
	dataA = dataA << 4;
	dataA = dataA | opcode;
	dataB = built_instruction(address, opcode, color, form, mult, ref_point_x, ref_point_y);
	sendInstructionToProcessor(dataA, dataB);

	address 	= 9;
	ref_point_x = 330;
	ref_point_y = 300;
	mult        = 7;
	form        = 0;
	color       = 0b111000000;
	dataA = address;
	dataA = dataA << 4;
	dataA = dataA | opcode;
	dataB = built_instruction(address, opcode, color, form, mult, ref_point_x, ref_point_y);
	sendInstructionToProcessor(dataA, dataB);
	
	address 	= 8;
	ref_point_x = 330;
	ref_point_y = 235;
	mult        = 4;
	form        = 0;
	color       = 0b111000000;
	dataA = address;
	dataA = dataA << 4;
	dataA = dataA | opcode;
	dataB = built_instruction(address, opcode, color, form, mult, ref_point_x, ref_point_y);
	sendInstructionToProcessor(dataA, dataB);

	address 	= 7;
	ref_point_x = 330;
	ref_point_y = 190;
	mult        = 3;
	form        = 0;
	color       = 0b111000000;
	dataA = address;
	dataA = dataA << 4;
	dataA = dataA | opcode;
	dataB = built_instruction(address, opcode, color, form, mult, ref_point_x, ref_point_y);
	sendInstructionToProcessor(dataA, dataB);

	address 	= 6;
	ref_point_x = 330;
	ref_point_y = 300;
	mult        = 4;
	form        = 0;
	color       = 0b000000111;
	dataA = address;
	dataA = dataA << 4;
	dataA = dataA | opcode;
	dataB = built_instruction(address, opcode, color, form, mult, ref_point_x, ref_point_y);
	sendInstructionToProcessor(dataA, dataB);

	address 	= 5;
	ref_point_x = 330;
	ref_point_y = 260;
	mult        = 2;
	form        = 0;
	color       = 0b000000111;
	dataA = address;
	dataA = dataA << 4;
	dataA = dataA | opcode;
	dataB = built_instruction(address, opcode, color, form, mult, ref_point_x, ref_point_y);
	sendInstructionToProcessor(dataA, dataB);
	/* =======================================================================================*/
	/* ===================================== Bico =========================================== */
	address 	= 4;
	ref_point_x = 330;
	ref_point_y = 170;
	mult        = 3;
	form        = 1;
	color       = 0b111000000;
	dataA = address;
	dataA = dataA << 4;
	dataA = dataA | opcode;
	dataB = built_instruction(address, opcode, color, form, mult, ref_point_x, ref_point_y);
	sendInstructionToProcessor(dataA, dataB);	
	/* =======================================================================================*/
	/* ==================================== Centro =========================================== */
	address 	= 3;
	ref_point_x = 330;
	ref_point_y = 300;
	mult        = 3;
	form        = 1;
	color       = 0b111111111;
	dataA = address;
	dataA = dataA << 4;
	dataA = dataA | opcode;
	dataB = built_instruction(address, opcode, color, form, mult, ref_point_x, ref_point_y);
	sendInstructionToProcessor(dataA, dataB);	
	/* =======================================================================================*/
	/* =======================================================================================*/
	/* =============================== Construção da Pista ===================================*/
	int line   = 60;
	int column = 10;
	int R = 0b111;
	int G = 0b111;
	int B = 0b111;
	int i = 0;
	int inter_initial    = 0;
	int inter_initial_2  = 50;
	// Preenchimento do Meio da Pista
	for (int l = 0; l <= 59; l++)
	{
		for (int c = 12; c <= 67; c++)
		{
			while( set_background_block(c, l, 0b000, 0b000, 0b000) == 0 );		
		}		
	}
	while(1)
	{
		while(i < line)
		{
			while( set_sprite(tree_1.data_register, tree_1.coord_x, tree_1.coord_y, tree_1.offset, tree_1.ativo) == 0 );
			while( set_sprite(tree_2.data_register, tree_2.coord_x, tree_2.coord_y, tree_2.offset, tree_2.ativo) == 0 );
			while( set_sprite(tree_3.data_register, tree_3.coord_x, tree_3.coord_y, tree_3.offset, tree_3.ativo) == 0 );
			while( set_sprite(tree_4.data_register, tree_4.coord_x, tree_4.coord_y, tree_4.offset, tree_4.ativo) == 0 );
			while( set_sprite(rock_1.data_register, rock_1.coord_x, rock_1.coord_y, rock_1.offset, rock_1.ativo)  == 0 );
			while( set_sprite(rock_2.data_register, rock_2.coord_x, rock_2.coord_y, rock_2.offset, rock_2.ativo)  == 0 );
			while( set_sprite(rock_3.data_register, rock_3.coord_x, rock_3.coord_y, rock_3.offset, rock_3.ativo)  == 0 );
			while( set_sprite(rock_4.data_register, rock_4.coord_x, rock_4.coord_y, rock_4.offset, rock_4.ativo)  == 0 );
			
			while( set_background_block(column, i, R, G, B) == 0 );  
			while( set_background_block((column + 58), i, R, G, B) == 0 );

			while( set_background_block((column + 1), i, R, G, B) == 0 );  
			while( set_background_block((column + 59), i, R, G, B) == 0 );
			// Preenchimento da linha divisória da pista
			if(i >= inter_initial && i <= inter_initial + 15 )
			{
				// Lista 1 central da pista
				while( set_background_block(40, i, 0b111, 0b111, 0b111) == 0 );
				while( set_background_block(41, i, 0b111, 0b111, 0b111) == 0 );
			}
			else if(i >= inter_initial_2 && i <= inter_initial_2 + 15 )
			{
				// Lista 2 central da pista
				while( set_background_block(40, i, 0b111, 0b111, 0b111) == 0 );
				while( set_background_block(41, i, 0b111, 0b111, 0b111) == 0 );
			}
			else
			{
				while( set_background_block(40, i, 0b000, 0b000, 0b000) == 0 );
				while( set_background_block(41, i, 0b000, 0b000, 0b000) == 0 );
			}
			i++;

			if(i % 3 == 0)
			{
				if(R == 0b111 && G == 0b111 && B == 0b111)
				{
					R = 0b111;
					G = 0b000;
					B = 0b000;	
				}
				else
				{
					R = 0b111;
					G = 0b111;
					B = 0b111;
				}
			}
		}
		increase_coordinate(&tree_1, 1);
		increase_coordinate(&tree_2, 1);
		increase_coordinate(&tree_3, 1);
		increase_coordinate(&tree_4, 1);
		increase_coordinate(&rock_1, 1);
		increase_coordinate(&rock_2, 1);
		increase_coordinate(&rock_3, 1);
		increase_coordinate(&rock_4, 1);

 		// Wait 2 seconds = 120 screens
 		wait(3);
 		i = 0;
 		inter_initial   +=5;
		inter_initial_2 +=5;
 		if(inter_initial > 80)
 		{
 			inter_initial = -10;
 		}
 		if(inter_initial_2 > 80)
 		{
 			inter_initial_2 = -10;
 		}
		if(R == 0b111 && G == 0b111 && B == 0b111)
		{
			R = 0b111;
			G = 0b000;
			B = 0b000;	
		}
		else
		{
			R = 0b111;
			G = 0b111;
			B = 0b111;
		}	
	}


	/* =======================================================================================*/
	/* =======================================================================================*/

	return 0;
}

int sendInstructionToProcessor(unsigned long dataA, unsigned long dataB){
	if(IORD(WRFULL_BASE,0) == 0){                        //FIFO nao esta cheia
		IOWR_ALTERA_AVALON_PIO_DATA(WRREG_BASE,0);       //Desabilita o sinal de escrita
		IOWR_ALTERA_AVALON_PIO_DATA(DATA_A_BASE,dataA);  //Envia o dataA
		IOWR_ALTERA_AVALON_PIO_DATA(DATA_B_BASE,dataB);  //Envia o dataB
		IOWR_ALTERA_AVALON_PIO_DATA(WRREG_BASE,1);
		IOWR_ALTERA_AVALON_PIO_DATA(WRREG_BASE,0);
		return 1;
	}else{
		return 0;
	}
}


int built_instruction(int address, int opcode, int color, int form, int mult, int ref_point_x, int ref_point_y)
{
	int inst = form;
	inst = inst << 9;
	inst = inst | color;
	inst = inst << 4;
	inst = inst | mult;
	inst = inst << 9;
	inst = inst | ref_point_y;
	inst = inst << 9;
	inst = inst | ref_point_x;

	return inst;
}

void wait(int limit){
	int screens = 0;
 	while(screens <= limit){ // Wait x seconds for restart Game 
		if(IORD(SCREEN_BASE,0) == 1){ // Checks if a screen has finished drawing.
			// Structure for counter of screen and set parameters.
			screens++;
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,1);
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,0);
		}
	}
}
