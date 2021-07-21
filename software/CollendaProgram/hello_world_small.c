#include "stdio.h"
#include "io.h"
#include "sys/alt_stdio.h"
#include "altera_avalon_pio_regs.h"
#include "system.h"
#include "stdlib.h"
#include "time.h"
#include <unistd.h>
#define WRFULL_BASE 0x11070
#define WRREG_BASE  0x11080
#define DATA_A_BASE 0x110b0
#define DATA_B_BASE 0x110a0
#define RESET_PULSECOUNTER_BASE 0x11090
#define SCREEN_BASE 0x11060
#define MASX_TO_SHIFT_X 0b00011111111110000000000000000000
#define MASX_TO_SHIFT_Y 0b00000000000001111111111000000000


/*
1 - Implementar algoritmo para geração de valores aleatórios.
	- Sortear a posição dos sprites na tela, e o ângulo no qual ele irá se mover.
2 - Implementar o espelhamento da posição do sprite no momento em que ele sai da tela.
3 - Implementar colisão em software entre os sprites.
*/


/*-------Definição dos dados referentes aos sprites-------------------------------------------------------------------------*/
typedef struct sprite{
	int coord_x;          //coordenada x atual do sprite na tela.
	int coord_y;          //coordenada y atual do sprite na tela.
	int direction;        //variável que define o ângulo de movimento do sprite.
	int offset;           //variável que definie o offset de memória do sprite. É usada para escolher qual animação usar.
	unsigned long mask_x; //Número de passos que o sprite se movimenta no eixo X.
	unsigned long mask_y; //Número de passos que o sprite se movimenta no eixo Y.
	unsigned long inst_A; //Variável que armazena os bits de instrução referente ao barramento dataA.
	unsigned long inst_B; //Variável que armazena os bits de instrução referente ao barramento dataB.
} Sprite;
/*---------------------------------------------------------------------------------------------------------------------------*/

/*-------------------MACRO DAS FUNÇÕES-------------------------*/
int sendInstruction(unsigned long dataA, unsigned long dataB);
unsigned long position_instruction_builder(int x, int y, int offset);
/*-----------------FIM DAS MACROS------------------------------*/

/*----------------PROCEDIMENTOS--------------------------------*/
void increase_coordinate(Sprite *sp);
/*-------------------------------------------------------------*/

