`timescale 1ns/1ns
module ID_EX( rst, clk, RegDst_in, ALUSrc_in, MemtoReg_in, RegWrite_in, MemRead_in, MemWrite_in, 
			  rt_in, rd_in, rfile_rd1, rfile_rd2,
			  funct_in, shamt_in,
			  RegDst_out, ALUSrc_out, MemtoReg_out, RegWrite_out, rfile_rd1_id_out, 
			  rfile_rd2_id_out, extend_immed_id_in, extend_immed_id_out, 
			  funct_id_out, shamt_id_out,
			  MemRead_out, MemWrite_out, rt_out, rd_out, ALUOp_in, ALUOp_out );
input   clk, rst ;
input [1:0] ALUOp_in;
input [5:0] funct_in ;
input [4:0] rt_in, rd_in, shamt_in ;
input [31:0] extend_immed_id_in ;
input [31:0] rfile_rd1, rfile_rd2;
input RegDst_in, ALUSrc_in, MemtoReg_in, RegWrite_in, MemRead_in, MemWrite_in ;

output [1:0] ALUOp_out;
output [5:0] funct_id_out ;
output [4:0] rt_out, rd_out, shamt_id_out ;
output [31:0] extend_immed_id_out ;
output [31:0] rfile_rd1_id_out, rfile_rd2_id_out ;
output RegDst_out, ALUSrc_out, MemtoReg_out, RegWrite_out, MemRead_out, MemWrite_out ;

reg [1:0] ALUOp_out;
reg [5:0] funct_id_out ;
reg [4:0] rt_out, rd_out, shamt_id_out ;
reg [31:0] extend_immed_id_out ;
reg [31:0] rfile_rd1_id_out, rfile_rd2_id_out;
reg RegDst_out, ALUSrc_out, MemtoReg_out, RegWrite_out, MemRead_out, MemWrite_out, Branch_out;

always @(posedge clk) 
	begin
		if ( rst ) begin
			rd_out <= 5'd0 ;
			rt_out <= 5'd0 ;
		
			// pc add
			// pc_incr_id_out <= 32'd0 ;
		
			// reg file
			rfile_rd1_id_out <= 32'd0 ;
			rfile_rd2_id_out <= 32'd0 ;
		
			// E
			RegDst_out <= 1'b0 ;
			ALUSrc_out <= 1'b0;
		
			// W
			MemtoReg_out <= 1'b0 ;
			RegWrite_out <= 1'b0 ;
		
			// M
			MemRead_out <= 1'b0 ;		
			MemWrite_out <= 1'b0 ;
			// Branch_out <= 1'b0 ;
		
			funct_id_out <= 6'd0 ;
			shamt_id_out <= 5'd0 ;
			ALUOp_out <= 2'd0;
		
			extend_immed_id_out <= 32'd0 ;
		end
		else begin	
			rd_out <= rd_in ;
			rt_out <= rt_in ;
		
			// pc add
			// pc_incr_id_out <= pc_incr_in ;
		
			// reg file
			rfile_rd1_id_out <= rfile_rd1 ;
			rfile_rd2_id_out <= rfile_rd2 ;
		
			// E
			RegDst_out <= RegDst_in ;
			ALUSrc_out <= ALUSrc_in;
		
			// W
			MemtoReg_out <= MemtoReg_in ;
			RegWrite_out <= RegWrite_in ;
		
			// M
			MemRead_out <= MemRead_in ;		
			MemWrite_out <= MemWrite_in ;
			// Branch_out <= Branch_in ;
		
			funct_id_out <= funct_in ;
			shamt_id_out <= shamt_in ;
			ALUOp_out <= ALUOp_in;
		
			extend_immed_id_out <= extend_immed_id_in ;
		end
	end
	
endmodule




