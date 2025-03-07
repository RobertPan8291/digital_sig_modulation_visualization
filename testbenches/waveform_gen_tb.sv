module waveform_gen_tb();
	reg clk;
	reg reset;
	reg en;
	reg [31:0] phase_inc;
	wire [11:0] sin_out;
	wire [11:0] cos_out;
	wire [11:0] squ_out;
	wire [11:0] saw_out; 
	
	waveform_gen DUT(
		.clk(clk),
		.reset(reset),
		.en(en),
		.phase_inc(phase_inc),
		.sin_out(sin_out),
		.cos_out(cos_out),
		.squ_out(squ_out),
		.saw_out(saw_out)
	);
	
	always #5 clk = ~clk; 
	
	initial begin 
		clk = 1'b0;
		en = 1'b1;
		phase_inc = 32'd258;
		reset = 1'b1;
	end
	
endmodule 
	