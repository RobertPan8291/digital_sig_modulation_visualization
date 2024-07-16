module sig_mux(
	input [11:0] sig1,
	input [11:0] sig2,
	input [11:0] sig3, 
	input [11:0] sig4,
	input [1:0] sel,
	output logic [11:0] selected_sig
	);
	
	always_comb begin 
		case(sel) 
			2'b00: selected_sig = sig1;
			2'b01: selected_sig = sig2;
			2'b10: selected_sig = sig3; 
			2'b11: selected_sig = sig4;
		endcase
	end 

endmodule
