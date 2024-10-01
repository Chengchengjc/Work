`timescale 1ns/1ns
module EX_MEM( rst, clk, RegWrite_in, MemtoReg_in, MemRead_in, MemWrite_in, zero_in,
                alu_result_in, rd2_in, rfile_wn_in,
				RegWrite_out, MemtoReg_out, MemRead_out, MemWrite_out, zero_out, 
				alu_result_out, rd2_out, rfile_wn_out ) ;

input   clk , zero_in, rst ;
input [4:0] rfile_wn_in ;
input [31:0] alu_result_in, rd2_in;
input MemtoReg_in, RegWrite_in, MemRead_in, MemWrite_in ;

output zero_out ;
output [4:0] rfile_wn_out ;
output [31:0] alu_result_out, rd2_out ;
output MemtoReg_out, RegWrite_out, MemRead_out, MemWrite_out;

reg zero_out ;
reg [4:0] rfile_wn_out ;
reg [31:0] alu_result_out, rd2_out ;
reg MemtoReg_out, RegWrite_out, MemRead_out, MemWrite_out ;

always @(posedge clk) begin
    if ( rst ) begin
	    zero_out <= 1'd0 ;
		rfile_wn_out <= 5'd0 ;
	    alu_result_out <= 32'd0 ;
		rd2_out <= 32'd0 ; 
		
		MemtoReg_out <= 1'd0 ;
		RegWrite_out <= 32'd0 ;
		MemRead_out <= 32'd0 ;		
		MemWrite_out <= 32'd0 ;
		// Branch_out <= 32'd0 ;
		// Jump_out <= 32'd0 ;
		// jump_addr_out <= 32'd0;
		// branch_add_out <= 32'd0;
	    end
    else begin
	    zero_out <= zero_in ;
		rfile_wn_out <= rfile_wn_in ;
	    alu_result_out <= alu_result_in ;
		rd2_out <= rd2_in ; 
		
		MemtoReg_out <= MemtoReg_in ;
		RegWrite_out <= RegWrite_in ;
		MemRead_out <= MemRead_in ;		
		MemWrite_out <= MemWrite_in ;
		// Branch_out <= Branch_in ;
		// Jump_out <= Jump_in ;
		// jump_addr_out <= jump_addr_in;
		// branch_add_out <= branch_add_in;
		end
	end
	
endmodule




