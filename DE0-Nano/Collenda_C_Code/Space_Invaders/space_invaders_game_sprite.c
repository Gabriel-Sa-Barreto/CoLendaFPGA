#include "stdio.h"
#include "altera_avalon_pio_regs.h"
#include "system.h"
#include "stdlib.h"
#include "time.h"
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
/* =========================================================
Configuracao do barramento de botoes:
UP:               move_buttons[1] = GPIO_111 = PIN_R10
LEFT:             move_buttons[2] = GPIO_113 = PIN_P9 
RIGHT:            move_buttons[3] = GPIO_115 = PIN_N11
ATIRAR:           direction_and_shoot[0] = GPIO_117 = PIN_K16
============================================================ */

/* ==========================================================
OFFSET:
Aliens: 1, 2, 3, 4
Nave: 0       
Tiro: 5     
============================================================ */
/*-------------------PROTOTIPO DAS FUNCOES-------------------------*/
/*-----------------------------------------------------------------*/

/*----------------PROCEDIMENTOS--------------------------------*/
void atirar_sp(Sprite *nave, Sprite *tiro);
void atirar_al(Sprite *al, Sprite *tiro);
void collision_between_tiro_and_al(Sprite *tiro, Sprite* enemies[]);
void wait(int limit);

int main(){
	srand(time(NULL));
	/*======================= Sprite Variables ========================*/
	Sprite spaceship;
	Sprite alien_1, alien_2, alien_3, alien_4, alien_5, alien_6, alien_7, alien_8, alien_9, alien_10, alien_11, alien_12, alien_13;
	Sprite ship_tiro, ship_tiro2;
	Sprite alien_shoot, alien_shoot_2, alien_shoot_3, alien_shoot_4;
	/*================= Set Register for each sprite ==================*/
	spaceship.data_register	 = 1;
	alien_1.data_register  	 = 2;
	alien_2.data_register  	 = 3;
	alien_3.data_register  	 = 4;
	alien_4.data_register  	 = 5;
	alien_5.data_register  	 = 6;
	alien_6.data_register  	 = 7;
	alien_7.data_register  	 = 8;
	alien_8.data_register  	 = 9;
	alien_9.data_register  	 = 10;
	alien_10.data_register   = 11;
	alien_11.data_register   = 12;
	alien_12.data_register   = 13;
	alien_13.data_register   = 14;
	ship_tiro.data_register  = 15;
	ship_tiro2.data_register = 16;
	alien_shoot.data_register   = 17;
	alien_shoot_2.data_register = 18;
	alien_shoot_3.data_register = 19;
	alien_shoot_4.data_register = 20;
	/*====================== Set sprite informations ====================*/
	spaceship.coord_x = 310; spaceship.coord_y = 300; spaceship.offset  = 0;  spaceship.ativo 	 = 1;
	spaceship.step_x  = 5; 	 spaceship.step_y  = 5; spaceship.direction = LEFT; spaceship.collision = 0;
	alien_1.coord_x  = 140; alien_1.coord_y  = 50;  alien_1.offset  = 3; alien_1.ativo  = 1; alien_1.direction  = DOWN;
	alien_2.coord_x  = 180; alien_2.coord_y  = 50;  alien_2.offset  = 3; alien_2.ativo  = 1; alien_2.direction  = DOWN;
	alien_3.coord_x  = 290; alien_3.coord_y  = 50;  alien_3.offset  = 3; alien_3.ativo  = 1; alien_3.direction  = DOWN;
	alien_4.coord_x  = 330; alien_4.coord_y  = 50;  alien_4.offset  = 3; alien_4.ativo  = 1; alien_4.direction  = DOWN;
	alien_5.coord_x  = 480; alien_5.coord_y  = 50;  alien_5.offset  = 3; alien_5.ativo  = 1; alien_5.direction  = DOWN;
	alien_6.coord_x  = 520; alien_6.coord_y  = 50;  alien_6.offset  = 3; alien_6.ativo  = 1; alien_6.direction  = DOWN;
	alien_7.coord_x  = 140; alien_7.coord_y  = 150; alien_7.offset  = 1; alien_7.ativo  = 1; alien_7.direction  = DOWN;
	alien_8.coord_x  = 180; alien_8.coord_y  = 150; alien_8.offset  = 1; alien_8.ativo  = 1; alien_8.direction  = DOWN;
	alien_9.coord_x  = 290; alien_9.coord_y  = 150; alien_9.offset  = 1; alien_9.ativo  = 1; alien_9.direction  = DOWN;
	alien_10.coord_x = 330; alien_10.coord_y = 150; alien_10.offset = 1; alien_10.ativo = 1; alien_10.direction = DOWN;
	alien_11.coord_x = 480; alien_11.coord_y = 150; alien_11.offset = 1; alien_11.ativo = 1; alien_11.direction = DOWN;
	alien_12.coord_x = 520; alien_12.coord_y = 150; alien_12.offset = 1; alien_12.ativo = 1; alien_12.direction = DOWN;
	alien_13.coord_x = 310; alien_13.coord_y = 200; alien_13.offset = 1; alien_13.ativo = 1; alien_13.direction = DOWN;
	/*=================================================================*/
	/*============== Define the shoot sprites initial data .===============*/
	ship_tiro.step_y = 5;  ship_tiro2.step_y = 5;
	ship_tiro.offset = 5; ship_tiro2.offset  = 5;
	ship_tiro.ativo  = 0;  ship_tiro2.ativo  = 0;

	alien_shoot.step_y = 7;  alien_shoot_2.step_y = 7;  alien_shoot_3.step_y = 7;  alien_shoot_4.step_y = 7; 
	alien_shoot.offset = 5;  alien_shoot_2.offset = 5;  alien_shoot_3.offset = 5; alien_shoot_4.offset  = 5;
	alien_shoot.ativo  = 0;  alien_shoot_2.ativo  = 0;  alien_shoot_3.ativo  = 0;  alien_shoot_4.ativo  = 0; 
	/*=================================================================*/
	/* ====================Set alien sprite on screen ====================*/
	set_sprite(alien_1.data_register, alien_1.coord_x, alien_1.coord_y, alien_1.offset, alien_13.ativo);
	set_sprite(alien_2.data_register, alien_2.coord_x, alien_2.coord_y, alien_2.offset, alien_13.ativo);
	set_sprite(alien_3.data_register, alien_3.coord_x, alien_3.coord_y, alien_3.offset, alien_13.ativo);
	set_sprite(alien_4.data_register, alien_4.coord_x, alien_4.coord_y, alien_4.offset, alien_13.ativo);
	set_sprite(alien_5.data_register, alien_5.coord_x, alien_5.coord_y, alien_5.offset, alien_13.ativo);
	set_sprite(alien_6.data_register, alien_6.coord_x, alien_6.coord_y, alien_6.offset, alien_13.ativo);
	set_sprite(alien_7.data_register, alien_7.coord_x, alien_7.coord_y, alien_7.offset, alien_13.ativo);
	set_sprite(alien_8.data_register, alien_8.coord_x, alien_8.coord_y, alien_8.offset, alien_13.ativo);
	set_sprite(alien_9.data_register, alien_9.coord_x, alien_9.coord_y, alien_9.offset, alien_13.ativo);
	set_sprite(alien_10.data_register, alien_10.coord_x, alien_10.coord_y, alien_10.offset, alien_13.ativo);
	set_sprite(alien_11.data_register, alien_11.coord_x, alien_11.coord_y, alien_11.offset, alien_13.ativo);
	set_sprite(alien_12.data_register, alien_12.coord_x, alien_12.coord_y, alien_12.offset, alien_13.ativo);
	set_sprite(alien_13.data_register, alien_13.coord_x, alien_13.coord_y, alien_13.offset, alien_13.ativo);
	/*=================================================================*/
	while(IORD(WRFULL_BASE,0) != 0){}
	set_sprite(spaceship.data_register, spaceship.coord_x, spaceship.coord_y, spaceship.offset, spaceship.ativo);	
	/*==================== Build Scenario of Game =====================*/
	int line_block   = 0;  // Line of background block.
	int column_block = 0;  // Column of background block.
	int count 	 	 = 0;
 	int count_inst	 = 0;
	//--------------------------------------------------------------------
	// Set limit in the inferior part of screen --------------------------
 	line_block	 = 52;
 	column_block = 0;
 	while(line_block <= 59){
 		while(column_block <= 79){
 			if(IORD(WRFULL_BASE,0) == 0){
 				if(line_block > 52){  
					set_background_block(column_block, line_block, 0, 0b100, 0b011);
				}else{
					set_background_block(column_block, line_block, 0, 0b011, 0b011);
				}
				column_block++;
			}
		}
		column_block = 0;
		line_block++;
 	}
 	//--------------------------------------------------------------------
 	// Set the objects that represent the mount of lifes -----------------
 	line_block   = 0;
 	column_block = 0;
 	count 		 = 0;
 	do{
 		count_inst = 0;
 		if(count == 0){ 		line_block = 54; column_block = 3; count_inst = 1;}
 		else if(count == 1){ 	line_block = 55; column_block = 2; count_inst = 3;}
 		else if(count == 2){ 	line_block = 56; column_block = 2; count_inst = 3;}
 		while(count_inst > 0){
 			if(IORD(WRFULL_BASE,0) == 0){
 				set_background_block(column_block, line_block, 0b111, 0b01, 0b001);
 				set_background_block(column_block + 6, line_block, 0b111, 0b001, 0b001);
 				column_block++;
 				count_inst--;
 			}
 		}
 		count++;
 	}while(count < 3);
 	// ------------------------------------------------------------------
	/*==================== End build of scenario ======================*/
	/*========= Variables for stores the status of bottoms ============*/
	int A    = 0;
	int move = 0;
	int start_pause = 0;
	int state_game  = 0;       //0 - In Working; 1 - Game Paused.
	int lifes       = 2;
	int screen_to_aliens = 15; // Variable that inform the mount of screen to refresh the aliens sprites
	int actived_aliens   = 13; // Variable that informs how many sprites are currently active.
	int increase_y       = 10;
	/*============================ LIFE LOOP  =============================*/
	while(lifes >= 0){
		/*========================= MAIN LOOP  ============================*/
		int screen_aliens    = 0;
		int screen_ship      = 0;
		int alien_offset     = 1;
		int alien_offset_2   = 3;
		int mount_steps      = 6;
		int refresh_ship     = 0;
		int number_of_aliens = 13;
		int wait_shoot       = 0;
		int wait_shoot_al    = 0;
		int wait_to_pause    = 0;
		int en_shoot         = 0; // Enable shoots for spaceship.
		int en_shoot_al      = 0; // Enable shoots for aliens.
		int en_pause         = 0;
		while(spaceship.collision == 0){
			A     = IORD(A_BASE,0);
			move  = IORD(DIRECTION_ANALOGIC_BASE,0);
			start_pause = IORD(START_BASE,0);
			if(IORD(SCREEN_BASE,0) == 1){ // Checks if a screen has finished drawing.
				// Structure for counter of screen and set parameters.
				screen_aliens++;          // Screen counter for refresh aliens.
				screen_ship++;			  // Screen counter for refresh spaceship.
				IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,1);
				IOWR_ALTERA_AVALON_PIO_DATA(RESET_PULSECOUNTER_BASE,0);
				wait_shoot++;
				wait_shoot_al++;
				wait_to_pause++;
				if(wait_to_pause == 29) {
					en_pause = 1;
				}
				if(wait_shoot == 59){
					//enable to possibilty for shoot.
					en_shoot = 1;
				}
				if(wait_shoot_al == 29){
					//enable to possibilty for shoot.
					en_shoot_al = 1;
				}
			}else{ // Collision Analizer
				Sprite* ENEMIES[13] = { &alien_1, &alien_2, &alien_3, &alien_4, &alien_5, &alien_6, &alien_7, &alien_8, &alien_9, &alien_10, &alien_11, &alien_12, &alien_13};
				// Loop for check whether exist collision between spaceship and aliens
				for(int i = 0; i < number_of_aliens; i++) {
					Sprite *enemy = ENEMIES[i];
					if((*enemy).ativo == 1){
						if(collision(&spaceship, enemy) == 1){
							// There is a collision. 
							// That means one life was lost
							spaceship.collision = 1;
							// Get out Loop For
							break; 
						}
					}
				}

				// check if exist collision between aliens shootes and the spaceship.
				if(alien_shoot.ativo   == 1 && alien_shoot.collision   == 0){ if(collision(&spaceship, &alien_shoot) == 1){   spaceship.collision = 1; }}
				if(alien_shoot_2.ativo == 1 && alien_shoot_2.collision == 0){ if(collision(&spaceship, &alien_shoot_2) == 1){ spaceship.collision = 1; }}
				if(alien_shoot_3.ativo == 1 && alien_shoot_3.collision == 0){ if(collision(&spaceship, &alien_shoot_3) == 1){ spaceship.collision = 1; }}
				if(alien_shoot_4.ativo == 1 && alien_shoot_4.collision == 0){ if(collision(&spaceship, &alien_shoot_4) == 1){ spaceship.collision = 1; }}

				// check if exist collision between shoots and a active alien.
				if(ship_tiro.ativo  == 1 && ship_tiro.collision   == 0){ collision_between_tiro_and_al(&ship_tiro,  ENEMIES); }
				if(ship_tiro2.ativo == 1 && ship_tiro2.collision  == 0){ collision_between_tiro_and_al(&ship_tiro2, ENEMIES); }
			}
			/*-----------------------IF and PAUSE and START------------------------------------*/
			if(state_game == 0) {
				switch(move){
					case 0b0111:
						spaceship.direction = RIGHT;
						refresh_ship = 1;
						break;
					case 0b1011:
						spaceship.direction = LEFT;
						refresh_ship = 1;
						break;
					default:
						refresh_ship = 0;
						break;
				}
				if(en_shoot_al == 1){ // Check the possibilty for new shoots.
					en_shoot_al   = 0;
					wait_shoot_al = 0;
					Sprite *sp;
					switch((rand() % 13)){
						case 1: sp = &alien_1; break;
						case 2: sp = &alien_2; break;
						case 3: sp = &alien_3; break;
						case 4: sp = &alien_4; break;
						case 5: sp = &alien_5; break;
						case 6: sp = &alien_6; break;
						case 7: sp = &alien_7; break;
						case 8: sp = &alien_8; break;
						case 9: sp = &alien_9; break;
						case 10: sp = &alien_10; break;
						case 11: sp = &alien_11; break;
						case 12: sp = &alien_12; break;
						case 13: sp = &alien_13; break;
					}
					if(alien_shoot.ativo == 0 && (*sp).ativo == 1){        alien_shoot.ativo   = 1; atirar_al(sp, &alien_shoot);   }
					else if(alien_shoot_2.ativo == 0 && (*sp).ativo == 1){ alien_shoot_2.ativo = 1; atirar_al(sp, &alien_shoot_2); }
					else if(alien_shoot_3.ativo == 0 && (*sp).ativo == 1){ alien_shoot_3.ativo = 1; atirar_al(sp, &alien_shoot_3); }
					else if(alien_shoot_4.ativo == 0 && (*sp).ativo == 1){ alien_shoot_4.ativo = 1; atirar_al(sp, &alien_shoot_4); }
				}

				if(A == 1 && en_shoot == 1){ // Check the possibilty for new shoots.
					en_shoot   = 0;
					wait_shoot = 0;
					/*----- Conditional structure that checks which shootes are available for use..-----*/
					if(ship_tiro.ativo == 0){       ship_tiro.ativo  = 1; atirar_sp(&spaceship, &ship_tiro);  }
					else if(ship_tiro2.ativo == 0){ ship_tiro2.ativo = 1; atirar_sp(&spaceship, &ship_tiro2); }
					/*---------------------------------------------------------------------------------------------------*/
				}
			}
		    /* ============================== Refresh aliens sprites ====================================== */
			if(screen_aliens == screen_to_aliens){
				screen_aliens = 0;
				if(state_game == 0) {
					if(alien_1.coord_x < 10){ 
						mount_steps   = 6;
						alien_1.coord_y+= 5; alien_2.coord_y+= 5;  alien_3.coord_y+= 5;  alien_4.coord_y+= 5;
						alien_5.coord_y+= 5; alien_6.coord_y+= 5;  alien_7.coord_y+= 5;  alien_8.coord_y+= 5;
						alien_9.coord_y+= 5; alien_10.coord_y+= 5; alien_11.coord_y+= 5; alien_12.coord_y+= 5;
						alien_13.coord_y+= 5;
					}else if(alien_1.coord_x > 226) {
						mount_steps = -6;
						alien_1.coord_y+= 5; alien_2.coord_y+= 5;  alien_3.coord_y+= 5;  alien_4.coord_y+= 5;
						alien_5.coord_y+= 5; alien_6.coord_y+= 5;  alien_7.coord_y+= 5;  alien_8.coord_y+= 5;
						alien_9.coord_y+= 5; alien_10.coord_y+= 5; alien_11.coord_y+= 5; alien_12.coord_y+= 5;
						alien_13.coord_y+= 5;
					}
					 
					if(alien_1.collision == 1) {alien_1.ativo = 0; alien_1.collision = 0; actived_aliens--; } 
					set_sprite(alien_1.data_register, alien_1.coord_x, alien_1.coord_y, alien_1.offset, alien_1.ativo); alien_1.coord_x+= mount_steps; alien_1.offset = alien_offset_2; 
					
					if(alien_2.ativo == 1){ 
						if(alien_2.collision == 1) {alien_2.ativo = 0; alien_2.collision = 0; actived_aliens--;} 
						set_sprite(alien_2.data_register, alien_2.coord_x, alien_2.coord_y, alien_2.offset, alien_2.ativo); alien_2.coord_x+= mount_steps; alien_2.offset = alien_offset_2; 
					}
					if(alien_3.ativo == 1){
						if(alien_3.collision == 1) {alien_3.ativo = 0; alien_3.collision = 0; actived_aliens--;}
						set_sprite(alien_3.data_register, alien_3.coord_x, alien_3.coord_y, alien_3.offset, alien_3.ativo); alien_3.coord_x+= mount_steps; alien_3.offset = alien_offset_2; 
					}
					if(alien_4.ativo == 1){ 
						if(alien_4.collision == 1) {alien_4.ativo = 0; alien_4.collision = 0; actived_aliens--;}
						set_sprite(alien_4.data_register, alien_4.coord_x, alien_4.coord_y, alien_4.offset, alien_4.ativo); alien_4.coord_x+= mount_steps; alien_4.offset = alien_offset_2; 
					}
					if(alien_5.ativo == 1){ 
						if(alien_5.collision == 1) {alien_5.ativo = 0; alien_5.collision = 0; actived_aliens--;}
						set_sprite(alien_5.data_register, alien_5.coord_x, alien_5.coord_y, alien_5.offset, alien_5.ativo); alien_5.coord_x+= mount_steps; alien_5.offset = alien_offset_2; 
					}
					if(alien_6.ativo == 1){ 
						if(alien_6.collision == 1) {alien_6.ativo = 0; alien_6.collision = 0; actived_aliens--;}
						set_sprite(alien_6.data_register, alien_6.coord_x, alien_6.coord_y, alien_6.offset, alien_6.ativo); alien_6.coord_x+= mount_steps; alien_6.offset = alien_offset_2; 
					}
					if(alien_7.ativo == 1){ 
						if(alien_7.collision == 1) {alien_7.ativo = 0; alien_7.collision = 0; actived_aliens--;}
						set_sprite(alien_7.data_register, alien_7.coord_x, alien_7.coord_y, alien_7.offset, alien_7.ativo); alien_7.coord_x+= mount_steps; alien_7.offset = alien_offset; 
					}
					if(alien_8.ativo == 1){ 
						if(alien_8.collision == 1) {alien_8.ativo = 0; alien_8.collision = 0; actived_aliens--;}
						set_sprite(alien_8.data_register, alien_8.coord_x, alien_8.coord_y, alien_8.offset, alien_8.ativo); alien_8.coord_x+= mount_steps; alien_8.offset = alien_offset; 
					}
					if(alien_9.ativo == 1){ 
						if(alien_9.collision == 1) {alien_9.ativo = 0; alien_9.collision = 0; actived_aliens--;}
						set_sprite(alien_9.data_register, alien_9.coord_x, alien_9.coord_y, alien_9.offset, alien_9.ativo); alien_9.coord_x+= mount_steps; alien_9.offset = alien_offset; 
					}
					if(alien_10.ativo == 1){ 
						if(alien_10.collision == 1) {alien_10.ativo = 0; alien_10.collision = 0; actived_aliens--;}
						set_sprite(alien_10.data_register, alien_10.coord_x, alien_10.coord_y, alien_10.offset, alien_10.ativo); alien_10.coord_x+= mount_steps; alien_10.offset = alien_offset; 
					}
					if(alien_11.ativo == 1){ 
						if(alien_11.collision == 1) {alien_11.ativo = 0; alien_11.collision = 0; actived_aliens--;}
						set_sprite(alien_11.data_register, alien_11.coord_x, alien_11.coord_y, alien_11.offset, alien_11.ativo); alien_11.coord_x+= mount_steps; alien_11.offset = alien_offset; 
					}
					if(alien_12.ativo == 1){ 
						if(alien_12.collision == 1) {alien_12.ativo = 0; alien_12.collision = 0; actived_aliens--;}
						set_sprite(alien_12.data_register, alien_12.coord_x, alien_12.coord_y, alien_12.offset, alien_12.ativo); alien_12.coord_x+= mount_steps; alien_12.offset = alien_offset; 
					}
					if(alien_13.ativo == 1){ 
						if(alien_13.collision == 1) {alien_13.ativo = 0; alien_13.collision = 0; actived_aliens--;} 
						set_sprite(alien_13.data_register, alien_13.coord_x, alien_13.coord_y, alien_13.offset, alien_13.ativo); alien_13.coord_x+= mount_steps; alien_13.offset = alien_offset; 
					}
					
					if(alien_offset == 1) { alien_offset = 2;}
					else{ alien_offset = 1; }

					if(alien_offset_2 == 3) { alien_offset_2 = 4;}
					else{ alien_offset_2 = 3; }
				}
			}
			/* ============================= Refresh spaceship and shoots ==================================== */
			if(screen_ship == 2){
				screen_ship = 0;
				if(state_game == 0) {
					while(IORD(WRFULL_BASE,0) == 1){}; // Wait until the FIFO empty
					if(refresh_ship){
						refresh_ship = 0;
						increase_coordinate(&spaceship, 0);
						set_sprite(spaceship.data_register, spaceship.coord_x, spaceship.coord_y, spaceship.offset, spaceship.ativo);
					}
					if(ship_tiro.ativo == 1){
						if(ship_tiro.collision == 1 || ship_tiro.coord_y <= 0) {ship_tiro.ativo = 0; ship_tiro.collision = 0;}
						else{ increase_coordinate(&ship_tiro, 0); }
						set_sprite(ship_tiro.data_register,  ship_tiro.coord_x, ship_tiro.coord_y ,   ship_tiro.offset, ship_tiro.ativo);	
					}
					if(ship_tiro2.ativo == 1){
						if(ship_tiro2.collision == 1 || ship_tiro2.coord_y <= 0) {ship_tiro2.ativo = 0; ship_tiro2.collision = 0;}
						else { increase_coordinate(&ship_tiro2, 0); }
						set_sprite(ship_tiro2.data_register, ship_tiro2.coord_x, ship_tiro2.coord_y , ship_tiro2.offset, ship_tiro2.ativo);	
					}

					if(alien_shoot.ativo == 1){
						if(alien_shoot.collision == 1 || alien_shoot.coord_y >= 396) {alien_shoot.ativo = 0; alien_shoot.collision = 0;}
						else{ increase_coordinate(&alien_shoot, 0); }
						set_sprite(alien_shoot.data_register, alien_shoot.coord_x, alien_shoot.coord_y , alien_shoot.offset, alien_shoot.ativo);	
					}

					if(alien_shoot_2.ativo == 1){
						if(alien_shoot_2.collision == 1 || alien_shoot_2.coord_y >= 396) {alien_shoot_2.ativo = 0; alien_shoot_2.collision = 0;}
						else{ increase_coordinate(&alien_shoot_2, 0); }
						set_sprite(alien_shoot_2.data_register, alien_shoot_2.coord_x, alien_shoot_2.coord_y , alien_shoot_2.offset, alien_shoot_2.ativo);	
					}

					if(alien_shoot_3.ativo == 1){
						if(alien_shoot_3.collision == 1 || alien_shoot_3.coord_y >= 396) {alien_shoot_3.ativo = 0; alien_shoot_3.collision = 0;}
						else{ increase_coordinate(&alien_shoot_3, 0); }
						set_sprite(alien_shoot_3.data_register, alien_shoot_3.coord_x, alien_shoot_3.coord_y , alien_shoot_3.offset, alien_shoot_3.ativo);	
					}

					if(alien_shoot_4.ativo == 1){
						if(alien_shoot_4.collision == 1 || alien_shoot_4.coord_y >= 396) {alien_shoot_4.ativo = 0; alien_shoot_4.collision = 0;}
						else{ increase_coordinate(&alien_shoot_4, 0); }
						set_sprite(alien_shoot_4.data_register, alien_shoot_4.coord_x, alien_shoot_4.coord_y , alien_shoot_4.offset, alien_shoot_4.ativo);	
					}
				}
			}

			if(actived_aliens == 0){
				actived_aliens = 13;
				screen_aliens  = 0;
				screen_ship    = 0;
 				// Wait 4 seconds to next stage = 240 screens
 				wait(240);
 				// Increase the speed aliens
 				if(screen_to_aliens != 3){ screen_to_aliens-= 3; }
 				
				/*========================= Set sprite informations ======================*/
				spaceship.coord_x = 310; spaceship.coord_y = 300; spaceship.collision = 0;
				alien_1.coord_x  = 140; alien_1.coord_y  = 50  + increase_y;  alien_1.offset  = 3; alien_1.ativo  = 1; alien_1.collision  = 0;
				alien_2.coord_x  = 180; alien_2.coord_y  = 50  + increase_y;  alien_2.offset  = 3; alien_2.ativo  = 1; alien_2.collision  = 0;
				alien_3.coord_x  = 290; alien_3.coord_y  = 50  + increase_y;  alien_3.offset  = 3; alien_3.ativo  = 1; alien_3.collision  = 0;
				alien_4.coord_x  = 330; alien_4.coord_y  = 50  + increase_y;  alien_4.offset  = 3; alien_4.ativo  = 1; alien_4.collision  = 0;
				alien_5.coord_x  = 480; alien_5.coord_y  = 50  + increase_y;  alien_5.offset  = 3; alien_5.ativo  = 1; alien_5.collision  = 0;
				alien_6.coord_x  = 520; alien_6.coord_y  = 50  + increase_y;  alien_6.offset  = 3; alien_6.ativo  = 1; alien_6.collision  = 0;
				alien_7.coord_x  = 140; alien_7.coord_y  = 150 + increase_y;  alien_7.offset  = 1; alien_7.ativo  = 1; alien_7.collision  = 0;
				alien_8.coord_x  = 180; alien_8.coord_y  = 150 + increase_y;  alien_8.offset  = 1; alien_8.ativo  = 1; alien_8.collision  = 0;
				alien_9.coord_x  = 290; alien_9.coord_y  = 150 + increase_y;  alien_9.offset  = 1; alien_9.ativo  = 1; alien_9.collision  = 0;
				alien_10.coord_x = 330; alien_10.coord_y = 150 + increase_y;  alien_10.offset = 1; alien_10.ativo = 1; alien_10.collision = 0;
				alien_11.coord_x = 480; alien_11.coord_y = 150 + increase_y;  alien_11.offset = 1; alien_11.ativo = 1; alien_11.collision = 0;
				alien_12.coord_x = 520; alien_12.coord_y = 150 + increase_y;  alien_12.offset = 1; alien_12.ativo = 1; alien_12.collision = 0;
				alien_13.coord_x = 310; alien_13.coord_y = 200 + increase_y;  alien_13.offset = 1; alien_13.ativo = 1; alien_13.collision = 0;

				increase_y += 10;
 				
 				if(ship_tiro.ativo == 1){   ship_tiro.collision  = 1; }
 				if(ship_tiro2.ativo == 1){  ship_tiro2.collision = 1; }
 				if(alien_shoot.ativo == 1){   alien_shoot.collision   = 1; }
 				if(alien_shoot_2.ativo == 1){ alien_shoot_2.collision = 1; }
 				if(alien_shoot_3.ativo == 1){ alien_shoot_3.collision = 1; }
 				if(alien_shoot_4.ativo == 1){ alien_shoot_4.collision = 1; }
				while(IORD(WRFULL_BASE,0) != 0){}
				set_sprite(spaceship.data_register, spaceship.coord_x, spaceship.coord_y, spaceship.offset, spaceship.ativo);
			}
			/*====================== Process start and pause ====================*/
			if(en_pause == 1){
				if(start_pause == 1 && state_game == 0){ 
					state_game = 1; en_pause = 0; wait_to_pause = 0;
				}
				else if(start_pause == 1 && state_game == 1) { 
					state_game = 0; en_pause = 0; wait_to_pause = 0;
				}
			}
		} /*====================== END MAIN LOOP  =============================*/
		actived_aliens = 13;
		screen_aliens  = 0;
		screen_ship    = 0;
		// The player lost one life
		lifes--;
		// Reset the collision flag
		spaceship.collision = 0;
		// Erase one object that represent the mount of lifes -----------------
 		line_block   = 0;
 		column_block = 0;
 		count 		 = 0;
 		if(lifes >= 0){
	 		do{
	 			count_inst = 0;
	 			if(count == 0){ 		line_block = 54; column_block = 3; count_inst = 1;}
	 			else if(count == 1){ 	line_block = 55; column_block = 2; count_inst = 3;}
	 			else if(count == 2){ 	line_block = 56; column_block = 2; count_inst = 3;}
	 			while(count_inst > 0){
	 				if(IORD(WRFULL_BASE,0) == 0){
	 					if(lifes == 0){      set_background_block(column_block, line_block, 0, 0b100, 0b011); }
	 					else if(lifes == 1){ set_background_block(column_block + 6, line_block, 0, 0b100, 0b011);}
	 					column_block++;
	 					count_inst--;
	 				}
	 			}
	 		count++;
	 		}while(count < 3);

			/*====================== Restart sprite informations ====================*/
			spaceship.coord_x = 310; spaceship.coord_y = 300; spaceship.collision = 0;
			alien_1.coord_x  = 140; alien_1.coord_y  = 50 + increase_y;  alien_1.offset  = 3; alien_1.ativo  = 1; alien_1.collision  = 0;
			alien_2.coord_x  = 180; alien_2.coord_y  = 50 + increase_y;  alien_2.offset  = 3; alien_2.ativo  = 1; alien_2.collision  = 0;
			alien_3.coord_x  = 290; alien_3.coord_y  = 50 + increase_y;  alien_3.offset  = 3; alien_3.ativo  = 1; alien_3.collision  = 0;
			alien_4.coord_x  = 330; alien_4.coord_y  = 50 + increase_y;  alien_4.offset  = 3; alien_4.ativo  = 1; alien_4.collision  = 0;
			alien_5.coord_x  = 480; alien_5.coord_y  = 50 + increase_y;  alien_5.offset  = 3; alien_5.ativo  = 1; alien_5.collision  = 0;
			alien_6.coord_x  = 520; alien_6.coord_y  = 50 + increase_y;  alien_6.offset  = 3; alien_6.ativo  = 1; alien_6.collision  = 0;
			alien_7.coord_x  = 140; alien_7.coord_y  = 150 + increase_y; alien_7.offset  = 1; alien_7.ativo  = 1; alien_7.collision  = 0;
			alien_8.coord_x  = 180; alien_8.coord_y  = 150 + increase_y; alien_8.offset  = 1; alien_8.ativo  = 1; alien_8.collision  = 0;
			alien_9.coord_x  = 290; alien_9.coord_y  = 150 + increase_y; alien_9.offset  = 1; alien_9.ativo  = 1; alien_9.collision  = 0;
			alien_10.coord_x = 330; alien_10.coord_y = 150 + increase_y; alien_10.offset = 1; alien_10.ativo = 1; alien_10.collision = 0;
			alien_11.coord_x = 480; alien_11.coord_y = 150 + increase_y; alien_11.offset = 1; alien_11.ativo = 1; alien_11.collision = 0;
			alien_12.coord_x = 520; alien_12.coord_y = 150 + increase_y; alien_12.offset = 1; alien_12.ativo = 1; alien_12.collision = 0;
			alien_13.coord_x = 310; alien_13.coord_y = 200 + increase_y; alien_13.offset = 1; alien_13.ativo = 1; alien_13.collision = 0;
 			
 			if(ship_tiro.ativo == 1){   ship_tiro.collision  = 1; }
 			if(ship_tiro2.ativo == 1){  ship_tiro2.collision = 1; }
 			if(alien_shoot.ativo == 1){   alien_shoot.collision   = 1; }
 			if(alien_shoot_2.ativo == 1){ alien_shoot_2.collision = 1; }
 			if(alien_shoot_3.ativo == 1){ alien_shoot_3.collision = 1; }
 			if(alien_shoot_4.ativo == 1){ alien_shoot_4.collision = 1; }
 		}
 		// Wait 4 seconds to restart Game = 240 screens
 		wait(240);
		while(IORD(WRFULL_BASE,0) != 0){}
		set_sprite(spaceship.data_register, spaceship.coord_x, spaceship.coord_y, spaceship.offset, spaceship.ativo);
	}   /*====================== END LIFE LOOP  =============================*/
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

void atirar_al(Sprite *al, Sprite *tiro){
	(*tiro).coord_x   = (*al).coord_x;
	(*tiro).coord_y   = (*al).coord_y + 20;
	(*tiro).offset    = 5;
	(*tiro).direction = DOWN;
}

void atirar_sp(Sprite *nave, Sprite *tiro){
	(*tiro).coord_x   = (*nave).coord_x;
	(*tiro).coord_y   = (*nave).coord_y - 20;
	(*tiro).offset    = 5;
	(*tiro).direction = UP;
}

void collision_between_tiro_and_al(Sprite *tiro, Sprite* enemies[]){
	for (int i = 0; i < 13; i++){
		Sprite *al = enemies[i];
		if((*al).ativo == 1){
			if(collision(tiro, al) == 1){
				(*tiro).collision = 1;
				(*al).collision   = 1;
			}
		}
	}
}
