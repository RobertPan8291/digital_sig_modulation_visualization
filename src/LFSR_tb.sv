module LFSR_tb();
	reg clk;
	reg rst;
	wire [4:0] lfsr;
	
	LFSR DUT(
		.clk(clk),
		.rst(rst), 
		.lfsr(lfsr) 
	); 
	
	always #5 clk = ~clk; 
	
	initial begin
		clk = 1'b0;
		rst = 1'b0;
		#10;
		rst = 1'b1;
	end

endmodule 
	