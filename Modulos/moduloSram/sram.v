`default_nettype none

/*
//Descriçao: 
//           Modulo de criaçao de uma memoria Ram para armazenamento dos sprites desejados
//           ADDR_WIDTH: tamanho do endereço em bits (padrão: 8) 
//           DATA_WIDTH: tamanho dos dados em bits (padrão: 8)
//                DEPTH: número de itens na matriz de memória (padrão: 256)
//////////////////////////////////////////////////////////////////////////////////////
*/
module sram #(parameter ADDR_WIDTH=8, DATA_WIDTH=8, DEPTH=256, MEMFILE="") (
    input wire i_clk,                      //clock da FPGA (50MHz)
    input wire [ADDR_WIDTH-1:0] i_addr,    //endereço de acesso a memoria
    input wire i_write,                    //entrada para habilitaçao de escrita
    input wire [DATA_WIDTH-1:0] i_data,    //entrada de dados
    output reg [DATA_WIDTH-1:0] o_data,    //saida de dados
	 output wire dataoutReady               //informa se o dado requerido esta pronto
    );

    reg [DATA_WIDTH-1:0] memory_array [0:DEPTH-1]; 

    initial begin
        if (MEMFILE > 0)
        begin
            $display("Loading memory init file '" + MEMFILE + "' into array.");
            $readmemh(MEMFILE, memory_array);
        end
    end

    always @ (posedge i_clk)
    begin
        if(i_write) begin
            memory_array[i_addr] <= i_data;
        end
        else begin
            o_data <= memory_array[i_addr];
        end     
    end
	 
	 assign dataoutReady = (i_write == 0) ? 1'b1: 1'b0;
endmodule