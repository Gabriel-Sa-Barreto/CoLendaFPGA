#include "stdio.h"
#include "io.h"
#include "sys/alt_stdio.h"
#include "altera_avalon_pio_regs.h"
#include "system.h"
#include "stdlib.h"
#include "time.h"
#include <unistd.h>
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
#define MASX_TO_SHIFT_X 0b00011111111110000000000000000000
#define MASX_TO_SHIFT_Y 0b00000000000001111111111000000000

/* =========================================================
Configuracao do barramento de botoes:
DOWN:             move_buttons[0] = GPIO_19  = PIN_R11
UP:               move_buttons[1] = GPIO_111 = PIN_R10
LEFT:             move_buttons[2] = GPIO_113 = PIN_P9 
RIGHT:            move_buttons[3] = GPIO_115 = PIN_N11
ATIRAR:           direction_and_shoot[0] = GPIO_117 = PIN_K16
GIRAR p/ESQUERDA: direction_and_shoot[1] = GPIO_119 = PIN_L15
GIRAR p/DIREITA:  direction_and_shoot[2] = GPIO_121 = PIN_P16
============================================================ */

/* ==========================================================
OFFSET:
Asteroids: 0, 1, 2
Nave:      3, 4, 5, 6, 7, 8, 9, 10
Tiro:      11,12,13,14
Numeros:   15,16,17,18,19,20,21,22,23,24
GAME OVER: 25,26,27,28,29,30,31
============================================================ */ 

/*-------Definicao dos dados referentes aos sprites-------------------------------------------------------------------------*/
typedef struct sprite{
	int coord_x;          //coordenada x atual do sprite na tela.
	int coord_y;          //coordenada y atual do sprite na tela.
	int direction;        //variavel que define o angulo de movimento do sprite.
	int offset;           //variavel que define o offset de memoria do sprite. Usada para escolher qual anima����o usar.
	int data_register;    //variavel que define em qual registrador os dados referente ao sprite serao armazenados na GPU.
	unsigned long mask_x; //Numero de passos que o sprite se movimenta no eixo X.
	unsigned long mask_y; //Numero de passos que o sprite se movimenta no eixo Y.
	unsigned long inst_A; //Variavel que armazena os bits de instrucao referente ao barramento dataA.
	unsigned long inst_B; //Variavel que armazena os bits de instrucao referente ao barramento dataB.
	int ativo;
	int collision;        // 0 - sem colisao , 1 - sprite colidiu
} Sprite;
/*---------------------------------------------------------------------------------------------------------------------------*/

/*-------------------PROTOTIPO DAS FUNCOES-------------------------*/
int sendInstruction(unsigned long dataA, unsigned long dataB);
unsigned long position_instruction_builder(int x, int y, int offset, int ativacao);
unsigned long dataA_builder(int opcode, int reg, int memory_address);
int collision(Sprite *sp1, Sprite *sp2);
/*-----------------------------------------------------------------*/

/*----------------PROCEDIMENTOS--------------------------------*/
void increase_coordinate(Sprite *sp, int refresh);
void initialize_sprite_random(int step_x, int step_y, int offset, Sprite *sp);
void initialize_sprite_fixe(int x, int y, int step_x, int step_y, int offset, Sprite *sp);
void atirar_sp(Sprite *nave, Sprite *tiro);
void collision_between_tiro_and_ast(Sprite *tiro, Sprite* asteroids[], Sprite* placar[]);
void increase_points(Sprite *dg_1, Sprite *dg_2, Sprite *dg_3);
/*-------------------------------------------------------------*/

