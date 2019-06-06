module top(
	  input      clk,          //clock da FPGA (50MHz)
	  //input  wire leftSprite,	  //mover sprite para a esquerda
	  //input  wire rightSprite,    //mover sprite para a direita
	  input wire       sprite,
	  output reg  [2:0] VGA_R,   //intensidade de vermelho
	  output reg  [2:0] VGA_G,   //intensidade de verde
	  output reg  [2:0] VGA_B,   //intensidade do azul
	  output wire hsync,        //sinal de sincronizaçao horizontal
	  output wire vsync         //sinal de sincronizaçao vertical
);


wire	 video_enable;    //sinal de area ativa da tela para impressao
wire [10:0]	pixel_x;    //coordenada x do pixel atual da tela 
wire [9:0]  pixel_y;    //coordenada y do pixel atual da tela

reg [11:0] address;      //endereco da memoria
wire [11:0] dataout;     //saida de dados da memoria
initial begin
	address = 0;
end
//reg [9:0] sprite_x;
//reg [9:0] sprite_y;
//reg [2:0] choose_sprite;

SVGA_sync	SVGA(.clock(clk),
				 .reset(),
				 .hsync(hsync),
				 .vsync(vsync),
				 .video_enable(video_enable),
				 .pixel_x(pixel_x),
				 .pixel_y(pixel_y)
				);
/*
initial begin
	choose_sprite = 0;
	SPRITE_OFFSET = 0;
   $readmemh("/home/gabriel/Documentos/ConsoleFPGA/teste3_palette.mem", palette);  // bitmap palette to load
end				
				
// VRAM frame buffers (read-write)
////////////////////////////////////////////
localparam SCREEN_WIDTH = 800;
localparam SCREEN_HEIGHT = 600;
////////////////////////////////////////////
//Definiçao do espaço que a imagem ira ocupar
localparam SPRITE_SIZE = 32;
////////////////////////////////////////////
//Definiçao do numero de sprites armazenado na memoria
localparam SPRITE_COUNT = 8;
////////////////////////////////////////////
//Número de itens que ira para a matriz de memoria
localparam VRAM_DEPTH = SPRITE_SIZE * SPRITE_SIZE; //* SPRITE_COUNT; 
////////////////////////////////////////////
//// 2^13 e o valor correspondente a 32 x 32 x 8.
localparam VRAM_A_WIDTH = 10;  
////////////////////////////////////////////
//Quantidade de bits para a cor de um pixel
localparam VRAM_D_WIDTH = 8;   
////////////////////////////////////////////
reg [VRAM_A_WIDTH-1:0] address;      //endereco da memoria
wire [VRAM_D_WIDTH-1:0] dataout;     //saida de dados da memoria
///////////////////////////////////////////
reg[12:0] SPRITE_OFFSET;


sram #(
        .ADDR_WIDTH(VRAM_A_WIDTH), 
        .DATA_WIDTH(VRAM_D_WIDTH), 
        .DEPTH(VRAM_DEPTH), 
        .MEMFILE("/home/gabriel/Documentos/ConsoleFPGA/teste3.mem"))  // bitmap to load
        vram (
        .i_addr(address), 
        .i_clk(clk), 
        .i_write(0),   // we're always reading
        .i_data(0), 
        .o_data(dataout)
    );

reg [11:0] palette [0:63];  // 256 x 12-bit colour palette entries
reg [11:0] colour;

always @ (posedge sprite)
begin
	if(choose_sprite > 7) choose_sprite <= 0;
	else choose_sprite <= choose_sprite + 1;
	SPRITE_OFFSET <= choose_sprite * SPRITE_SIZE * SPRITE_SIZE;
	
end

always @ (posedge clk)
    begin
		  address <= SPRITE_OFFSET + (SPRITE_SIZE * sprite_y) + sprite_x;
        if (video_enable)
				begin
					buildSprite(400,300);
				end
        else  colour <= 0;
        VGA_R <= colour[11:9];
        VGA_G <= colour[7:5];
        VGA_B <= colour[3:1];
    end			 
	 
task buildSprite;
	input [9:0] x;
	input [9:0] y;
	begin
		if( (pixel_x >= x && pixel_x <= (x + SPRITE_SIZE) ) && (pixel_y >= y && pixel_y <= (y + SPRITE_SIZE) ) )
		begin
			if(sprite_y > SPRITE_SIZE) 
			begin
				sprite_y <= 0;
				sprite_x <= 0;
			end
			if(sprite_x > SPRITE_SIZE-1) 
			begin
				sprite_x <= 0;
				sprite_y <= sprite_y + 1;
			end
			else sprite_x <= sprite_x + 1;	
			colour <= palette[dataout];
		end
		else
		begin
			colour[11:8] <= 0;
			colour[7:4]  <= 0;
			colour[3:0]  <= 0;
		end
	end
endtask
*/
localparam QTD_ELEMENTS = 3;
memorySprites #(.ELEMENTS(QTD_ELEMENTS))
memorySprites_inst
(
	.clk(clk) ,	// input  clk_sig
	.read_enable(1) ,	// input  read_enable_sig
	.address_sprite(address) ,	// input [9:0] address_sprite_sig
	.element(5) ,	// input  element_sig
	.dataout(dataout) 	// output [11:0] dataout_sig
);

always @ (posedge clk)
begin
	if(video_enable)
	begin
		if( (pixel_x >= 300 && pixel_x <= 332 ) && (pixel_y >= 300 && pixel_y <= 332 ) )
		begin
			VGA_R <= dataout[11:9];
			VGA_G <= dataout[7:5];
			VGA_B <= dataout[3:1];
			address <= address + 1;
		end
	end

end	 
endmodule
