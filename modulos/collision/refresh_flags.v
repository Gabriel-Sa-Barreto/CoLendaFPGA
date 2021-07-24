/**
///////////////////////////////////////////////////////////////////////////
     AUTHOR: Gabriel Sa Barreto Alves
DESCRIPTION: Module to update the collision flags according to analyse process result that was done between two sprites.
--------------------------------------------------------------------------
INPUTS:
    clk:                         Frequency Pulse of the module.
	enable:                      Signal that activate the update of collision Flags.
	collision_result:            Signal to report if occurred a collision between two sprites. (1 - Yes/ 0 - No)
	number_of_comparison_sprite: Number that represents which the current comparison sprite was caught for the analyze process.
	number_of_mobile_sprite:     Number that representes which the current mobile sprite was caught for the analyze process.
OUPUTS:
	collision_flags:  Bus that contains the collision flags to every mobile sprite.
//////////////////////////////////////////////////////////////////////////
**/
module refresh_flags #(parameter bits_to_sprite = 5, bits_to_flags = 30, begin_mobile_sprite = 0, end_mobile_sprite = 14,  begin_fixed_sprite = 15, end_fixed_sprite = 31)
(
	input  wire                 	 clk,
	input  wire                      enable,
	input  wire                      reset,
	input  wire                      process_finished,
	input  wire 					 collision_result,
	input  wire [bits_to_sprite-1:0] number_of_comparison_sprite,
	input  wire [bits_to_sprite-1:0] number_of_mobile_sprite,
	output wire [bits_to_flags-1:0]  collision_flags
);

reg [bits_to_flags-1:0] flags;
reg [bits_to_flags-1:0] aux_flags;
reg [4:0]               begin_coord_flags;
reg [4:0]               end_coord_flags;
// Registry that storages the collision code.
reg [1:0]               aux_code;         
reg [bits_to_flags-1:0]     code;
/*------Collision Code------
00 - Without collision.
01 - Collision with a mobile sprite.
10 - Collision with fixed sprite.
11 - Collision between a mobile and fixed sprite at the same time.
*/

