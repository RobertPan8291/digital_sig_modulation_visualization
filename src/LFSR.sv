module d_ff(
	input clk,
	input in,
	input rst,
	output logic out
	);
	
	
	always_ff @(posedge clk or negedge rst) begin
		if(~rst)
			out <= 1'b0;
		else
			out <= in;
	end
endmodule 

module d_ff2(
	input clk,
	input in,
	input rst,
	output logic out
	);
	
	always_ff @(posedge clk or negedge rst) begin
		if(~rst)
			out <= 1'b1;
		else
			out <= in;
	end
endmodule 

module LFSR(
	input clk,
	input rst,
	output logic [4:0] lfsr
	);
	
	
	logic lfsr_3_out = 1'b0;
	logic lfsr_2_out = 1'b0;
	logic lfsr_1_out = 1'b0;
	logic lfsr_0_out = 1'b1;
	logic lfsr_4_out = 1'b0;
	logic feedback;
	
	assign feedback = lfsr_0_out ^ lfsr_2_out;
	
	assign lfsr = {lfsr_4_out, lfsr_3_out, lfsr_2_out, lfsr_1_out, lfsr_0_out};
	
	
	d_ff lfsr3(
		.clk(clk),
		.in(lfsr_4_out),
		.rst(rst),
		.out(lfsr_3_out)
	);
	
	
	d_ff lfsr2(
		.clk(clk),
		.in(lfsr_3_out),
		.rst(rst),
		.out(lfsr_2_out)
	);
	
	d_ff lfsr1(
		.clk(clk),
		.in(lfsr_2_out),
		.rst(rst),
		.out(lfsr_1_out)
	);
	
	d_ff2 lfsr0(
		.clk(clk),
		.in(lfsr_1_out),
		.rst(rst),
		.out(lfsr_0_out)
	); 
	
	d_ff lfsr4(
		.clk(clk),
		.in(feedback),
		.rst(rst),
		.out(lfsr_4_out)
	); 
	
endmodule

	