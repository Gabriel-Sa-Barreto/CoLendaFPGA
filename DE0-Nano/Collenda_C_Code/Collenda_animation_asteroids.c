#include "stdio.h"
#include "io.h"
#include "sys/alt_stdio.h"
#include "altera_avalon_pio_regs.h"
#include "system.h"
#include "stdlib.h"
#include "time.h"
#include <unistd.h>
#define WRFULL_BASE 0x11090
#define WRREG_BASE  0x110a0
#define DATA_A_BASE 0x110d0
#define DATA_B_BASE 0x110c0
#define RESET_PULSECOUNTER_BASE 0x110b0
#define DIRECTION_BUTTONS_BASE 0x11070
#define SCREEN_BASE 0x11080
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

/*-------------------PROTÓTIPO DAS FUNÇÕES-------------------------*/
int sendInstruction(unsigned long dataA, unsigned long dataB);
unsigned long position_instruction_builder(int x, int y, int offset);
unsigned long dataA_builder(int opcode, int reg, int memory_address);
/*-----------------------------------------------------------------*/

/*----------------PROCEDIMENTOS--------------------------------*/
void increase_coordinate(Sprite *sp);
void initialize_sprite_random(int step_x, int step_y, int offset, Sprite *sp);
void initialize_sprite_fixe(int x, int y, int step_x, int step_y, int offset, Sprite *sp);
/*-------------------------------------------------------------*/

