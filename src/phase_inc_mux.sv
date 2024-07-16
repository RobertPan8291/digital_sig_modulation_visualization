module phase_inc_mux(
	input [31:0] dds_increment,
	input [2:0] sel,
	output logic [31:0] selected_phase_inc
	); 
	
	
	always_comb begin
		if(sel == 3'b001) 	
			selected_phase_inc = dds_increment; //Either 1Hz or 5Hz depending on signal sent by NIOS2
		else 
			selected_phase_inc = 32'd258;  //Generates 3Hz signal
	end
	
endmodule 

	