always @(enable or number_of_mobile_sprite or number_of_comparison_sprite or collision_result or aux_flags) begin
	aux_code = 2'b00;
	// Checks if the update was activate.
	if(enable) begin
		code = aux_flags;
		// Checks if occurred a collision.
		if(collision_result) begin
			// Checks if the collision occurred between two mobile sprite.
			if(number_of_comparison_sprite >= begin_mobile_sprite && number_of_comparison_sprite <= end_mobile_sprite) begin
				// Occurred a collision with a mobile sprite.
				aux_code = 2'b01;
			end // Checks if the collision occurred with fixed sprite.
			else if(number_of_comparison_sprite >= begin_fixed_sprite) begin
				// Occurred a collision with a fixed sprite.
				aux_code = 2'b10;
			end
			else
				aux_code = 2'b00;

			case(number_of_mobile_sprite)
				5'd0: begin 
					// Checks if occurred two collisions at same time.
					if(aux_flags[1:0] == 2'b01 && aux_code == 2'b10) begin
						code[1:0] = 2'b11;
					end
					else if(aux_flags[1:0] == 2'b10 && aux_code == 2'b01) begin
						code[1:0] = 2'b11;
					end
					else begin
						code[1:0] = aux_code; 
					end
				end   
				5'd1:    begin 
					// Checks if occurred two collisions at same time.
					if(aux_flags[3:2] == 2'b01 && aux_code == 2'b10) begin
						code[3:2] = 2'b11;
					end
					else if(aux_flags[3:2] == 2'b10 && aux_code == 2'b01) begin
						code[3:2] = 2'b11;
					end
					else begin
						code[3:2] = aux_code; 
					end 
				end
				5'd2:    begin 
					// Checks if occurred two collisions at same time.
					if(aux_flags[5:4] == 2'b01 && aux_code == 2'b10) begin
						code[5:4] = 2'b11;
					end
					else if(aux_flags[1:0] == 2'b10 && aux_code == 2'b01) begin
						code[5:4] = 2'b11;
					end
					else begin
						code[5:4] = aux_code; 
					end
				end
				5'd3:    begin 
					// Checks if occurred two collisions at same time.
					if(aux_flags[7:6] == 2'b01 && aux_code == 2'b10) begin
						code[7:6] = 2'b11;
					end
					else if(aux_flags[7:6] == 2'b10 && aux_code == 2'b01) begin
						code[7:6] = 2'b11;
					end
					else begin
						code[7:6] = aux_code; 
					end
				end
				5'd4:    begin 
					// Checks if occurred two collisions at same time.
					if(aux_flags[9:8] == 2'b01 && aux_code == 2'b10) begin
						code[9:8] = 2'b11;
					end
					else if(aux_flags[9:8] == 2'b10 && aux_code == 2'b01) begin
						code[9:8] = 2'b11;
					end
					else begin
						code[9:8] = aux_code; 
					end
				end
				5'd5:    begin 
					// Checks if occurred two collisions at same time.
					if(aux_flags[11:10] == 2'b01 && aux_code == 2'b10) begin
						code[11:10] = 2'b11;
					end
					else if(aux_flags[11:10] == 2'b10 && aux_code == 2'b01) begin
						code[11:10] = 2'b11;
					end
					else begin
						code[11:10] = aux_code; 
					end
				end
				5'd6:    begin 
					// Checks if occurred two collisions at same time.
					if(aux_flags[13:12] == 2'b01 && aux_code == 2'b10) begin
						code[13:12] = 2'b11;
					end
					else if(aux_flags[13:12] == 2'b10 && aux_code == 2'b01) begin
						code[13:12] = 2'b11;
					end
					else begin
						code[13:12] = aux_code; 
					end
				end
				5'd7:    begin 
					// Checks if occurred two collisions at same time.
					if(aux_flags[15:14] == 2'b01 && aux_code == 2'b10) begin
						code[15:14] = 2'b11;
					end
					else if(aux_flags[15:14] == 2'b10 && aux_code == 2'b01) begin
						code[15:14] = 2'b11;
					end
					else begin
						code[15:14] = aux_code; 
					end
				end
				5'd8:    begin 
					// Checks if occurred two collisions at same time.
					if(aux_flags[17:16] == 2'b01 && aux_code == 2'b10) begin
						code[17:16] = 2'b11;
					end
					else if(aux_flags[17:16] == 2'b10 && aux_code == 2'b01) begin
						code[17:16] = 2'b11;
					end
					else begin
						code[17:16] = aux_code; 
					end
				end
				5'd9:    begin 
					// Checks if occurred two collisions at same time.
					if(aux_flags[19:18] == 2'b01 && aux_code == 2'b10) begin
						code[19:18] = 2'b11;
					end
					else if(aux_flags[19:18] == 2'b10 && aux_code == 2'b01) begin
						code[19:18] = 2'b11;
					end
					else begin
						code[19:18] = aux_code; 
					end
				end
				5'd10:   begin 
					// Checks if occurred two collisions at same time.
					if(aux_flags[21:20] == 2'b01 && aux_code == 2'b10) begin
						code[21:20] = 2'b11;
					end
					else if(aux_flags[21:20] == 2'b10 && aux_code == 2'b01) begin
						code[21:20] = 2'b11;
					end
					else begin
						code[21:20] = aux_code; 
					end
				end
				5'd11:   begin 
					// Checks if occurred two collisions at same time.
					if(aux_flags[23:22] == 2'b01 && aux_code == 2'b10) begin
						code[23:22] = 2'b11;
					end
					else if(aux_flags[23:22] == 2'b10 && aux_code == 2'b01) begin
						code[23:22] = 2'b11;
					end
					else begin
						code[23:22] = aux_code; 
					end
				end
				5'd12:   begin 
					// Checks if occurred two collisions at same time.
					if(aux_flags[25:24] == 2'b01 && aux_code == 2'b10) begin
						code[25:24] = 2'b11;
					end
					else if(aux_flags[25:24] == 2'b10 && aux_code == 2'b01) begin
						code[25:24] = 2'b11;
					end
					else begin
						code[25:24] = aux_code; 
					end
				end
				5'd13:   begin 
					// Checks if occurred two collisions at same time.
					if(aux_flags[27:26] == 2'b01 && aux_code == 2'b10) begin
						code[27:26] = 2'b11;
					end
					else if(aux_flags[27:26] == 2'b10 && aux_code == 2'b01) begin
						code[27:26] = 2'b11;
					end
					else begin
						code[27:26] = aux_code; 
					end
				end
				5'd14:   begin 
					// Checks if occurred two collisions at same time.
					if(aux_flags[29:28] == 2'b01 && aux_code == 2'b10) begin
						code[29:28] = 2'b11;
					end
					else if(aux_flags[29:28] == 2'b10 && aux_code == 2'b01) begin
						code[29:28] = 2'b11;
					end
					else begin
						code[29:28] = aux_code; 
					end
				end
				default: begin 
					code = aux_flags;
				end
			endcase
		end
		else
			code = aux_flags;
	end
	else
		code = aux_flags;
end

always @(posedge clk or negedge reset) begin
	if(!reset) begin
		aux_flags <= 30'd0;
	end
	else begin
		if(!process_finished) begin
			// Update the collision flags and reset the auxiliar registry.
			flags <= aux_flags;
		end
		else begin
			if(enable) begin
			// Update the auxiliar registry of collision flags.
				aux_flags  <= code;
			end
			else begin
				aux_flags <= aux_flags;
			end
		end
	end
end


assign collision_flags = flags;
endmodule