/*----------------VARIAVEIS GLOBAIS----------------------------*/
int number_of_asteroids = 9;
int points = 0;
int state_placar = 0;
/*-------------------------------------------------------------*/
/*---------------FUNCAO PRINCIPAL-------------------------*/
int main(){
	int screen_for_1s = 59;
	int number_screen;
	int number_screen_2;
	int number_screen_3;
	int number_screen_4;
	int wait_shoot = 0;
	int atirar     = 0;
	int refresh_coord   = 0;
	int check_collision = 0;
	srand(time(NULL));
	//========================================================================================
	//=== Variaveis para controle dos asteroids.
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
	ast_1.ativo = 1;
	ast_2.ativo = 1;
	ast_3.ativo = 1;
	ast_4.ativo = 1;
	ast_5.ativo = 1;
	ast_6.ativo = 1;
	ast_7.ativo = 1;
	ast_8.ativo = 1;
	ast_9.ativo = 1;
	//========================================================================================
	//=== Variaveis para controle da nave e dos tiros.
	Sprite nave;
	Sprite tiro;
	Sprite tiro2;
	Sprite tiro3;
	Sprite tiro4;
	Sprite tiro5;
	//========================================================================================
	//=== Variaveis para controle do placar.
	Sprite placar_dg1;
	Sprite placar_dg2;
	Sprite placar_dg3;
	//========================================================================================
	//=== Inicializa as informacoes dos asteroids referentes as coordenadas x e y, angulo de movimento, offset e passos a percorrer.
	initialize_sprite_random(2,2,0,&ast_1);
	initialize_sprite_random(2,2,0,&ast_2);
	initialize_sprite_random(2,2,0,&ast_3);
	initialize_sprite_random(2,2,0,&ast_4);
	initialize_sprite_random(2,2,0,&ast_5);
	initialize_sprite_random(2,2,0,&ast_6);
	initialize_sprite_random(3,3,0,&ast_7);
	initialize_sprite_random(3,3,0,&ast_8);
	initialize_sprite_random(3,3,0,&ast_9);
	//========================================================================================
	//=== Definicao dos registradores para armazenamento das informacoes de cada sprite.
	ast_1.data_register  = 0;
	ast_2.data_register  = 1;
	ast_3.data_register  = 2;
	ast_4.data_register  = 3;
	ast_5.data_register  = 4;
	ast_6.data_register  = 5;
	ast_7.data_register  = 6;
	ast_8.data_register  = 7;
	ast_9.data_register  = 8;
	nave.data_register   = 9;
	tiro.data_register   = 10;
	tiro2.data_register  = 11;
	tiro3.data_register  = 12;
	tiro4.data_register  = 13;
	tiro5.data_register  = 14;
	placar_dg1.data_register = 16;
	placar_dg2.data_register = 17;
	placar_dg3.data_register = 18;
    //========================================================================================
	//=== Inicializa as informacoes referentes aos sprites do placar e da nave.
	placar_dg1.ativo = 1;
	placar_dg2.ativo = 1;
	placar_dg3.ativo = 1;
	nave.ativo  = 1;
	tiro.ativo  = 0;
	tiro2.ativo = 0;
	tiro3.ativo = 0;
	tiro4.ativo = 0;
	tiro5.ativo = 0;
	initialize_sprite_fixe(20,10,0,0,15,&placar_dg1);
	initialize_sprite_fixe(40,10,0,0,15,&placar_dg2);
	initialize_sprite_fixe(60,10,0,0,15,&placar_dg3);
	initialize_sprite_fixe(320,430,5,5,3,&nave);
	initialize_sprite_fixe(0,0,5,5,11,&tiro);
	initialize_sprite_fixe(0,0,5,5,11,&tiro2);
	initialize_sprite_fixe(0,0,5,5,11,&tiro3);
	initialize_sprite_fixe(0,0,5,5,11,&tiro4);
	initialize_sprite_fixe(0,0,5,5,11,&tiro5);
	//========================================================================================
	//Prepara os dados que serao enviados no barramento dataA para controle dos asteroids, nave, tiro e placar.
	ast_1.inst_A       = dataA_builder(0,ast_1.data_register,0);
	ast_2.inst_A       = dataA_builder(0,ast_2.data_register,0);
	ast_3.inst_A       = dataA_builder(0,ast_3.data_register,0);
	ast_4.inst_A       = dataA_builder(0,ast_4.data_register,0);
	ast_5.inst_A       = dataA_builder(0,ast_5.data_register,0);
	ast_6.inst_A       = dataA_builder(0,ast_6.data_register,0);
	ast_7.inst_A       = dataA_builder(0,ast_7.data_register,0);
	ast_8.inst_A       = dataA_builder(0,ast_8.data_register,0);
	ast_9.inst_A       = dataA_builder(0,ast_9.data_register,0);
	nave.inst_A        = dataA_builder(0,nave.data_register,0);
	tiro.inst_A        = dataA_builder(0,tiro.data_register,0);
	tiro2.inst_A       = dataA_builder(0,tiro2.data_register,0);
	tiro3.inst_A       = dataA_builder(0,tiro3.data_register,0);
	tiro4.inst_A       = dataA_builder(0,tiro4.data_register,0);
	tiro5.inst_A       = dataA_builder(0,tiro5.data_register,0);
	placar_dg1.inst_A  = dataA_builder(0,placar_dg1.data_register,0);
	placar_dg2.inst_A  = dataA_builder(0,placar_dg2.data_register,0);
	placar_dg3.inst_A  = dataA_builder(0,placar_dg3.data_register,0);
	//========================================================================================

	int send = 0;
	number_screen = 0;
	while(send == 0) {
		if(IORD(SCREEN_BASE,0) == 1){
			number_screen++;
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,1);
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,0);
		}
		if(IORD(WRFULL_BASE,0) == 0){
			if(number_screen == 1){
				sendInstruction(placar_dg3.inst_A, placar_dg3.inst_B);
				sendInstruction(placar_dg2.inst_A, placar_dg2.inst_B);
				sendInstruction(placar_dg1.inst_A, placar_dg1.inst_B);
				sendInstruction(nave.inst_A, nave.inst_B);
				sendInstruction(dataA_builder(1,0,16383),0);
				send++;
			}
		}
	}
	number_screen   = 0;
	number_screen_2 = 0;
	number_screen_3 = 0;
	number_screen_4 = 0;
	int TR = 0;
	int TL = 0;
	int A  = 0;
	int move = 0;
	int collision_nave = 0;
	/*----------------------------------------------------------------------------------------*/
	/*----------------------------LOOP PRINCIPAL-----------------------------*/
	while(collision_nave == 0){
		TR   = IORD(TR_BASE,0);
		TL   = IORD(TL_BASE,0);
		A    = IORD(A_BASE,0);
		move = IORD(DIRECTION_ANALOGIC_BASE,0);  
		if(IORD(SCREEN_BASE,0) == 1){
			number_screen++;
			number_screen_2++;
			number_screen_3++;
			number_screen_4++;
			wait_shoot++;
			if(wait_shoot == screen_for_1s){
				atirar = 1; //ativa a possibilidade de atirar.
			}
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,1);
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,0);
		} else {
			if(check_collision == 1){ //realiza as analises de colisao.
				check_collision = 0;
				Sprite* asteroids[9] = { &ast_1, &ast_2, &ast_3, &ast_4, &ast_5, &ast_6, &ast_7, &ast_8, &ast_9 };
				// verifica se existe colisao entre a nave e algum asteroid
				for(int i = 0; i < number_of_asteroids; i++) {
					Sprite *ast = asteroids[i];
					if(collision(&nave, ast) == 1){
						// houve colisao entre a nave e algum asteroid. 
						// significa GAME OVER
						collision_nave = 1;
						break;
					}
				}
				// verifica se existe colisao entre os tiros e algum asteroid
				Sprite* placar[3] = { &placar_dg3, &placar_dg2, &placar_dg1 };
				if(tiro.ativo  == 1 && tiro.collision   == 0){ collision_between_tiro_and_ast(&tiro,  asteroids, placar); }
				if(tiro2.ativo == 1 && tiro2.collision  == 0){ collision_between_tiro_and_ast(&tiro2, asteroids, placar); }
				if(tiro3.ativo == 1 && tiro3.collision  == 0){ collision_between_tiro_and_ast(&tiro3, asteroids, placar); }
				if(tiro4.ativo == 1 && tiro4.collision  == 0){ collision_between_tiro_and_ast(&tiro4, asteroids, placar); }
				if(tiro5.ativo == 1 && tiro5.collision  == 0){ collision_between_tiro_and_ast(&tiro5, asteroids, placar); }
			}
		}
		switch(move){
			case 0b0111: //Down
				nave.direction = 6;
				refresh_coord = 1;
				break;
			case 0b1011: //Up
				nave.direction = 2;
				refresh_coord = 1;
				break;
			case 0b1101: //Direita
				nave.direction = 4;
				refresh_coord = 1;
				break;
			case 0b1110: //Esquerda
				nave.direction = 0;
				refresh_coord = 1;
				break;
			case 0b0101: //direita inferior
				nave.direction = 7;
				refresh_coord = 1;
				break;
			case 0b1001: //direita superior
				nave.direction = 1;
				refresh_coord = 1;
				break;
			case 0b1010: //esquerda superior  
				nave.direction = 3;
				refresh_coord = 1;
				break;
			case 0b0110: //esquerda inferior
				nave.direction = 5;
				refresh_coord = 1;
				break;
		}
		if(A == 1 && atirar == 1){
			atirar     = 0; // Depois de atirar, espera um novo momento.
			wait_shoot = 0;
			if(tiro.ativo == 0 && tiro.collision == 0){        tiro.ativo  = 1; atirar_sp(&nave, &tiro);  }
			else if(tiro2.ativo == 0 && tiro2.collision == 0){ tiro2.ativo = 1; atirar_sp(&nave, &tiro2); }
			else if(tiro3.ativo == 0 && tiro3.collision == 0){ tiro3.ativo = 1; atirar_sp(&nave, &tiro3); }
			else if(tiro4.ativo == 0 && tiro4.collision == 0){ tiro4.ativo = 1; atirar_sp(&nave, &tiro4); }
			else if(tiro5.ativo == 0 && tiro5.collision == 0){ tiro5.ativo = 1; atirar_sp(&nave, &tiro5); }
		}
		if(number_screen_2 == 6){
			number_screen_2 = 0;
			if(TR == 1){
				nave.offset += 1;
				if(nave.offset > 10){
					nave.offset = 3;
				}
				refresh_coord = 0;
			}else if(TL == 1){
				nave.offset -= 1;
				if(nave.offset < 3){
					nave.offset = 10;
				}
			}
		}
		if(number_screen == 2){
			number_screen = 0;
			if(IORD(WRFULL_BASE,0) == 0){                           //FIFO nao esta cheia
				if(tiro.collision  == 1){ tiro.ativo  = 0; tiro.collision  = 0; }
				if(tiro2.collision == 1){ tiro2.ativo = 0; tiro2.collision = 0; }
				if(tiro3.collision == 1){ tiro3.ativo = 0; tiro3.collision = 0; }
				if(tiro4.collision == 1){ tiro4.ativo = 0; tiro4.collision = 0; }
				if(tiro5.collision == 1){ tiro5.ativo = 0; tiro5.collision = 0; }
				increase_coordinate(&nave, refresh_coord);
				increase_coordinate(&tiro, 1);
				increase_coordinate(&tiro2, 1);
				increase_coordinate(&tiro3, 1);
				increase_coordinate(&tiro4, 1);
				increase_coordinate(&tiro5, 1);
				sendInstruction(nave.inst_A, nave.inst_B);
				sendInstruction(tiro.inst_A, tiro.inst_B);
				sendInstruction(tiro2.inst_A, tiro2.inst_B);
				sendInstruction(tiro3.inst_A, tiro3.inst_B);
				sendInstruction(tiro4.inst_A, tiro4.inst_B);
				sendInstruction(tiro5.inst_A, tiro5.inst_B);
				refresh_coord   = 0;
				check_collision = 1;
			}
		}
		if(number_screen_3 == 1){
			if(IORD(WRFULL_BASE,0) == 0){                           //FIFO nao esta cheia
				number_screen_3 = 0;
				check_collision = 1;
				increase_coordinate(&ast_1,1);
				increase_coordinate(&ast_2,1);
				increase_coordinate(&ast_3,1);
				increase_coordinate(&ast_4,1);
				increase_coordinate(&ast_5,1);
				increase_coordinate(&ast_6,1);
				sendInstruction(ast_1.inst_A, ast_1.inst_B);
				sendInstruction(ast_2.inst_A, ast_2.inst_B);
				sendInstruction(ast_3.inst_A, ast_3.inst_B);
				sendInstruction(ast_4.inst_A, ast_4.inst_B); 
				sendInstruction(ast_5.inst_A, ast_5.inst_B);
				sendInstruction(ast_6.inst_A, ast_6.inst_B);
			}
		}

		if(number_screen_4 == 3){
			if(IORD(WRFULL_BASE,0) == 0){                           //FIFO nao esta cheia
				number_screen_4 = 0;
				increase_coordinate(&ast_7,1);
				increase_coordinate(&ast_8,1);
				increase_coordinate(&ast_9,1);
				sendInstruction(placar_dg1.inst_A, placar_dg1.inst_B);
				sendInstruction(placar_dg2.inst_A, placar_dg2.inst_B);
				sendInstruction(placar_dg3.inst_A, placar_dg3.inst_B);
				sendInstruction(ast_7.inst_A,  ast_7.inst_B);
				sendInstruction(ast_8.inst_A,  ast_8.inst_B);
				sendInstruction(ast_9.inst_A,  ast_9.inst_B);
			}
		}
	}
	/*--------------------------FIM DO LOOP PRINCIPAL----------------------*/
	Sprite lG;
	Sprite lA;
	Sprite lM;
	Sprite lE;
	Sprite lO;
	Sprite lV;
	Sprite lR;
	Sprite lE_1;
	
	lG.data_register   = 0;
	lA.data_register   = 1;
	lM.data_register   = 2;
	lE.data_register   = 3;
	lO.data_register   = 4;
	lV.data_register   = 5;
	lE_1.data_register = 6;
	lR.data_register   = 7;
	
	lG.ativo   = 1;
	lA.ativo   = 1;
	lM.ativo   = 1;
	lE.ativo   = 1;
	lO.ativo   = 1;
	lV.ativo   = 1;
	lE_1.ativo = 1;
	lR.ativo   = 1;
	
	initialize_sprite_fixe(240,240,0,0,25,&lG);
	initialize_sprite_fixe(260,240,0,0,26,&lA);
	initialize_sprite_fixe(280,240,0,0,27,&lM);
	initialize_sprite_fixe(300,240,0,0,28,&lE);
	initialize_sprite_fixe(340,240,0,0,29,&lO);
	initialize_sprite_fixe(360,240,0,0,30,&lV);
	initialize_sprite_fixe(380,240,0,0,28,&lE_1);
	initialize_sprite_fixe(400,240,0,0,31,&lR);
	
	lG.inst_A   = dataA_builder(0,lG.data_register,0);
	lA.inst_A   = dataA_builder(0,lA.data_register,0);
	lM.inst_A   = dataA_builder(0,lM.data_register,0);
	lE.inst_A   = dataA_builder(0,lE.data_register,0);
	lO.inst_A   = dataA_builder(0,lO.data_register,0);
	lV.inst_A   = dataA_builder(0,lV.data_register,0);
	lE_1.inst_A = dataA_builder(0,lE_1.data_register,0);
	lR.inst_A   = dataA_builder(0,lR.data_register,0);
	
	ast_9.ativo = 0;
	increase_coordinate(&ast_9,0);

	wait_shoot = 0;
	while(1){
		if(IORD(SCREEN_BASE,0) == 1){
			wait_shoot++;
			if(wait_shoot == screen_for_1s/4){
				break;
			}
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,1);
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,0);
		}
	}
	send = 0;
	int lot1 = 0;
	
	while(send < 2){
		if(IORD(SCREEN_BASE,0) == 1){
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,1);
			IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,0);
			send++;
		}
		if(IORD(WRFULL_BASE,0) == 0 && send == 1 && lot1 == 0){
			sendInstruction(lG.inst_A, lG.inst_B);
			sendInstruction(lA.inst_A, lA.inst_B);
			sendInstruction(lM.inst_A, lM.inst_B);
			sendInstruction(lE.inst_A, lE.inst_B);
			sendInstruction(ast_9.inst_A, ast_9.inst_B);
			lot1 = 1;
		}else if(IORD(WRFULL_BASE,0) == 0 && send == 2){
			sendInstruction(lO.inst_A,   lO.inst_B);
			sendInstruction(lV.inst_A,   lV.inst_B);
			sendInstruction(lE_1.inst_A, lE_1.inst_B);
			sendInstruction(lR.inst_A,   lR.inst_B);
		}
	}
}
/*----------FIM DA FUNCAO PRINCIPAL--------------------------*/
void collision_between_tiro_and_ast(Sprite *tiro, Sprite* asteroids[], Sprite* placar[]){
	for (int i = 0; i < number_of_asteroids; i++){
		Sprite *ast = asteroids[i];
		if((*ast).ativo == 1){
			if(collision(tiro, ast) == 1){
				increase_points(placar[0], placar[1], placar[2]);
				(*tiro).collision = 1;
				(*ast).offset += 1;
				if((*ast).offset == 2){
					(*ast).offset = 0;
					(*ast).coord_x   = 1 + rand() % 639;     //sorteio de uma nova posicaoo x.
					(*ast).coord_y   = 1 + rand() % 439;     //sorteio de uma nova posicaoo y.
					(*ast).direction = rand() % 7;           //sorteio de um novo angulo inicial de movimento do sprite.
				}
			}
		}
	}
}

