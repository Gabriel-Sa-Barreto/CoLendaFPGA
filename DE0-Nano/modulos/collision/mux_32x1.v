/**
//////////////////////////////////////////////////////////////////////////
     AUTHOR: Gabriel Sa Barreto Alves
DESCRIPTION: Module to select the data bus that the collision controller need. 
--------------------------------------------------------------------------
INPUTS: 
    reg_r(0,1,2...31): data bus
             selector: signal to select the data bus.
OUPUTS:
	out_reg: data bus that was selected.
//////////////////////////////////////////////////////////////////////////
**/
module mux_32x1 #(parameter bus_data = 32, bits_to_selector = 5)
(
	//---------Mobile sprite registers--------
	input wire [bus_data-1:0] reg_r0,
	input wire [bus_data-1:0] reg_r1,
	input wire [bus_data-1:0] reg_r2,
	input wire [bus_data-1:0] reg_r3,
	input wire [bus_data-1:0] reg_r4,
	input wire [bus_data-1:0] reg_r5,
	input wire [bus_data-1:0] reg_r6,
	input wire [bus_data-1:0] reg_r7,
	input wire [bus_data-1:0] reg_r8,
	input wire [bus_data-1:0] reg_r9,
	input wire [bus_data-1:0] reg_r10,
	input wire [bus_data-1:0] reg_r11,
	input wire [bus_data-1:0] reg_r12,
	input wire [bus_data-1:0] reg_r13,
	input wire [bus_data-1:0] reg_r14,
	//---------Fixed sprite registers----------
	input wire [bus_data-1:0] reg_r15,
	input wire [bus_data-1:0] reg_r16,
	input wire [bus_data-1:0] reg_r17,
	input wire [bus_data-1:0] reg_r18,
	input wire [bus_data-1:0] reg_r19,
	input wire [bus_data-1:0] reg_r20,
	input wire [bus_data-1:0] reg_r21,
	input wire [bus_data-1:0] reg_r22,
	input wire [bus_data-1:0] reg_r23,
	input wire [bus_data-1:0] reg_r24,
	input wire [bus_data-1:0] reg_r25,
	input wire [bus_data-1:0] reg_r26,
	input wire [bus_data-1:0] reg_r27,
	input wire [bus_data-1:0] reg_r28,
	input wire [bus_data-1:0] reg_r29,
	input wire [bus_data-1:0] reg_r30,
	input wire [bus_data-1:0] reg_r31,
	//-----------------------------------------
	input wire [bits_to_selector-1:0] selector,   
   output reg  [bus_data-1:0] out_reg
);


always @(*) begin
	case(selector)
		5'd0:    out_reg = reg_r0;   
		5'd1:    out_reg = reg_r1;  
		5'd2:    out_reg = reg_r2;   
		5'd3:    out_reg = reg_r3;   
		5'd4:    out_reg = reg_r4;    
		5'd5:    out_reg = reg_r5;   
		5'd6:    out_reg = reg_r6;   
		5'd7:    out_reg = reg_r7;   
		5'd8:    out_reg = reg_r8;  
		5'd9:    out_reg = reg_r9;  
		5'd10:   out_reg = reg_r10;   
		5'd11:   out_reg = reg_r11;  
		5'd12:   out_reg = reg_r12;  
		5'd13:   out_reg = reg_r13;
		5'd14:   out_reg = reg_r14;
		5'd15:   out_reg = reg_r15;
		5'd16:   out_reg = reg_r16;
		5'd17:   out_reg = reg_r17;
		5'd18:   out_reg = reg_r18;
		5'd19:   out_reg = reg_r19;
		5'd20:   out_reg = reg_r20;
		5'd21:   out_reg = reg_r21;
		5'd22:   out_reg = reg_r22;
		5'd23:   out_reg = reg_r23;
		5'd24:   out_reg = reg_r24;
		5'd25:   out_reg = reg_r25;
		5'd26:   out_reg = reg_r26;
		5'd27:   out_reg = reg_r27;
		5'd28:   out_reg = reg_r28;
		5'd29:   out_reg = reg_r29;
		5'd30:   out_reg = reg_r30;
		5'd31:   out_reg = reg_r31;  
		default: out_reg = 32'd0; 
	endcase
end

endmodule