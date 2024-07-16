module ASK_modulator(
	input [11:0] signal,
	input lfsr, 
	input clk, 
	output logic [11:0] modulated_signal
	);
	
	always_ff @(posedge clk) begin 
		case(lfsr) 
			1'b0: modulated_signal <= 12'd0; 
			1'b1: modulated_signal <= signal;
		endcase
	end
	
endmodule 

	