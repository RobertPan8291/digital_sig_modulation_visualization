module QPSK_modulator(
	input [11:0] sin_sig,
	input [11:0] cos_sig,
	input [1:0] lfsr, 
	input clk, 
	output logic [11:0] modulated_signal
	);
	
	always_ff @(posedge clk) begin 
		case(lfsr) 
			2'b00: modulated_signal = (cos_sig ^ 12'b111111111111) + 1;  
			2'b01: modulated_signal = (sin_sig ^ 12'b111111111111) + 1;
			2'b10: modulated_signal = cos_sig;
			2'b11: modulated_signal = sin_sig;
		endcase
	end
	
endmodule 