void increase_points(Sprite *dg_1, Sprite *dg_2, Sprite *dg_3){
	int incresed = 0;
	while(incresed == 0 && state_placar != 3){
		if(state_placar == 0){ // controla o primeiro digito
			if((*dg_1).offset == 24){
				(*dg_1).offset = 15; // numero 0
				state_placar    = 1;
				incresed        = 0;
			}else {
				(*dg_1).offset += 1;
				state_placar    = 0;
				incresed        = 1;
			}		
		}else if(state_placar == 1){  // controla o segundo digito
			if((*dg_2).offset == 24){
				(*dg_2).offset = 15; // numero 0
				state_placar    = 2;
				incresed        = 0;
			}else {
				(*dg_2).offset += 1;
				state_placar    = 0;
				incresed        = 1;
			}	
		}else if(state_placar == 2){ // controla o terceiro digito
			if((*dg_3).offset == 24){
				state_placar = 3; // vai para um estado inexistente. Nao conta mais. Chegou na pontuacao limite (900).
				incresed     = 1;
			}else{
				(*dg_3).offset += 1;
				state_placar    = 0;
				incresed        = 1;
			}	
		}
	}
	(*dg_1).inst_B  = position_instruction_builder((*dg_1).coord_x, (*dg_1).coord_y, (*dg_1).offset, (*dg_1).ativo);
	(*dg_2).inst_B  = position_instruction_builder((*dg_2).coord_x, (*dg_2).coord_y, (*dg_2).offset, (*dg_2).ativo);
	(*dg_3).inst_B  = position_instruction_builder((*dg_3).coord_x, (*dg_3).coord_y, (*dg_3).offset, (*dg_3).ativo);
}


