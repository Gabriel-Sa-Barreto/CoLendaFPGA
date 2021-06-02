
module Arquitetura (
	clk_clk,
	data_a_export,
	data_b_export,
	reset_reset_n,
	wrclk_export,
	wrfull_export,
	wrreg_export);	

	input		clk_clk;
	output	[31:0]	data_a_export;
	output	[31:0]	data_b_export;
	input		reset_reset_n;
	output		wrclk_export;
	input		wrfull_export;
	output		wrreg_export;
endmodule
