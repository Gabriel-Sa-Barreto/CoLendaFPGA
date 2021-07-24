/**
//////////////////////////////////////////////////////////////////////////
     AUTHOR: Gabriel Sa Barreto Alves
DESCRIPTION: Module to controls the collision process between all sprites.
--------------------------------------------------------------------------
INPUTS:
    clk:    Frequency pulse for the module.() 
	enable: Pulse to activate the collision analyze process.

OUPUTS:
	enable_refresh_flags:        Signal for activate the update of the collision flags.
	sprite_selector:             Code that select the sprite to will be read.
	number_of_comparison_sprite: Number that represents which the current comparison sprite was caught for the analyze process.
	number_of_mobile_sprite:     Number that representes which the current mobile sprite was caught for the analyze process.
 	out_m_sprite:                Data bus of the mobile sprite.
	out_c_sprite:                Data bus of the comparison sprite.
//////////////////////////////////////////////////////////////////////////
**/
module collision_controler #(parameter bits_to_select_sprite = 5, bits_to_sprite_data = 32, n_m_sprite = 15, n_sprite = 32)
(
	input  wire                             clk,
	input  wire                             reset,
	input  wire                             enable,
	input  wire [bits_to_sprite_data-1:0]   sprite,
	output reg                              reset_refresh_mod,
	output reg                              enable_refresh_flags,
	output reg                              analyze_process_finished,
	output reg  [bits_to_select_sprite-1:0] sprite_selector,
	output reg  [bits_to_select_sprite-1:0] number_of_comparison_sprite,
	output reg  [bits_to_select_sprite-1:0] number_of_mobile_sprite,
 	output reg  [bits_to_sprite_data-1:0]   out_m_sprite,
	output reg  [bits_to_sprite_data-1:0]   out_c_sprite
);
 
localparam [3:0]     WAITING             = 3'b0000, // state in which the controller is waiting for activation. 
				     BEFORE_FIRST        = 3'b0001,
				     FIRST_READ          = 3'b0010, // state for reads the data from main sprite data.
				     BEFORE_SECOND       = 3'b0011,
				     SECOND_READ         = 3'b0100, // state for reads the data from the comparison sprite.
				     CHECK_COLLISION     = 3'b0101, // state for analyzes the collision datas.
				     BEFORE_REFRESH      = 3'b0110,
				     REFRESH_FLAG        = 3'b0111, // state to updates the regiters of the mobile and fixed sprites, in addition to activating the update of the Collision Flags.
				     FINISHED            = 4'b1000;        
				     
				     

reg [3:0]                       next, state;
reg [bits_to_select_sprite-1:0] mobile_sprite_number;
reg [bits_to_select_sprite-1:0] number_of_sprite;
reg [bits_to_sprite_data-1:0]   first_reading;

reg [bits_to_select_sprite-1:0] aux_mobile_sprite_number;
reg [bits_to_select_sprite-1:0] aux_number_of_sprite;
reg [bits_to_select_sprite-1:0] mobile;
reg [bits_to_select_sprite-1:0] comparison;
reg                             aux_selector, aux_selector_ms; // 0 - For mobile sprite./1 - For comparison sprite.
reg                             enable_refresh_mod;

localparam [bits_to_select_sprite-1:0] add_sprite       = 1;
localparam [bits_to_select_sprite-1:0] default_sprite   = 0;  // default value to analyzes the sprites. 
localparam [bits_to_select_sprite-1:0] default_selector = 0;  // default value to the sprites selector.

always @(posedge clk or negedge reset) begin
	if(!reset) state <= WAITING;
	else       state <= next;
end