int collision(Sprite *sp1, Sprite *sp2){
	int h = 10;
	int y_face_1 = (*sp1).coord_y + h;
	int y_face_2 = (*sp2).coord_y + h;
	int x_face_1 = (*sp1).coord_x + h;
	int x_face_2 = (*sp2).coord_x + h;
	/*-------------------------------------*/   
	if( (y_face_1 > (*sp2).coord_y ) && ((*sp1).coord_y < y_face_2) ){
		//Colisoes a esquerda --------------------------------------------
		if( ( x_face_1 > (*sp2).coord_x ) && (x_face_1 < x_face_2) ){
			return 1;
		}else if ( (x_face_1 < x_face_2) && (x_face_1 > (*sp2).coord_x) ){
			return 1;
		}
		/*--------------------------------------------------------------*/
		//Colisoes a direita --------------------------------------------
		if( ( x_face_1 > x_face_2) && (x_face_2 > (*sp1).coord_x) ){
			return 1;
		}else if( ( x_face_1 > x_face_2) && (x_face_2 > (*sp1).coord_x) ){
			return 1;
		}
		/*--------------------------------------------------------------*/
	}
	return 0;
}

void atirar_sp(Sprite *nave, Sprite *tiro){
	switch((*nave).offset){
		case 9: //esquerda
			(*tiro).coord_x   = (*nave).coord_x - 20;
			(*tiro).coord_y   = (*nave).coord_y;
			(*tiro).offset    = 13;
			(*tiro).direction = 0;
			break;
		case 4: //direita diagonal superior
			(*tiro).coord_x   = (*nave).coord_x + 20;
			(*tiro).coord_y   = (*nave).coord_y - 20;
			(*tiro).offset    = 12;
			(*tiro).direction = 1;
			break;
		case 3: //para cima
			(*tiro).coord_x   = (*nave).coord_x;
			(*tiro).coord_y   = (*nave).coord_y - 20;
			(*tiro).offset    = 11;
			(*tiro).direction = 2;
			break;
		case 10: //esquerda diagonal superior
			(*tiro).coord_x   = (*nave).coord_x - 20;
			(*tiro).coord_y   = (*nave).coord_y - 20;
			(*tiro).offset    = 14;
			(*tiro).direction = 3;
			break;
		case 5: //direita
			(*tiro).coord_x   = (*nave).coord_x + 20;
			(*tiro).coord_y   = (*nave).coord_y;
			(*tiro).offset    = 13;
			(*tiro).direction = 4;
			break;
		case 8: //esquerda diagonal inferior
			(*tiro).coord_x   = (*nave).coord_x - 20;
			(*tiro).coord_y   = (*nave).coord_y + 20;
			(*tiro).offset    = 12;
			(*tiro).direction = 5;
			break;
		case 7: //para baixo
			(*tiro).coord_x   = (*nave).coord_x;
			(*tiro).coord_y   = (*nave).coord_y + 20;
			(*tiro).offset    = 11;
			(*tiro).direction = 6;
			break;
		case 6: //diagonal direita inferior
			(*tiro).coord_x   = (*nave).coord_x + 20;
			(*tiro).coord_y   = (*nave).coord_y + 20;
			(*tiro).offset    = 14;
			(*tiro).direction = 7;
			break;
	}
}


