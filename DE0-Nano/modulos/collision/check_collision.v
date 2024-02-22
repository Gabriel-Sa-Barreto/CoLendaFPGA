/**
//////////////////////////////////////////////////////////////////////////
     AUTHOR: Gabriel Sa Barreto Alves
DESCRIPTION: Module to analyzes the collision between two sprites. 
--------------------------------------------------------------------------
INPUTS: 
    register: Input bus that contains the datas of comparison sprite.
      data,   Input bus that contains the sprite datas to be compared.
OUPUTS:
	     out: logic result of the comparison between the sprites.
//////////////////////////////////////////////////////////////////////////
**/
module check_collision(
	input wire [31:0] register,
	input wire [31:0] data,
	output reg out
);

reg [9:0] x1;
reg [9:0] y1;

reg [9:0] x2;
reg [9:0] y2;

reg [9:0] y_face_1;
reg [9:0] y_face_2;

reg [9:0] x_face_1;
reg [9:0] x_face_2;

/*----------Registers of results-----------*/
reg up_collision;
reg down_collision;
reg left_collision;
reg right_collision;
/*-----------------------------------------*/

localparam [9:0] h1 = 20;
localparam [9:0] h2 = 20;
localparam [9:0] w1 = 20;
localparam [9:0] w2 = 20;

/*--------------Equations-------------------
Equations: h = height w = width
yl = [ (y1 + h1) > y2 ] && [ y1 < (y2 + h2) ] left collision for y
yr = [ (y1 + h1) > y2 ] && [ y1 < (y2 + h2) ] right collision for  y

xl = [ (x1 + w1) > x2 ] left collision for x
xr = [ (x2 + w2) > x1 ] right collision for x 

up collision:
first check if: y1 > y2
after check if: [ (x1 + w1) > x2 ] && [ x1 < (x2 + w2) ]

 
down colision:
first check if: y1 < (y2 + h2)
after check if: [ (x1 + w1) > x2 ] && [ x1 < (x2 + w2) ]
*/

always @(data or register) begin
	up_collision   = 1'b0;
	down_collision = 1'b0; 
	left_collision  = 1'b0;
	right_collision = 1'b0; 
	/*-------Sprite Coordenate Datas--------*/
	x1 = data[28:19];
	x2 = register[28:19];
	y1 = data[18:9];
	y2 = register[18:9];
	y_face_1 = y1 + h1;
	y_face_2 = y2 + h2;
	x_face_1 = x1 + w1;
	x_face_2 = x2 + w2;
	/*-------------------------------------*/
	/*-------Left or Right Collision-------*/   
	if( (y_face_1 > y2) && (y1 < (y_face_2)) ) begin
		if( (x_face_1) > x2 ) begin
			left_collision = 1'b1;    //left collision
		end
		else if( (x_face_2) > x1 ) begin
			right_collision = 1'b1;  //right collision
		end
		else begin
			left_collision   = 1'b0;
			right_collision = 1'b0;
		end
	end
	else begin
		left_collision   = 1'b0;
		right_collision = 1'b0;
	end
	/*-------END Left or Right Collision-------*/
	/*----------Up or Down Collision-----------*/ 
	if( (x_face_1 > x2) && (x1 < x_face_2) ) begin
		if( y1 > y2 ) begin
			up_collision = 1'b1;     //up collision		
		end
		else if( y1 < y_face_2) begin
			down_collision = 1'b1;   //down collision
		end
		else begin
			up_collision   = 1'b0;
			down_collision = 1'b0;
		end
	end
	else begin
		up_collision   = 1'b0;
		down_collision = 1'b0;
	end
	/*-------END Up or Down Collision---------*/
	/*------------Checks if any collision was detected---------------*/
	out = up_collision || down_collision || left_collision || right_collision;
end
endmodule