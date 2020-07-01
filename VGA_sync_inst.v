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
// Created on Tue Apr 21 12:40:36 2020

VGA_sync VGA_sync_inst
(
	.clock(clock_sig) ,	// input  clock_sig
	.reset(reset_sig) ,	// input  reset_sig
	.hsync(hsync_sig) ,	// output  hsync_sig
	.vsync(vsync_sig) ,	// output  vsync_sig
	.video_enable(video_enable_sig) ,	// output  video_enable_sig
	.pixel_x(pixel_x_sig) ,	// output [9:0] pixel_x_sig
	.pixel_y(pixel_y_sig) 	// output [8:0] pixel_y_sig
);

defparam VGA_sync_inst.HD = 640;
defparam VGA_sync_inst.HF = 16;
defparam VGA_sync_inst.HB = 48;
defparam VGA_sync_inst.HR = 96;
defparam VGA_sync_inst.HT = 800;
defparam VGA_sync_inst.VD = 480;
defparam VGA_sync_inst.VF = 11;
defparam VGA_sync_inst.VB = 31;
defparam VGA_sync_inst.VR = 2;
defparam VGA_sync_inst.VT = 524;