void increase_coordinate(Sprite *sp, int refresh){
	int step_x = 0;     				     //Valor inteiro que corresponde a quantidade de passos que o sprite faz no eixo X.
	int step_y = 0;    					     //Valor inteiro que corresponde a quantidade de passos que o sprite faz no eixo Y.	
	step_x = (*sp).mask_x & MASX_TO_SHIFT_X; //Aplica uma operacao AND para selecionar os bits da coordenada X.
	step_y = (*sp).mask_y & MASX_TO_SHIFT_Y; //Aplica uma operacao AND para selecionar os bits da coordenada Y.
	step_x = step_x >> 19;                   //Realiza uma operacao de deslocamento para chegar ao valor correto da coordenada.
	step_y = step_y >> 9;
	/*Switch que escolhe quais incrementos/decrementos serao realizados de acordo ao anngulo de movimento do sprite.*/
	switch((*sp).direction){
		case 0:                   							//0 graus   (esquerda)
			if(refresh == 1){
				(*sp).coord_x -= step_x; 						//Atualiza a coordenada X.
				if((*sp).coord_x < 1){                          //Realiza a troca de posicao do sprite ao chegar no limite esquerdo da tela.
					(*sp).coord_x = 640;
				}
			}
			break;
		case 1:                                             //45 graus  (diagonal superior direita)
			if(refresh == 1){
				(*sp).coord_x += step_x; 						//Atualiza a coordenada X.
				(*sp).coord_y -= step_y; 						//Atualiza a coordenada Y.
				if((*sp).coord_y < 0){                          //Realiza a troca de posicao do sprite ao chegar no limite superior da tela.
					(*sp).coord_y = 480;
				}else if((*sp).coord_x > 640){                  //Realiza a troca de posicao do sprite ao chegar no limite direito da tela.
					(*sp).coord_x = 0;	
				}
			}
			break;
		case 2:                                             //90 graus  (pra cima)
			if(refresh == 1){
				(*sp).coord_y -= step_y; 						//Atualiza a coordenada Y.
				if((*sp).coord_y < 0){                          //Realiza a troca de posicao do sprite ao chegar no limite superior da tela.
					(*sp).coord_y = 480;
				}
			}
			break;
		case 3:          									//135 graus (diagonal superior esquerda)
			if(refresh == 1){
				(*sp).coord_x -= step_x; 						//Atualiza a coordenada X.
				(*sp).coord_y -= step_y; 						//Atualiza a coordenada Y.
				if((*sp).coord_y < 0){                          //Realiza a troca de posicao do sprite ao chegar no limite superior da tela.
					(*sp).coord_y = 480;
				}else if((*sp).coord_x < 1){                    //Realiza a troca de posicao do sprite ao chegar no limite esquerdo da tela.
					(*sp).coord_x = 640;
				}
			}
			break;
		case 4:          									//180 graus (direita)
			if(refresh == 1){
				(*sp).coord_x += step_x; 						//Atualiza a coordenada X.
				if((*sp).coord_x > 640){                        //Realiza a troca de posicao do sprite ao chegar no limite direito da tela.
					(*sp).coord_x = 0;
				}
			}

			break;
		case 5:          									//225 graus (diagonal inferior esquerda)
			if(refresh == 1){
				(*sp).coord_x -= step_x; 						//Atualiza a coordenada X.
				(*sp).coord_y += step_y; 						//Atualiza a coordenada Y.
				if((*sp).coord_y > 480){                        //Realiza a troca de posicao do sprite ao chegar no limite inferior da tela.
					(*sp).coord_y = 0;
				}else if((*sp).coord_x < 1){                    //Realiza a troca de posicao do sprite ao chegar no limite esquerdo da tela.
					(*sp).coord_x = 640;
				}				
			}
			break;
		case 6:                                              //270 graus (pra baixo)
			if(refresh == 1){
				(*sp).coord_y += step_y; 						//Atualiza a coordenada Y.
				if((*sp).coord_y > 480){                        //Realiza a troca de posicao do sprite ao chegar no limite inferior da tela.
					(*sp).coord_y = 0;
				}
			}
			break;
		case 7:                                             //315 graus (diagonal inferior direita)
			if(refresh == 1){
				(*sp).coord_x += step_x; 						//Atualiza a coordenada X.
				(*sp).coord_y += step_y; 						//Atualiza a coordenada Y.
				if((*sp).coord_y > 480){                        //Realiza a troca de posicao do sprite ao chegar no limite inferior da tela.
					(*sp).coord_y = 0;
				}else if((*sp).coord_x > 640){                  //Realiza a troca de posicao do sprite ao chegar no limite direito da tela.
					(*sp).coord_x = 0;
				}
			}
			break;
	}
	(*sp).inst_B  = position_instruction_builder((*sp).coord_x, (*sp).coord_y, (*sp).offset, (*sp).ativo);
}