/*-------Combinational Part for analyze the change of states------*/
always @(state or enable or number_of_sprite or mobile_sprite_number or first_reading[29] or sprite[29]) begin
	next = 4'bxxxx;
	case(state)
		WAITING: begin
			if(!enable) next = WAITING;
			else        next = FIRST_READ;
			aux_selector_ms          = 1'b0;
			enable_refresh_mod       = 1'b0;
			aux_number_of_sprite     = default_sprite;
			aux_mobile_sprite_number = default_sprite;
		end

		BEFORE_FIRST:  begin 
			next = FIRST_READ; 
			aux_selector_ms = 1'b0; 
			enable_refresh_mod = 1'b0; 
			aux_number_of_sprite = number_of_sprite; 
			aux_mobile_sprite_number = mobile_sprite_number;
		end
		
		FIRST_READ:    begin next = SECOND_READ;     aux_selector_ms = 1'b0; enable_refresh_mod = 1'b0; aux_number_of_sprite = number_of_sprite; aux_mobile_sprite_number = mobile_sprite_number; end 
		
		BEFORE_SECOND: begin 
			next = SECOND_READ; 
			aux_selector_ms = 1'b0; 
			enable_refresh_mod = 1'b0; 
			aux_number_of_sprite = number_of_sprite; 
			aux_mobile_sprite_number = mobile_sprite_number;
		end
		
		SECOND_READ:   begin next = CHECK_COLLISION; aux_selector_ms = 1'b0; enable_refresh_mod = 1'b0; aux_number_of_sprite = number_of_sprite; aux_mobile_sprite_number = mobile_sprite_number; end
		
		CHECK_COLLISION:     begin 
			aux_selector_ms          = 1'b0;
			enable_refresh_mod       = 1'b0;
			aux_number_of_sprite     = number_of_sprite;
			aux_mobile_sprite_number = mobile_sprite_number;
			// Checks whether the two read sprites are active for printing.
			if(first_reading[29] == 1'b0) begin
				/*----The comparisons between the sprite that was read in the "FIRST READ" was canceled-----*/
				//Will be changes in the mobile sprite.
				aux_selector_ms              = 1'b0; 
				// Checks if it was the last sprite.
				if(mobile_sprite_number == (n_m_sprite-1)) begin
					// The collision process was finished.
					aux_mobile_sprite_number = default_sprite;
					next                     = FINISHED; 
				end
				else begin
					aux_mobile_sprite_number = mobile_sprite_number + add_sprite;
					next                     = BEFORE_FIRST;
				end 
			end
			else begin
				next = BEFORE_REFRESH;
			end
		end

		BEFORE_REFRESH: begin 
			aux_selector_ms          = 1'b0;
			enable_refresh_mod       = 1'b0;
			aux_number_of_sprite     = number_of_sprite;
			aux_mobile_sprite_number = mobile_sprite_number;
			next = REFRESH_FLAG; 
		end

		REFRESH_FLAG: begin
			aux_selector_ms          = 1'b0;
			enable_refresh_mod       = 1'b0;
			aux_number_of_sprite     = number_of_sprite;
			aux_mobile_sprite_number = mobile_sprite_number;
			if(sprite[29] == 1'b1) begin
				if(mobile_sprite_number == default_sprite && number_of_sprite == default_sprite) begin
					enable_refresh_mod = 1'b0;	
				end
				else begin
					//  Enables the update of the Flags
					enable_refresh_mod = 1'b1;
				end
			end
			else begin
				enable_refresh_mod = 1'b0;
			end
			/*-----Checks the number of the next comparison sprite-----*/
			if(number_of_sprite < (n_sprite-1)) begin
				// The comparisons between the sprite that was read in the "FIRST READ" aren't yet over.
				next = BEFORE_SECOND;
				//Without changes in the mobile sprite.
				aux_selector_ms          = 1'b1; 
				//aux_mobile_sprite_number = mobile_sprite_number;
				if( (number_of_sprite + 1) == mobile_sprite_number )
					aux_number_of_sprite  = number_of_sprite + add_sprite + add_sprite;
				else
					aux_number_of_sprite  = number_of_sprite + add_sprite;
			end
			else begin
				/*----The comparisons between the sprite that was read in the "FIRST READ" was finished-----*/
				aux_number_of_sprite         = default_sprite;
				//Will be changes in the mobile sprite.
				aux_selector_ms              = 1'b0; 
				// Checks if it was the last sprite.
				if(mobile_sprite_number == (n_m_sprite-1)) begin
					// The collision process was finished.
					aux_mobile_sprite_number = default_sprite;
					next                     = FINISHED; 
				end
				else begin
					aux_mobile_sprite_number = mobile_sprite_number + add_sprite;
					next                     = BEFORE_FIRST;
				end 
			end
		end

		FINISHED: begin
			aux_selector_ms          = 1'b0;
			enable_refresh_mod       = 1'b0;
			aux_number_of_sprite     = number_of_sprite;
			aux_mobile_sprite_number = mobile_sprite_number;
			if(enable) begin
				next = FINISHED;
			end
			else begin
				next = WAITING;
			end
		end
		default: next = WAITING;
	endcase
end
/*----------------------------------------------------------------*/

/*--------Part to defines the output of each state----------------*/
always @(negedge clk or negedge reset) begin
	if(!reset) begin
		sprite_selector   		    <= default_selector;
		mobile_sprite_number        <= default_sprite;
		number_of_sprite            <= default_sprite;
		number_of_comparison_sprite <= default_sprite;
		number_of_mobile_sprite     <= default_sprite;
		enable_refresh_flags        <= 1'b0;
		aux_selector                <= 1'b0;
		analyze_process_finished    <= 1'b1;
		reset_refresh_mod           <= 1'b1;
	end
	else begin
		case(state)
			WAITING: begin
				sprite_selector   		    <= default_selector;
				mobile_sprite_number 		<= default_sprite;
				number_of_sprite     		<= default_sprite;
				number_of_comparison_sprite <= default_sprite;
				mobile                      <= default_sprite;
				comparison                  <= default_sprite;
				number_of_mobile_sprite     <= default_sprite;
				enable_refresh_flags 		<= enable_refresh_mod;
				analyze_process_finished    <= 1'b1; 
				aux_selector         		<= aux_selector_ms;
				reset_refresh_mod           <= 1'b0;
			end

			BEFORE_FIRST: begin
				mobile_sprite_number <= mobile;
				number_of_sprite     <= comparison;
				enable_refresh_flags <= 1'b0;
			end

			FIRST_READ: begin
				sprite_selector      		<= mobile_sprite_number;   // selects the mobile sprite to collision analyze.
				enable_refresh_flags 		<= enable_refresh_mod;     // Disable the update of the flags.
				analyze_process_finished    <= 1'b1;
				aux_selector         		<= aux_selector_ms;        // Identify change in the mobile sprite.
				number_of_comparison_sprite <= number_of_comparison_sprite;
				number_of_mobile_sprite     <= number_of_mobile_sprite;
				reset_refresh_mod           <= 1'b1;
			end

			BEFORE_SECOND: begin
				enable_refresh_flags <= 1'b0;
				mobile_sprite_number <= mobile;
				number_of_sprite     <= comparison;
			end

			SECOND_READ: begin
				first_reading        		<= (aux_selector == 1'b0) ? sprite: first_reading; // Receives the value of the mobile sprite.
				sprite_selector      		<= number_of_sprite;       // Changes the selector for catch the comparison sprite.
				enable_refresh_flags 		<= enable_refresh_mod;     // Disable the update of the flags.
				analyze_process_finished    <= 1'b1;
				number_of_comparison_sprite <= number_of_comparison_sprite;
				number_of_mobile_sprite     <= number_of_mobile_sprite;
				reset_refresh_mod           <= 1'b1;
			end

			CHECK_COLLISION: begin
				out_m_sprite         		<= first_reading;
				out_c_sprite         		<= sprite;                // Receives the value of the second reading.
				enable_refresh_flags 		<= enable_refresh_mod;    // Disable the update of the flags.
				analyze_process_finished    <= 1'b1;
				reset_refresh_mod           <= 1'b1;
				aux_selector         		<= aux_selector_ms;
				number_of_comparison_sprite <= number_of_sprite;
				number_of_mobile_sprite     <= mobile_sprite_number;
				mobile            <= aux_mobile_sprite_number;
				comparison        <= aux_number_of_sprite;
			end

			BEFORE_REFRESH: begin
				mobile_sprite_number <= mobile;
				number_of_sprite     <= comparison;
			end

			REFRESH_FLAG: begin
				enable_refresh_flags 		<= enable_refresh_mod;    // Enable the update of the collsion flags.
				analyze_process_finished    <= 1'b1;
				reset_refresh_mod           <= 1'b1;
				mobile 		                <= aux_mobile_sprite_number;
				comparison     		        <= aux_number_of_sprite;
				aux_selector         		<= aux_selector_ms;
			end 

			FINISHED: begin
				analyze_process_finished    <= 1'b0;                   // Indicates that the collision analyze process was finished.
				reset_refresh_mod           <= 1'b1;
				enable_refresh_flags 		<= enable_refresh_mod; 
			end

			default: begin
				sprite_selector   		    <= default_selector;
				mobile_sprite_number 		<= default_sprite;
				number_of_sprite     		<= default_sprite;
				number_of_comparison_sprite <= default_sprite;
				number_of_mobile_sprite     <= default_sprite;
				enable_refresh_flags 		<= enable_refresh_mod;
				analyze_process_finished    <= 1'b1;
				reset_refresh_mod           <= 1'b1;
			end
		endcase
	end
end
/*----------------------------------------------------------------*/
endmodule