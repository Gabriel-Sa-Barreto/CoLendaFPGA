#include "stdio.h"
#include "io.h"
#include "sys/alt_stdio.h"
#include "altera_avalon_pio_regs.h"
#include "system.h"
#include <stdlib.h>
#include <time.h>
#define WRFULL_BASE 0x11070
#define WRREG_BASE  0x11080
#define DATA_A_BASE 0x110b0
#define DATA_B_BASE 0x110a0
#define RESET_PULSECOUNTER_BASE 0x11090
#define SCREEN_BASE 0x11060
#define MASX_TO_SHIFT_X 0b00011111111110000000000000000000


/*
1 - Implementar algoritmo para geração de valores aleatórios.
	- Sortear a posição dos sprites na tela, e o ângulo no qual ele irá se mover.
2 - Implementar o espelhamento da posição do sprite no momento em que ele sai da tela.
3 - Implementar colisão em software entre os sprites.
*/


/*-------Definição dos dados referentes aos sprites-------------------------------------------------------------------------*/
typedef struct Sprites{
	int coord_x          = 0;                                  //coordenada x atual do sprite na tela.
	int coord_y          = 0;                                  //coordenada y atual do sprite na tela.
	int direction        = 0;                                  //variável que define o ângulo de movimento do sprite.
	int offset           = 0;                                  //variável que definie o offset de memória do sprite. É usada para escolher qual animação usar.
	unsigned long mask_x = 0b00000000001010000000000000000000; //Número de passos que o sprite se movimenta no eixo X.
	unsigned long mask_Y = 0b00000000000000000000000000000000; //Número de passos que o sprite se movimenta no eixo Y.
	unsigned long inst_A = 0b00000000000000000000000010000000; //Variável que armazena os bits de instrução referente ao barramento dataA.
	unsigned long inst_B = 0b00000000000000000000000000000000; //Variável que armazena os bits de instrução referente ao barramento dataB.
} Asteroid;
/*---------------------------------------------------------------------------------------------------------------------------*/

/*-------------------MACRO DAS FUNÇÕES-------------------------*/
int sendInstruction(unsigned long dataA, unsigned long dataB);
unsigned long position_instruction_builder(int x, int y, int offset);
/*-----------------FIM DAS MACROS------------------------------*/


/*---------------FUNÇÂO PRINCIPAL-------------------------*/
int main(){
	int number_screen = 0; //Variável que conta o número de telas impressas.
	int result = 0;        //Variável que define se uma instrução foi enviada. (1 - Sim/ 0 - Não)
	int convert = 0;
	srand(time(NULL));
	
	Asteroid ast_1;

	/*----Definição dos dados iniciais de cada sprite-----*/
	ast_1.coord_x   = 1 + rand() % 639;     //sorteio da posição x.
	ast_1.coord_x   = 1 + rand() % 439;     //sorteio da posição y.
	ast_1.offset    = 0;                    //definição do offset de memória.
	ast_1.direction = rand() % 7;           //sorteio do ângulo inicial de movimento do sprite.
	ast_1.inst_B    = position_instruction_builder(ast_1.coord_x, ast_1.coord_y, ast_1.offset);
	/*----------------------------------------------------------------------------------------*/

	/*----------------------------LOOP PRINCIPAL-----------------------------*/
	while(1){
		if(IORD(SCREEN_BASE,0) == 1){
			number_screen++;
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,1);
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,0);
		}
		if(number_screen == 2){
			number_screen = 0;
			if(IORD(WRFULL_BASE,0) == 0){                           //FIFO não está cheia
				result = sendInstruction(ast_1.inst_A, ast_1.inst_B);       
				if(result == 1){
					break;		
				}
			}else{
				IOWR_ALTERA_AVALON_PIO_DATA(WRREG_BASE,0);         //Desabilita o sinal de escrita
				printf("[INFO] FILA CHEIA\n");
			}
		}
	}
	/*--------------------------FIM DO LOOP PRINCIPAL----------------------*/
}
/*----------FIM DA FUNÇÂO PRINCIPAL--------------------------*/


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

unsigned long position_instruction_builder(int x, int y, int offset){
	unsigned long data = 0; 
	data = data | 1;      //coloca o bit de ativação do sprite.
    data = data << 10;    //desloca o bit de ativação em 10 posições à esquerda.
    data = data | x;      //operação OR com a variável data. (insere x no barramento de bits)
    data = data << 10;    //desloca os bits em 10 posições à esquerda. 
    data = data | y;      //operação OR com a variável data. (insere y no barramento de bits)
    data = data << 9;     //desloca os bits em 9 posições à esquerda.
    data = data | offset; //insere o offset.
    return data;		
}