int sendInstruction(unsigned long dataA, unsigned long dataB){
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

/* ==============================================================================
Funcao que define o barramento dataB da instrucao de modificar os dados de um sprite.
===================================================================================*/
unsigned long position_instruction_builder(int x, int y, int offset, int ativacao){
	unsigned long data = 0; 
	data = data | ativacao; //coloca o bit de ativacao do sprite.
    data = data << 10;    	//desloca o bit de ativacao em 10 posicoes a esquerda.
    data = data | x;      	//operacao OR com a variavel data. (insere x no barramento de bits)
    data = data << 10;    	//desloca os bits em 10 posicoes a esquerda.
    data = data | y;      	//operacao OR com a variavel data. (insere y no barramento de bits)
    data = data << 9;     	//desloca os bits em 9 posicoes a esquerda.
    data = data | offset; 	//insere o offset.
    return data;		
}

void initialize_sprite_random(int step_x, int step_y, int offset, Sprite *sp){
	(*sp).collision = 0;
	(*sp).coord_x   = 1 + rand() % 639;     //sorteio da posicao x.
	(*sp).coord_y   = 1 + rand() % 439;     //sorteio da posicao y.
	(*sp).offset    = offset;               //definicao do offset de memoria.
	(*sp).direction = rand() % 7;           //sorteio do anngulo inicial de movimento do sprite.
	(*sp).inst_B    = position_instruction_builder((*sp).coord_x, (*sp).coord_y, (*sp).offset, (*sp).ativo);
	(*sp).inst_A    = 0;
	(*sp).mask_x    = MASX_TO_SHIFT_X & (step_x << 19);
	(*sp).mask_y    = MASX_TO_SHIFT_Y & (step_y << 9);
}

void initialize_sprite_fixe(int x, int y, int step_x, int step_y, int offset, Sprite *sp){
	(*sp).collision = 0;
	(*sp).coord_x   = x;
	(*sp).coord_y   = y;
	(*sp).offset    = offset;               //definicao do offset de memoria.
	(*sp).direction = 0;
	(*sp).inst_B    = position_instruction_builder((*sp).coord_x, (*sp).coord_y, (*sp).offset, (*sp).ativo);
	(*sp).inst_A    = 0;
	(*sp).mask_x    = MASX_TO_SHIFT_X & (step_x << 19);
	(*sp).mask_y    = MASX_TO_SHIFT_Y & (step_y << 9);
}


unsigned long dataA_builder(int opcode, int reg, int memory_address){
	unsigned long data = 0b00000000000000000000000000000000;
	switch(opcode){
		case(0):                                //instrucao de escrita no banco de registradores.
			data = data | reg;                  //operacao OR (adiciona o numero do registrador)
			data = data << 4;                   //deslocamento a esquerda em 4 posicoes.
			data = data | opcode;               //operacao OR (adiciona o opcode).
			break;
		case(1):                                //instrucao de escrita na memoria de sprites.
			data = data | memory_address;       //operacao OR (adiciona o endereco de memoria)
			data = data << 4;                   //deslocamento a esquerda em 4 posicoes.
			data = data | opcode;               //operacao OR (adiciona o opcode).
			break;
	}
	return data;
}


