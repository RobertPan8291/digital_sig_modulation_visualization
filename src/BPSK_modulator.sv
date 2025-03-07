module BPSK_modulator(
	input [11:0] signal, 
	input lfsr,
	input clk,
	output logic [11:0] modulated_signal
	);
	
	always_ff @(posedge clk) begin 
		case(lfsr) 
			1'b0: modulated_signal = signal; 
			1'b1: modulated_signal = (signal ^ 12'b111111111111) + 1;
		endcase
	end
	
endmodule 