/*---------------FUNÇÂO PRINCIPAL-------------------------*/
int main(){
	int number_screen = 0; //Variável que conta o número de telas impressas.
	int result = 0;        //Variável que define se uma instrução foi enviada. (1 - Sim/ 0 - Não)
	//int convert = 0;
	srand(time(NULL));
	
	Sprite ast_1;
	Sprite ast_2;
	Sprite ast_3;
	Sprite ast_4;
	Sprite ast_5;
	Sprite ast_6;
	Sprite ast_7;
	Sprite ast_8;


	/*----Definição dos dados iniciais de cada sprite-----*/
	ast_1.coord_x   = 1 + rand() % 639;     //sorteio da posição x.
	ast_1.coord_y   = 1 + rand() % 439;     //sorteio da posição y.
	ast_1.offset    = 0;                    //definição do offset de memória.
	ast_1.direction = rand() % 7;           //sorteio do ângulo inicial de movimento do sprite.
	ast_1.inst_B    = position_instruction_builder(ast_1.coord_x, ast_1.coord_y, ast_1.offset);
	ast_1.inst_A    = 0b00000000000000000000000000010000;
	ast_1.mask_x    = 0b00000000001010000000000000000000;
	ast_1.mask_y    = 0b00000000000000000000001000000000;


	ast_2.coord_x   = 1 + rand() % 639;     //sorteio da posição x.
	ast_2.coord_y   = 1 + rand() % 439;     //sorteio da posição y.
	ast_2.offset    = 0;                    //definição do offset de memória.
	ast_2.direction = rand() % 7;           //sorteio do ângulo inicial de movimento do sprite.
	ast_2.inst_B    = position_instruction_builder(ast_2.coord_x, ast_2.coord_y, ast_2.offset);
	ast_2.inst_A    = 0b00000000000000000000000000100000;
	ast_2.mask_x    = 0b00000000001010000000000000000000;
	ast_2.mask_y    = 0b00000000000000000000001000000000;

	ast_3.coord_x   = 1 + rand() % 639;     //sorteio da posição x.
	ast_3.coord_y   = 1 + rand() % 439;     //sorteio da posição y.
	ast_3.offset    = 0;                    //definição do offset de memória.
	ast_3.direction = rand() % 7;           //sorteio do ângulo inicial de movimento do sprite.
	ast_3.inst_B    = position_instruction_builder(ast_3.coord_x, ast_3.coord_y, ast_3.offset);
	ast_3.inst_A    = 0b00000000000000000000000000110000;
	ast_3.mask_x    = 0b00000000001010000000000000000000;
	ast_3.mask_y    = 0b00000000000000000000001000000000;


	ast_4.coord_x   = 1 + rand() % 639;     //sorteio da posição x.
	ast_4.coord_y   = 1 + rand() % 439;     //sorteio da posição y.
	ast_4.offset    = 1;                    //definição do offset de memória.
	ast_4.direction = rand() % 7;           //sorteio do ângulo inicial de movimento do sprite.
	ast_4.inst_B    = position_instruction_builder(ast_4.coord_x, ast_4.coord_y, ast_4.offset);
	ast_4.inst_A    = 0b00000000000000000000000001000000;
	ast_4.mask_x    = 0b00000000001010000000000000000000;
	ast_4.mask_y    = 0b00000000000000000000001000000000;

	ast_5.coord_x   = 1 + rand() % 639;     //sorteio da posição x.
	ast_5.coord_y   = 1 + rand() % 439;     //sorteio da posição y.
	ast_5.offset    = 2;                    //definição do offset de memória.
	ast_5.direction = rand() % 7;           //sorteio do ângulo inicial de movimento do sprite.
	ast_5.inst_B    = position_instruction_builder(ast_5.coord_x, ast_5.coord_y, ast_5.offset);
	ast_5.inst_A    = 0b00000000000000000000000001010000;
	ast_5.mask_x    = 0b00000000001010000000000000000000;
	ast_5.mask_y    = 0b00000000000000000000101000000000;


	ast_6.coord_x   = 1 + rand() % 639;     //sorteio da posição x.
	ast_6.coord_y   = 1 + rand() % 439;     //sorteio da posição y.
	ast_6.offset    = 1;                    //definição do offset de memória.
	ast_6.direction = rand() % 7;           //sorteio do ângulo inicial de movimento do sprite.
	ast_6.inst_B    = position_instruction_builder(ast_6.coord_x, ast_6.coord_y, ast_6.offset);
	ast_6.inst_A    = 0b00000000000000000000000001100000;
	ast_6.mask_x    = 0b00000000001010000000000000000000;
	ast_6.mask_y    = 0b00000000000000000000101000000000;

	ast_7.coord_x   = 1 + rand() % 639;     //sorteio da posição x.
	ast_7.coord_y   = 1 + rand() % 439;     //sorteio da posição y.
	ast_7.offset    = 2;                    //definição do offset de memória.
	ast_7.direction = rand() % 7;           //sorteio do ângulo inicial de movimento do sprite.
	ast_7.inst_B    = position_instruction_builder(ast_7.coord_x, ast_7.coord_y, ast_7.offset);
	ast_7.inst_A    = 0b00000000000000000000000001110000;
	ast_7.mask_x    = 0b00000000001010000000000000000000;
	ast_7.mask_y    = 0b00000000000000000000101000000000;


	ast_8.coord_x   = 1 + rand() % 639;     //sorteio da posição x.
	ast_8.coord_y   = 1 + rand() % 439;     //sorteio da posição y.
	ast_8.offset    = 1;                    //definição do offset de memória.
	ast_8.direction = rand() % 7;           //sorteio do ângulo inicial de movimento do sprite.
	ast_8.inst_B    = position_instruction_builder(ast_8.coord_x, ast_8.coord_y, ast_8.offset);
	ast_8.inst_A    = 0b00000000000000000000000010000000;
	ast_8.mask_x    = 0b00000000001010000000000000000000;
	ast_8.mask_y    = 0b00000000000000000000001000000000;

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
			increase_coordinate(&ast_1);
			increase_coordinate(&ast_2);
			increase_coordinate(&ast_5);
			increase_coordinate(&ast_6);
			increase_coordinate(&ast_7);
			increase_coordinate(&ast_8);
			if(IORD(WRFULL_BASE,0) == 0){                           //FIFO não está cheia
				sendInstruction(ast_1.inst_A, ast_1.inst_B);
				sendInstruction(ast_2.inst_A, ast_2.inst_B);
				/*if(result == 1){
					increase_coordinate(&ast_2);
				}
				result = sendInstruction(ast_3.inst_A, ast_3.inst_B);
				if(result == 1){
					increase_coordinate(&ast_3);
				}
				result = sendInstruction(ast_4.inst_A, ast_4.inst_B);
				if(result == 1){
					increase_coordinate(&ast_4);
				}*/
				result = sendInstruction(ast_5.inst_A, ast_5.inst_B);
				result = sendInstruction(ast_6.inst_A, ast_6.inst_B);
				result = sendInstruction(ast_7.inst_A, ast_7.inst_B);
				result = sendInstruction(ast_8.inst_A, ast_8.inst_B);
			}else{
				IOWR_ALTERA_AVALON_PIO_DATA(WRREG_BASE,0);         //Desabilita o sinal de escrita
				printf("[INFO] FILA CHEIA\n");
			}
		}
	}
	/*--------------------------FIM DO LOOP PRINCIPAL----------------------*/
}
/*----------FIM DA FUNÇÂO PRINCIPAL--------------------------*/


