module synchronizer_tb(); 
	reg async_sig; 
	reg clk_A;
	reg clk_B; 
	wire sync_sig; 
	
	synchronizer DUT(
		.async_sig(async_sig),
		.clk_A(clk_A),
		.clk_B(clk_B),
		.sync_sig(sync_sig)
	);     
	
	always #25 clk_A = ~clk_A; 
	
	always #5 clk_B = ~clk_B; 
	
	initial begin
		clk_A = 1'b0;
		async_sig = 1'b0;
		#3;
		clk_B = 1'b0;
		#22;
		async_sig = 1'b1;
		#50;
		async_sig = 1'b0;
		#50;
		async_sig = 1'b1;
		
	end
	
endmodule