/*---------------FUNÇÂO PRINCIPAL-------------------------*/
int main(){
	int number_screen = 0;
	int send_lot1 = 0;
	int send_lot2 = 0;
	srand(time(NULL));
	unsigned long dataA_background = dataA_builder(1,0,16383);
	int background_color = 0;

	Sprite ast_1;
	Sprite ast_2;
	Sprite ast_3;
	Sprite ast_4;
	Sprite ast_5;
	Sprite ast_6;
	Sprite ast_7;
	Sprite ast_8;
	Sprite ast_9;
	Sprite ast_10;
	Sprite ast_11;
	Sprite ast_12;
	Sprite ast_13;
	Sprite ast_14;
	Sprite ast_15;
	Sprite ast_16;
	Sprite ast_17;
	Sprite ast_18;
	Sprite ast_19;
	Sprite ast_20;
	Sprite ast_21;
	Sprite ast_22;
	Sprite ast_23;
	Sprite ast_24;
	Sprite ast_25;
	Sprite ast_26;
	Sprite ast_27;
	Sprite ast_28;
	Sprite ast_29;
	Sprite ast_30;
	Sprite ast_31;
	Sprite ast_32;

	initialize_sprite_random(5,5,0,&ast_1);
	initialize_sprite_random(5,5,0,&ast_2);
	initialize_sprite_random(5,5,0,&ast_3);
	initialize_sprite_random(5,5,0,&ast_4);
	initialize_sprite_random(5,5,0,&ast_5);
	initialize_sprite_random(5,5,0,&ast_6);
	initialize_sprite_random(5,5,0,&ast_7);
	initialize_sprite_random(5,5,0,&ast_8);
	initialize_sprite_random(5,5,0,&ast_9);
	initialize_sprite_random(5,5,0,&ast_10);
	initialize_sprite_random(5,5,0,&ast_11);
	initialize_sprite_fixe(200,10,5,5,1,&ast_12);
	initialize_sprite_fixe(230,10,5,5,2,&ast_13);
	initialize_sprite_fixe(260,10,5,5,0,&ast_14);
	initialize_sprite_fixe(290,10,5,5,0,&ast_15);
	initialize_sprite_fixe(320,10,5,5,2,&ast_16);
	initialize_sprite_fixe(350,10,5,5,0,&ast_17);
	initialize_sprite_fixe(470,10,5,5,1,&ast_18);
	initialize_sprite_fixe(500,10,5,5,0,&ast_19);
	initialize_sprite_fixe(530,10,5,5,2,&ast_20);
	initialize_sprite_fixe(560,10,5,5,0,&ast_21);
	initialize_sprite_fixe(590,10,5,5,1,&ast_22);
	initialize_sprite_fixe(200,440,5,5,0,&ast_23);
	initialize_sprite_fixe(230,440,5,5,1,&ast_24);
	initialize_sprite_fixe(260,440,5,5,2,&ast_25);
	initialize_sprite_fixe(290,440,5,5,2,&ast_26);
	initialize_sprite_fixe(320,440,5,5,0,&ast_27);
	initialize_sprite_fixe(350,440,5,5,0,&ast_28);
	initialize_sprite_fixe(470,440,5,5,0,&ast_29);
	initialize_sprite_fixe(500,440,5,5,1,&ast_30);
	initialize_sprite_fixe(530,440,5,5,0,&ast_31);
	initialize_sprite_fixe(560,440,5,5,0,&ast_32);

	ast_1.inst_A  = dataA_builder(0,0,0);
	ast_2.inst_A  = dataA_builder(0,1,0);
	ast_3.inst_A  = dataA_builder(0,2,0);
	ast_4.inst_A  = dataA_builder(0,3,0);
	ast_5.inst_A  = dataA_builder(0,4,0);
	ast_6.inst_A  = dataA_builder(0,5,0);
	ast_7.inst_A  = dataA_builder(0,6,0);
	ast_8.inst_A  = dataA_builder(0,7,0);
	ast_9.inst_A  = dataA_builder(0,8,0);
	ast_10.inst_A = dataA_builder(0,9,0);
	ast_11.inst_A = dataA_builder(0,10,0);
	ast_12.inst_A = dataA_builder(0,11,0);
	ast_13.inst_A = dataA_builder(0,12,0);
	ast_14.inst_A = dataA_builder(0,13,0);
	ast_15.inst_A = dataA_builder(0,14,0);
	ast_16.inst_A = dataA_builder(0,15,0);
	ast_17.inst_A = dataA_builder(0,16,0);
	ast_18.inst_A = dataA_builder(0,17,0);
	ast_19.inst_A = dataA_builder(0,18,0);
	ast_20.inst_A = dataA_builder(0,19,0);
	ast_21.inst_A = dataA_builder(0,20,0);
	ast_22.inst_A = dataA_builder(0,21,0);
	ast_23.inst_A = dataA_builder(0,22,0);
	ast_24.inst_A = dataA_builder(0,23,0);
	ast_25.inst_A = dataA_builder(0,24,0);
	ast_26.inst_A = dataA_builder(0,25,0);
	ast_27.inst_A = dataA_builder(0,26,0);
	ast_28.inst_A = dataA_builder(0,27,0);
	ast_29.inst_A = dataA_builder(0,28,0);
	ast_30.inst_A = dataA_builder(0,29,0);
	ast_31.inst_A = dataA_builder(0,30,0);
	ast_32.inst_A = dataA_builder(0,31,0);

	int send = 0;
	while(send < 4) {
		if(IORD(SCREEN_BASE,0) == 1){
			number_screen++;
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,1);
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,0);
		}
		if(IORD(WRFULL_BASE,0) == 0){
			switch(send) {
				case 0:
					if(number_screen == 1){
						sendInstruction(ast_12.inst_A, ast_12.inst_B);
						sendInstruction(ast_13.inst_A, ast_13.inst_B);
						sendInstruction(ast_14.inst_A, ast_14.inst_B);
						sendInstruction(ast_15.inst_A, ast_15.inst_B);
						sendInstruction(ast_16.inst_A, ast_16.inst_B);
						sendInstruction(ast_17.inst_A, ast_17.inst_B);
						send++;
					}
					break;
				case 1:
					if(number_screen == 2){
						sendInstruction(ast_18.inst_A, ast_18.inst_B);
						sendInstruction(ast_19.inst_A, ast_19.inst_B);
						sendInstruction(ast_20.inst_A, ast_20.inst_B);
						sendInstruction(ast_21.inst_A, ast_21.inst_B);
						sendInstruction(ast_22.inst_A, ast_22.inst_B);
						sendInstruction(ast_23.inst_A, ast_23.inst_B);
						send++;
					}
					break;
				case 2:
					if(number_screen == 3){
						sendInstruction(ast_24.inst_A, ast_24.inst_B);
						sendInstruction(ast_25.inst_A, ast_25.inst_B);
						sendInstruction(ast_26.inst_A, ast_26.inst_B);
						sendInstruction(ast_27.inst_A, ast_27.inst_B);
						sendInstruction(ast_28.inst_A, ast_28.inst_B);
						sendInstruction(ast_29.inst_A, ast_29.inst_B);
						send++;
					}
					break;
				case 3:
					if(number_screen == 4){
						sendInstruction(ast_30.inst_A, ast_30.inst_B);
						sendInstruction(ast_31.inst_A, ast_31.inst_B);
						sendInstruction(ast_32.inst_A, ast_32.inst_B);
						send++;
					}
					break;
			}
		}
	}

	/*----------------------------------------------------------------------------------------*/
	/*----------------------------LOOP PRINCIPAL-----------------------------*/
	IOWR_ALTERA_AVALON_PIO_DATA(WRREG_BASE,0);         //Desabilita o sinal de escrita
	number_screen = 0;
	while(1){
		switch(IORD(DIRECTION_BUTTONS_BASE,0)) {
			case 1:
				background_color = 0b000000111;
				break;  
			case 2:
				background_color = 0b000111000;
				break;
			case 3:
				background_color = 0b111000000;
				break;
			case 4:
				background_color = 0b111111000;
				break;
			case 5:
				background_color = 0b000111111;
			case 6:
				background_color = 0b111111111;
				break;  
			case 7:
				background_color = 0b100101010;
				break;
			case 8:
				background_color = 0b000111011;
				break;
			case 9:
				background_color = 0b010100110;
				break;
			case 10:
				background_color = 0b000000000;
				break;
		}
		if(IORD(SCREEN_BASE,0) == 1){
			number_screen++;
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,1);
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,0);
		}
		if(number_screen == 1 && send_lot1 == 0){
			if(IORD(WRFULL_BASE,0) == 0){                           //FIFO não está cheia
				send_lot1 = 1;
				increase_coordinate(&ast_7);
				increase_coordinate(&ast_8);
				increase_coordinate(&ast_9);
				increase_coordinate(&ast_10);
				increase_coordinate(&ast_11);
				sendInstruction(ast_7.inst_A, ast_7.inst_B);
				sendInstruction(ast_8.inst_A, ast_8.inst_B);
				sendInstruction(ast_9.inst_A, ast_9.inst_B);
				sendInstruction(ast_10.inst_A, ast_10.inst_B);
				sendInstruction(ast_11.inst_A, ast_11.inst_B);
				sendInstruction(dataA_background,background_color);
			}
		}
		if(number_screen == 2 && send_lot2 == 0){
			if(IORD(WRFULL_BASE,0) == 0){                           //FIFO não está cheia
				send_lot2 = 1;
				increase_coordinate(&ast_1);
				increase_coordinate(&ast_2);
				increase_coordinate(&ast_3);
				increase_coordinate(&ast_4);
				increase_coordinate(&ast_5);
				increase_coordinate(&ast_6);
				sendInstruction(ast_1.inst_A, ast_1.inst_B);
				sendInstruction(ast_2.inst_A, ast_2.inst_B);
				sendInstruction(ast_3.inst_A, ast_3.inst_B);
				sendInstruction(ast_4.inst_A, ast_4.inst_B); 
				sendInstruction(ast_5.inst_A, ast_5.inst_B);
				sendInstruction(ast_6.inst_A, ast_6.inst_B);
				number_screen = 0;
				send_lot1 = 0;
				send_lot2 = 0;
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
			(*sp).inst_B  -= (*sp).mask_x;  		        //Atualiza os bits de instrução.
			if((*sp).coord_x < 1){                          //Realiza a troca de posição do sprite ao chegar no limite esquerdo da tela.
				(*sp).coord_x = 640;
				(*sp).inst_B  = position_instruction_builder((*sp).coord_x, (*sp).coord_y, (*sp).offset);
			}
			break;
		case 1:                                             //45 graus  (diagonal superior direita)
			(*sp).coord_x += step_x; 						//Atualiza a coordenada X.
			(*sp).coord_y -= step_y; 						//Atualiza a coordenada Y.
			(*sp).inst_B  += (*sp).mask_x;  		        //Atualiza os bits de instrução em X.
			(*sp).inst_B  -= (*sp).mask_y;  		        //Atualiza os bits de instrução em Y.
			if((*sp).coord_y < 0){                          //Realiza a troca de posição do sprite ao chegar no limite superior da tela.
				(*sp).coord_y = 480;
				(*sp).inst_B  = position_instruction_builder((*sp).coord_x, (*sp).coord_y, (*sp).offset);
			}else if((*sp).coord_x > 640){                  //Realiza a troca de posição do sprite ao chegar no limite direito da tela.
				(*sp).coord_x = 0;
				(*sp).inst_B  = position_instruction_builder((*sp).coord_x, (*sp).coord_y, (*sp).offset);
			}
			break;
		case 2:                                             //90 graus  (pra cima)
			(*sp).coord_y -= step_y; 						//Atualiza a coordenada Y.
			(*sp).inst_B  -= (*sp).mask_y;  		        //Atualiza os bits de instrução.
			if((*sp).coord_y < 0){                          //Realiza a troca de posição do sprite ao chegar no limite superior da tela.
				(*sp).coord_y = 480;
				(*sp).inst_B  = position_instruction_builder((*sp).coord_x, (*sp).coord_y, (*sp).offset);
			}
			break;
		case 3:          									//135 graus (diagonal superior esquerda)
			(*sp).coord_x -= step_x; 						//Atualiza a coordenada X.
			(*sp).coord_y -= step_y; 						//Atualiza a coordenada Y.
			(*sp).inst_B  -= (*sp).mask_x;  		        //Atualiza os bits de instrução em X.
			(*sp).inst_B  -= (*sp).mask_y;  		        //Atualiza os bits de instrução em Y.
			if((*sp).coord_y < 0){                          //Realiza a troca de posição do sprite ao chegar no limite superior da tela.
				(*sp).coord_y = 480;
				(*sp).inst_B  = position_instruction_builder((*sp).coord_x, (*sp).coord_y, (*sp).offset);
			}else if((*sp).coord_x < 1){                    //Realiza a troca de posição do sprite ao chegar no limite esquerdo da tela.
				(*sp).coord_x = 640;
				(*sp).inst_B  = position_instruction_builder((*sp).coord_x, (*sp).coord_y, (*sp).offset);
			}
			break;
		case 4:          									//180 graus (direita)
			(*sp).coord_x += step_x; 						//Atualiza a coordenada X.
			(*sp).inst_B  += (*sp).mask_x;  		        //Atualiza os bits de instrução.
			if((*sp).coord_x > 640){                        //Realiza a troca de posição do sprite ao chegar no limite direito da tela.
				(*sp).coord_x = 0;
				(*sp).inst_B  = position_instruction_builder((*sp).coord_x, (*sp).coord_y, (*sp).offset);
			}
			break;
		case 5:          									//225 graus (diagonal inferior esquerda)
			(*sp).coord_x -= step_x; 						//Atualiza a coordenada X.
			(*sp).coord_y += step_y; 						//Atualiza a coordenada Y.
			(*sp).inst_B  -= (*sp).mask_x;  		        //Atualiza os bits de instrução em X.
			(*sp).inst_B  += (*sp).mask_y;  		        //Atualiza os bits de instrução em Y.
			if((*sp).coord_y > 480){                        //Realiza a troca de posição do sprite ao chegar no limite inferior da tela.
				(*sp).coord_y = 0;
				(*sp).inst_B  = position_instruction_builder((*sp).coord_x, (*sp).coord_y, (*sp).offset);
			}else if((*sp).coord_x < 1){                    //Realiza a troca de posição do sprite ao chegar no limite esquerdo da tela.
				(*sp).coord_x = 640;
				(*sp).inst_B  = position_instruction_builder((*sp).coord_x, (*sp).coord_y, (*sp).offset);
			}
			break;
		case 6:                                              //270 graus (pra baixo)
			(*sp).coord_y += step_y; 						//Atualiza a coordenada Y.
			(*sp).inst_B  += (*sp).mask_y;  		        //Atualiza os bits de instrução.
			if((*sp).coord_y > 480){                        //Realiza a troca de posição do sprite ao chegar no limite inferior da tela.
				(*sp).coord_y = 0;
				(*sp).inst_B  = position_instruction_builder((*sp).coord_x, (*sp).coord_y, (*sp).offset);
			}
			break;
		case 7:                                             //315 graus (diagonal inferior direita)
			(*sp).coord_x += step_x; 						//Atualiza a coordenada X.
			(*sp).coord_y += step_y; 						//Atualiza a coordenada Y.
			(*sp).inst_B  += (*sp).mask_x;  		        //Atualiza os bits de instrução em X.
			(*sp).inst_B  += (*sp).mask_y;  		        //Atualiza os bits de instrução em Y.
			if((*sp).coord_y > 480){                        //Realiza a troca de posição do sprite ao chegar no limite inferior da tela.
				(*sp).coord_y = 480;
				(*sp).inst_B  = position_instruction_builder((*sp).coord_x, (*sp).coord_y, (*sp).offset);
			}else if((*sp).coord_x > 640){                  //Realiza a troca de posição do sprite ao chegar no limite direito da tela.
				(*sp).coord_x = 640;
				(*sp).inst_B  = position_instruction_builder((*sp).coord_x, (*sp).coord_y, (*sp).offset);
			}
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

/* ==============================================================================
Função que define o barramento dataB da instrução de modificar os dados de um sprite.
===================================================================================*/
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

void initialize_sprite_random(int step_x, int step_y, int offset, Sprite *sp){
	(*sp).coord_x   = 1 + rand() % 639;     //sorteio da posição x.
	(*sp).coord_y   = 1 + rand() % 439;     //sorteio da posição y.
	(*sp).offset    = offset;               //definição do offset de memória.
	(*sp).direction = rand() % 7;           //sorteio do ângulo inicial de movimento do sprite.
	(*sp).inst_B    = position_instruction_builder((*sp).coord_x, (*sp).coord_y, (*sp).offset);
	(*sp).inst_A    = 0;
	(*sp).mask_x    = MASX_TO_SHIFT_X & (step_x << 19);
	(*sp).mask_y    = MASX_TO_SHIFT_Y & (step_y << 9);
}

void initialize_sprite_fixe(int x, int y, int step_x, int step_y, int offset, Sprite *sp){
	(*sp).coord_x   = x;
	(*sp).coord_y   = y;
	(*sp).offset    = offset;               //definição do offset de memória.
	(*sp).direction = rand() % 7;           //sorteio do ângulo inicial de movimento do sprite.
	(*sp).inst_B    = position_instruction_builder((*sp).coord_x, (*sp).coord_y, (*sp).offset);
	(*sp).inst_A    = 0;
	(*sp).mask_x    = MASX_TO_SHIFT_X & (step_x << 19);
	(*sp).mask_y    = MASX_TO_SHIFT_Y & (step_y << 9);
}


unsigned long dataA_builder(int opcode, int reg, int memory_address){
	unsigned long data = 0b00000000000000000000000000000000;
	switch(opcode){
		case(0):                                //instrução de escrita no banco de registradores.
			data = data | reg;                  //operação OR (adiciona o número do registrador)
			data = data << 4;                   //deslocamento à esquerda em 4 posições.
			data = data | opcode;               //operação OR (adiciona o opcode).
			break;
		case(1):                                //instrução de escrita na memória de sprites.
			data = data | memory_address;       //operação OR (adiciona o endereço de memória)
			data = data << 4;                   //deslocamento à esquerda em 4 posições.
			data = data | opcode;               //operação OR (adiciona o opcode).
			break;
	}
	return data;
}


