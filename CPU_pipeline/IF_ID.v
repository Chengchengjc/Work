`timescale 1ns/1ns
module IF_ID( rst, clk, opcode_in, pc_incr_in, rs_in, rt_in, rd_in, shamt_in, funct_in, immed_in, jumpoffset_in, pc_incr_out, rs_out, rt_out, rd_out,
              opcode_out, shamt_out, funct_out, immed_out, jumpoffset_out , IF_flush);
input   clk, rst, IF_flush;
input  [5:0] opcode_in ;
input  [31:0]  pc_incr_in ;
input  [4:0] rs_in ;
input  [4:0] rt_in ;
input  [4:0] rd_in ;
input  [4:0] shamt_in ;
input  [5:0] funct_in ;
input  [15:0] immed_in ;
input  [25:0] jumpoffset_in ;
output  [5:0] opcode_out ;
output [31:0]  pc_incr_out ;
output [4:0] rs_out ;
output [4:0] rt_out ;
output [4:0] rd_out ;
output  [4:0] shamt_out ;
output  [5:0] funct_out ;
output  [15:0] immed_out ;
output  [25:0] jumpoffset_out ;

reg [5:0]  opcode_out ;
reg [31:0]  pc_incr_out ;
reg [4:0] rs_out ;
reg [4:0] rt_out ;
reg [4:0] rd_out ;
reg  [4:0] shamt_out ;
reg  [5:0] funct_out ;
reg  [15:0] immed_out ;
reg  [25:0] jumpoffset_out ;


always @(posedge clk) begin
	if ( rst ) begin
	    opcode_out <= 6'd0 ;
		pc_incr_out <= 32'd0;
		rs_out <= 5'd0 ;
		rt_out <= 5'd0 ;
		rd_out <= 5'd0 ;
		
		shamt_out <= 5'd0 ;
		funct_out <= 6'd0 ;
		immed_out <= 16'd0 ;
		jumpoffset_out <= 26'd0 ;
	end
	
	else if ( IF_flush ) begin
		opcode_out <= opcode_in ;
		pc_incr_out <= pc_incr_in + 32'd4;
		rs_out <= rs_in ;
		rt_out <= rt_in ;
		rd_out <= rd_in ;
		
		shamt_out <= shamt_in ;
		funct_out <= funct_in ;
		immed_out <= immed_in ;
		jumpoffset_out <= jumpoffset_in ;
	end
	
	else begin
	    opcode_out <= opcode_in ;
		pc_incr_out <= pc_incr_in;
		rs_out <= rs_in ;
		rt_out <= rt_in ;
		rd_out <= rd_in ;
		
		shamt_out <= shamt_in ;
		funct_out <= funct_in ;
		immed_out <= immed_in ;
		jumpoffset_out <= jumpoffset_in ;
		end
	end
	
endmodule




