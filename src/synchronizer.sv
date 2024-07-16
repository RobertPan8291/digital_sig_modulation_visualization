module dflipflop(
	input clk,
	input in,
	output logic out
	);
	
	always_ff @(posedge clk) begin
			out <= in;
	end
	
endmodule 

module synchronizer(
	input async_sig,
	input clk_A,
	input clk_B,
	output sync_sig
	);
	
	logic q1;
	logic q2; 
	
	dflipflop dflipflop1(
		.clk(clk_A), 
		.in(async_sig),
		.out(q1)
	);
	
	dflipflop dflipflop2(
		.clk(clk_B),
		.in(q1),
		.out(q2)
	);
	
	dflipflop dflipflop3(	
		.clk(clk_B),
		.in(q2),
		.out(sync_sig)
	);

endmodule 