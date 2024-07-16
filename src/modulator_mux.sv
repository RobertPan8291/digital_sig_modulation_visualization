module modulator_mux(
	input [11:0] sig1,
	input [11:0] sig2,
	input [11:0] sig3, 
	input [11:0] sig4,
	input [11:0] sig5,
	input [2:0] sel,
	output logic [11:0] selected_sig
	);
	
	always_comb begin 
		case(sel) 
			3'b000: selected_sig = sig1;
			3'b001: selected_sig = sig2;
			3'b010: selected_sig = sig3; 
			3'b011: selected_sig = sig4;
			3'b100: selected_sig = sig5;
			default: selected_sig = sig1;
		endcase
	end 

endmodule
