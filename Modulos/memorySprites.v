`timescale 1ns / 1ps
module memorySprites #(parameter ELEMENTS = 5)( 
                      input  wire                 clk,
							 input  wire                 reset,
							 input  wire                 read_enable, // 1 para ler um endereço de memoria do sprite
							 input  wire  [9:0]          address_sprite, 
							 input  wire  [ELEMENTS-1:0] element,
					       output reg   [11:0]        dataout);

reg [11:0] element1_palette [0:63];  //64 x 12-bit colour palette entries
reg [11:0] element2_palette [0:63];  //64 x 12-bit colour palette entries
reg [11:0] element3_palette [0:63];  //64 x 12-bit colour palette entries
reg [11:0] element4_palette [0:63];  //64 x 12-bit colour palette entries
reg [11:0] element5_palette [0:63];  //64 x 12-bit colour palette entries


reg read_enable1;
reg read_enable2;
reg read_enable3;
reg read_enable4;
reg read_enable5;
wire ready1;
wire ready2;
wire ready3;
wire ready4;
wire ready5;
wire [5:0] colour1;
wire [5:0] colour2;
wire [5:0] colour3;
wire [5:0] colour4;
wire [5:0] colour5;
reg [9:0]  address1;
reg [9:0]  address2;
reg [9:0]  address3;
reg [9:0]  address4;
reg [9:0]  address5;
							 
initial
begin
	$readmemh("/home/gabriel/Documentos/ConsoleFPGA/sprites/block/block_palette.mem", element5_palette);  // bitmap palette to load
end							 
							 
							 
/**********Parameter's list***********/
////////////////////////////////////////////
//Definiçao do espaço que a imagem ira ocupar
localparam element1_SPRITE_SIZE = 25;      //fruit sprites
localparam element2_SPRITE_SIZE = 25;      //heart sprites
localparam element3_SPRITE_SIZE = 20;      //snake
localparam element4_SPRITE_SIZE = 100;     //background
localparam element5_SPRITE_SIZE = 25;      //blocks
////////////////////////////////////////////
//Definiçao do numero de sprites armazenado no banco da memoria de sprites
localparam SPRITE_COUNT = 5;
////////////////////////////////////////////
//Número de itens que ira para a matriz de memoria que guardara cada sprite
localparam element1_VRAM_DEPTH = element1_SPRITE_SIZE * element1_SPRITE_SIZE;
localparam element2_VRAM_DEPTH = element2_SPRITE_SIZE * element2_SPRITE_SIZE;
localparam element3_VRAM_DEPTH = element3_SPRITE_SIZE * element3_SPRITE_SIZE;
localparam element4_VRAM_DEPTH = element4_SPRITE_SIZE * element4_SPRITE_SIZE; 
localparam element5_VRAM_DEPTH = element5_SPRITE_SIZE * element5_SPRITE_SIZE; 
////////////////////////////////////////////
////parametro correspondente ao numero maximo de endereços de cada sprites.
localparam element1_VRAM_A_WIDTH = 12; //25x25x6   = 3750 (2^12)
localparam element2_VRAM_A_WIDTH = 12; //25x25x6   = 3750 (2^12)
localparam element3_VRAM_A_WIDTH = 12; //20x20x6   = 2400 (2^12)
localparam element4_VRAM_A_WIDTH = 16; //100x100x6 = 60000 (2^16)
localparam element5_VRAM_A_WIDTH = 12; //25x25x6   = 3750 (2^12)  
////////////////////////////////////////////
//Quantidade de bits para a cor de um pixel dos sprites
localparam VRAM_D_WIDTH = 6;   
////////////////////////////////////////////
			
always @ (posedge clk) 
begin
	if(ready1)      read_enable1 <= 0;
	else if(ready2) read_enable2 <= 0;
	else if(ready3) read_enable3 <= 0;
	else if(ready4) read_enable4 <= 0;
	else            read_enable5 <= 0;
	
	if(read_enable) begin
		case(element)
			3'd1: 
			begin
				read_enable1 <= read_enable;
				address1 <= address_sprite;
			end
			3'd2: 
			begin
				read_enable2 <= read_enable;
				address2 <= address_sprite;
			end
			3'd3: 
			begin
				read_enable3 <= read_enable;
				address3 <= address_sprite;
			end
			3'd4: 
			begin
				read_enable4 <= read_enable;
				address4 <= address_sprite;
			end
			3'd5: 
			begin
				read_enable5 <= read_enable;
				address5 <= address_sprite;
			end
		endcase
	end
end	

always @ (*) 
begin
	if(ready1)      dataout <= element1_palette[colour1];
	else if(ready2) dataout <= element2_palette[colour2];
	else if(ready3) dataout <= element3_palette[colour3];
	else if(ready4) dataout <= element4_palette[colour4];
	else            dataout <= element5_palette[colour5];
end	
						 
sram #(     
		.ADDR_WIDTH(element1_VRAM_A_WIDTH), 
      .DATA_WIDTH(VRAM_D_WIDTH), 
      .DEPTH(element1_VRAM_DEPTH), 
      .MEMFILE(""))
   element1(
		.i_clk(clk) ,	         // input  clk_sig
		.i_addr(address1) ,	   // input [ADDR_WIDTH-1:0] i_addr_sig
		.i_read(read_enable1),	// input  read_sig
		.i_data(0) ,	         // input [DATA_WIDTH-1:0] i_data_sig
		.o_data(colour1), 	      // output   [DATA_WIDTH-1:0] data_sig
		.dataoutReady(ready1)   // output dataoutReady 
);


sram #(     
		.ADDR_WIDTH(element2_VRAM_A_WIDTH), 
      .DATA_WIDTH(VRAM_D_WIDTH), 
      .DEPTH(element2_VRAM_DEPTH), 
      .MEMFILE(""))
   element2(
		.i_clk(clk) ,	         
		.i_addr(address2) ,	   
		.i_read(read_enable2) ,	         
		.i_data(0) ,	         
		.o_data(colour2), 	
		.dataoutReady(ready2)	
);


sram #(     
		.ADDR_WIDTH(element3_VRAM_A_WIDTH), 
      .DATA_WIDTH(VRAM_D_WIDTH), 
      .DEPTH(element3_VRAM_DEPTH), 
      .MEMFILE(""))
   element3(
		.i_clk(clk) ,	         
		.i_addr(address3) ,	   
		.i_read(read_enable3),	         
		.i_data(0) ,	         
		.o_data(colour3),
		.dataoutReady(ready3)
);


sram #(     
		.ADDR_WIDTH(element4_VRAM_A_WIDTH), 
      .DATA_WIDTH(VRAM_D_WIDTH), 
      .DEPTH(element4_VRAM_DEPTH), 
      .MEMFILE(""))
   element4(
		.i_clk(clk) ,	        
		.i_addr(address4) ,	   
		.i_read(read_enable4) ,	         
		.i_data(0) ,	         
		.o_data(colour4),
		.dataoutReady(ready4)	
);

sram #(     
		.ADDR_WIDTH(element5_VRAM_A_WIDTH), 
      .DATA_WIDTH(VRAM_D_WIDTH), 
      .DEPTH(element5_VRAM_DEPTH), 
      .MEMFILE("/home/gabriel/Documentos/ConsoleFPGA/sprites/block/block.mem"))
   element5(
		.i_clk(clk) ,	        
		.i_addr(address5) ,	   
		.i_read(read_enable5) ,	         
		.i_data(0) ,		         
		.o_data(colour5),
		.dataoutReady(ready5)	
);

endmodule