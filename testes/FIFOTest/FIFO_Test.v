`timescale 1ns/1ns
module FIFO_Test();

integer counter;
reg [31:0] data; //bus input for written
reg rdclk;       //reading clock
reg rdreq;       //reading signal
reg wrclk;       //writting clock
reg wrreq;       //writting signal

wire wrfull;
wire rdempty;
wire [31:0] q;
initial begin
	data  = 32'd0;
	rdreq = 1'b1;
	wrreq = 1'b0;
	// Test written
	for(counter = 0; counter < 30; counter = counter + 1) begin
		@(posedge wrclk);
		@(negedge wrclk);
		if(wrfull) begin
			wrreq = 1'b0;
		end
		else begin
			wrreq = 1'b1;
		end
		data  = counter;
	end
	// Test Reading
	//for(counter = 0; counter < 20; counter = counter + 1) begin
	//	@(negedge rdclk);
	//	if(rdempty) begin
	//		rdreq = 1'b0;
	//	end
	//	else begin
	//		rdreq = 1'b1;
	//	end
	//end
	$stop;
end

FIFO FIFO_inst(
	.data(data),
	.rdclk(rdclk),
	.rdreq(rdreq),
	.wrclk(wrclk),
	.wrreq(wrreq),
	.q(q),
	.rdempty(rdempty),
	.wrfull(wrfull)
);

always begin //written frequency (100 MHz)
	rdclk = 1'b0;
	#5;
	rdclk = 1'b1;
	#5;
end

always begin //written frequency (50 MHz)
	wrclk = 1'b0;
	#10;
	wrclk = 1'b1;
	#10;
end
endmodule