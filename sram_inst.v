// Copyright (C) 2018  Intel Corporation. All rights reserved.
// Your use of Intel Corporation's design tools, logic functions 
// and other software and tools, and its AMPP partner logic 
// functions, and any output files from any of the foregoing 
// (including device programming or simulation files), and any 
// associated documentation or information are expressly subject 
// to the terms and conditions of the Intel Program License 
// Subscription Agreement, the Intel Quartus Prime License Agreement,
// the Intel FPGA IP License Agreement, or other applicable license
// agreement, including, without limitation, that your use is for
// the sole purpose of programming logic devices manufactured by
// Intel and sold by Intel or its authorized distributors.  Please
// refer to the applicable agreement for further details.


// Generated by Quartus Prime Version 18.1 (Build Build 625 09/12/2018)
// Created on Tue Jun  4 14:15:05 2019

sram sram_inst
(
	.i_clk(i_clk_sig) ,	// input  i_clk_sig
	.i_addr(i_addr_sig) ,	// input [ADDR_WIDTH-1:0] i_addr_sig
	.i_write(i_write_sig) ,	// input  i_write_sig
	.i_data(i_data_sig) ,	// input [DATA_WIDTH-1:0] i_data_sig
	.o_data(o_data_sig) 	// output [DATA_WIDTH-1:0] o_data_sig
);

defparam sram_inst.ADDR_WIDTH = 8;
defparam sram_inst.DATA_WIDTH = 8;
defparam sram_inst.DEPTH = 256;
//defparam sram_inst.MEMFILE = "";