void increase_coordinate(Sprite *sp){
	int step_x = 0;     				     //Valor inteiro que corresponde a quantidade de passos que o sprite faz no eixo X.
	int step_y = 0;    					     //Valor inteiro que corresponde a quantidade de passos que o sprite faz no eixo Y.	
	step_x = (*sp).mask_x & MASX_TO_SHIFT_X; //Aplica uma operação AND para selecionar os bits da coordenada X.
	step_y = (*sp).mask_y & MASX_TO_SHIFT_Y; //Aplica uma operação AND para selecionar os bits da coordenada Y.
	step_x = step_x >> 19;                   //Realiza uma operação de deslocamento para chegar ao valor correto da coordenada.
	step_y = step_y >> 9;
	/*Switch que escolhe quais incrementos/decrementos serão realizados de acordo ao ângulo de movimento do sprite.*/
	switch((*sp).direction){
		case 0:                   							//0 graus   (esquerda)
			(*sp).coord_x -= step_x; 						//Atualiza a coordenada X.
			(*sp).inst_B  -= (*sp).mask_x;  		            //Atualiza os bits de instrução.
			break;
		case 1:                                              //45 graus  (diagonal superior direita)
			(*sp).coord_x += step_x; 						//Atualiza a coordenada X.
			(*sp).coord_y += step_y; 						//Atualiza a coordenada Y.
			(*sp).inst_B  += (*sp).mask_x;  		            //Atualiza os bits de instrução em X.
			(*sp).inst_B  += (*sp).mask_y;  		            //Atualiza os bits de instrução em Y.
			break;
		case 2:                                              //90 graus  (pra cima)
			(*sp).coord_y += step_y; 						//Atualiza a coordenada Y.
			(*sp).inst_B  += (*sp).mask_y;  		            //Atualiza os bits de instrução.
			break;
		case 3:          									//135 graus (diagonal superior esquerda)
			(*sp).coord_x -= step_x; 						//Atualiza a coordenada X.
			(*sp).coord_y += step_y; 						//Atualiza a coordenada Y.
			(*sp).inst_B  -= (*sp).mask_x;  		            //Atualiza os bits de instrução em X.
			(*sp).inst_B  += (*sp).mask_y;  		            //Atualiza os bits de instrução em Y.
			break;
		case 4:          									//180 graus (direita)
			(*sp).coord_x += step_x; 						//Atualiza a coordenada X.
			(*sp).inst_B  += (*sp).mask_x;  		            //Atualiza os bits de instrução.
			break;
		case 5:          									//225 graus (diagonal inferior esquerda)
			(*sp).coord_x -= step_x; 						//Atualiza a coordenada X.
			(*sp).coord_y -= step_y; 						//Atualiza a coordenada Y.
			(*sp).inst_B  -= (*sp).mask_x;  		            //Atualiza os bits de instrução em X.
			(*sp).inst_B  -= (*sp).mask_y;  		            //Atualiza os bits de instrução em Y.
			break;
		case 6:                                              //270 graus (pra baixo)
			(*sp).coord_y -= step_y; 						//Atualiza a coordenada Y.
			(*sp).inst_B  -= (*sp).mask_y;  		            //Atualiza os bits de instrução.
			break;
		case 7:                                              //315 graus (diagonal inferior direita)
			(*sp).coord_x += step_x; 						//Atualiza a coordenada X.
			(*sp).coord_y -= step_y; 						//Atualiza a coordenada Y.
			(*sp).inst_B  += (*sp).mask_x;  		            //Atualiza os bits de instrução em X.
			(*sp).inst_B  -= (*sp).mask_y;  		            //Atualiza os bits de instrução em Y.
			break;
	}
}

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



