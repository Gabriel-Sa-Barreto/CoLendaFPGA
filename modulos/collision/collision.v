/**
//////////////////////////////////////////////////////////////////////////
     AUTHOR: Gabriel Sa Barreto Alves
DESCRIPTION: Module responsible to performance all collision analyze process.
--------------------------------------------------------------------------
INPUTS:
           clk: Frequency pulse for the module.
	mod_enable: Signal that activate the collision analyze process.
 s_r(0,1,2...): Bus data of the sprites storage in the registry file. 

OUPUTS:
	     flags: Bus that contains the collision flags to each sprite.   
//////////////////////////////////////////////////////////////////////////
**/
module collision #(parameter bits_to_select_sprite = 5)
(
	input wire         clk,
	input wire       reset,
	input wire  mod_enable,
	//---------Mobile sprite registers--------
	input wire [31:0] s_r0,
	input wire [31:0] s_r1,
	input wire [31:0] s_r2,
	input wire [31:0] s_r3,
	input wire [31:0] s_r4,
	input wire [31:0] s_r5,
	input wire [31:0] s_r6,
	input wire [31:0] s_r7,
	input wire [31:0] s_r8,
	input wire [31:0] s_r9,
	input wire [31:0] s_r10,
	input wire [31:0] s_r11,
	input wire [31:0] s_r12,
	input wire [31:0] s_r13,
	input wire [31:0] s_r14,
	//-----------------------------------------
	//---------Fixed sprite registers----------
	input wire [31:0] s_r15,
	input wire [31:0] s_r16,
	input wire [31:0] s_r17,
	input wire [31:0] s_r18,
	input wire [31:0] s_r19,
	input wire [31:0] s_r20,
	input wire [31:0] s_r21,
	input wire [31:0] s_r22,
	input wire [31:0] s_r23,
	input wire [31:0] s_r24,
	input wire [31:0] s_r25,
	input wire [31:0] s_r26,
	input wire [31:0] s_r27,
	input wire [31:0] s_r28,
	input wire [31:0] s_r29,
	input wire [31:0] s_r30,
	input wire [31:0] s_r31,
	//----------Collision Flags---------------
	output wire [29:0] flags
);

//Output bus of the mux_32x1
wire [31:0] mux_out;
//Selector input of the mux_32x1 
wire [4:0] mux_selector;
//Signal for activate the update the collision flags that correspond to mobile sprite that was be seing analyzed.
wire enabled_refresh;
//Signal for indicate that the collision analyze process for all sprites was finished.
wire process_finished;
//Data Bus that indicate the number of each sprite on the registry file. 
wire [bits_to_select_sprite-1:0] num_c_sprite; //comparison sprite
wire [bits_to_select_sprite-1:0] num_m_sprite; //only mobile sprite
//Two data buses for mobile and comparison sprites.
wire [31:0] mobile_sprite_data;
wire [31:0] comparison_sprite_data; 
//Signal of the combinational analyze of collision between two sprites.
wire collision_result;

wire reset_refresh_mod;

mux_32x1 mux_32x1_inst(
	 .reg_r0(s_r0),
	 .reg_r1(s_r1),
	 .reg_r2(s_r2),
	 .reg_r3(s_r3),
	 .reg_r4(s_r4),
	 .reg_r5(s_r5),
	 .reg_r6(s_r6),
	 .reg_r7(s_r7),
	 .reg_r8(s_r8),
	 .reg_r9(s_r9),
	.reg_r10(s_r10),
	.reg_r11(s_r11),
	.reg_r12(s_r12),
	.reg_r13(s_r13),
	.reg_r14(s_r14),
	.reg_r15(s_r15),
	.reg_r16(s_r16),
	.reg_r17(s_r17),
	.reg_r18(s_r18),
	.reg_r19(s_r19),
	.reg_r20(s_r20),
	.reg_r21(s_r21),
	.reg_r22(s_r22),
	.reg_r23(s_r23),
	.reg_r24(s_r24),
	.reg_r25(s_r25),
	.reg_r26(s_r26),
	.reg_r27(s_r27),
	.reg_r28(s_r28),
	.reg_r29(s_r29),
	.reg_r30(s_r30),
	.reg_r31(s_r31),
	.selector(mux_selector),
	.out_reg(mux_out)
);

collision_controler collision_controler_inst(
	//INPUTS------------------
	.clk(clk),
	.reset(reset),
	.enable(mod_enable),
	.sprite(mux_out),
	//OUTPUTS-----------------
	.reset_refresh_mod(reset_refresh_mod),
	.enable_refresh_flags(enabled_refresh),
	.analyze_process_finished(process_finished),
	.sprite_selector(mux_selector),
	.number_of_comparison_sprite(num_c_sprite),
	.number_of_mobile_sprite(num_m_sprite),
	.out_m_sprite(mobile_sprite_data),
	.out_c_sprite(comparison_sprite_data)
);

check_collision check_collision_inst(
	.register(comparison_sprite_data),
	.data(mobile_sprite_data),
	.out(collision_result)
);


refresh_flags refresh_flags_inst(
	.clk(clk),
	.enable(enabled_refresh),
	.reset(reset_refresh_mod),
	.process_finished(process_finished),
	.collision_result(collision_result),
	.number_of_comparison_sprite(num_c_sprite),
	.number_of_mobile_sprite(num_m_sprite),
	.collision_flags(flags)
);

endmodule