`timescale 1ns/1ns
module MEM_WB( rst, clk, RegWrite_in, MemtoReg_in, alu_result_in, rfile_wn_in, rd_in,
				RegWrite_out, MemtoReg_out, alu_result_out, rfile_wn_out, rd_out ) ;

input   clk, rst ;
input [4:0] rfile_wn_in ;
input [31:0] alu_result_in, rd_in ;
input MemtoReg_in, RegWrite_in ;

output [4:0] rfile_wn_out ;
output [31:0] alu_result_out, rd_out ;
output MemtoReg_out, RegWrite_out ;

reg [4:0] rfile_wn_out ;
reg [31:0] alu_result_out, rd_out ;
reg MemtoReg_out, RegWrite_out ;

always @(posedge clk) begin
    if ( rst ) begin

		rfile_wn_out <= 5'd0 ;
	    alu_result_out <= 32'd0 ;
		rd_out <= 32'd0 ; 
		
		// W
		RegWrite_out <= 1'd0 ;
		MemtoReg_out <= 1'd0 ;
	    end
	else begin

		rfile_wn_out <= rfile_wn_in ;
	    alu_result_out <= alu_result_in ;
		rd_out <= rd_in ; 
		
		// W
		RegWrite_out <= RegWrite_in ;
		MemtoReg_out <= MemtoReg_in ;
		end
	end
	
endmodule




