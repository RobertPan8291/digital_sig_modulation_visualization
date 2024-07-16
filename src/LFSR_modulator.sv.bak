module LFSR_modulator(	
	input lfsr,
	input clk,
	output logic [11:0] modulated_signal
	);

	always_ff @(posedge clk) begin 
		case(lfsr) 
			1'b0: modulated_signal = 12'b100000000000; 
			1'b1: modulated_signal = 12'd0;
		endcase
	end
	
endmodule 