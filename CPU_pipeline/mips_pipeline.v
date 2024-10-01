//	Title: MIPS Single-Cycle Processor
//	Editor: Selene (Computer System and Architecture Lab, ICE, CYCU)
`timescale 1ns/1ns
module mips_pipeline( clk, rst );
	input clk, rst;
	
	// instruction bus
	wire[31:0] instr;
	
	// break out important fields from instruction
	wire [5:0] opcode, funct;
    wire [4:0] rs_in, rt_in, rd_in, shamt_in;
    wire [15:0] immed_in;
    wire [31:0] extend_immed, b_offset, instrr;
	wire [25:0] jumpoffset_in;
	
	// IF_ID
	// wire [4:0] rs_if_in, rt_if_in, rd_if_in ;
	wire [31:0] pc_incr_out ;
    wire [25:0] jumpoffset_out;
	wire [5:0] opcode_out, funct_out;
    wire [4:0] rs_if_out, rt_if_out, rd_if_out, shamt_out;
    wire [15:0] immed_out;
	
	// ID_EX
	wire [1:0] ALUOp_out;
	wire [5:0] funct_id_out ;
	wire [4:0] rt_id_out, rd_id_out, shamt_id_out ;
	wire [31:0] extend_immed_id_out ;
	wire [31:0] rfile_rd1_id_out, rfile_rd2_id_out ;
	wire RegDst_id_out, ALUSrc_id_out, MemtoReg_id_out, RegWrite_id_out, 
	     MemRead_id_out, MemWrite_id_out ;
	
	// EX_MEM
	wire zero_ex_out ;
	wire [4:0] rfile_ex_wn ;
	wire [31:0] alu_ex_out, rfile_rd2_ex_out;
	wire MemtoReg_ex_out, RegWrite_ex_out, MemRead_ex_out, 
	     MemWrite_ex_out ;
	
	// MEM_WB
	wire [4:0] rfile_mem_wn ;
	wire [31:0] alu_mem_out, dmem_rdata_mem ;
	wire MemtoReg_mem_out, RegWrite_mem_out ;	
	
    // wire [31:0] extend_immed, b_offset;
	
	// datapath signals
    wire [4:0] rfile_wn;
    wire [31:0] rfile_rd1, rfile_rd2, rfile_wd, alu_b, alu_out, b_tgt, pc_next,
                pc, pc_incr, dmem_rdata, jump_addr, branch_addr;

	// control signals
    wire RegWrite, Branch, PCSrc, RegDst, MemtoReg, MemRead, MemWrite, ALUSrc, Zero, Jump, IF_flush;
    wire [1:0] ALUOp;
    // wire [2:0] Operation;
	
    assign opcode = instrr[31:26];
    assign rs_in = instrr[25:21];
    assign rt_in = instrr[20:16];
    assign rd_in = instrr[15:11];
    assign shamt_in = instrr[10:6];
    assign funct = instrr[5:0];
    assign immed_in = instrr[15:0];
    assign jumpoffset_in = instrr[25:0];
	
	// branch offset shifter
    assign b_offset = extend_immed << 2;
	
	// jump offset shifter & concatenation
	assign jump_addr = { pc_incr[31:28], jumpoffset_out << 2 };
	
	assign nopEn = ( IF_flush )? 1'b1 : 1'b0;
	assign instrr = ( nopEn )? 32'b0 : instr;

	// module instantiations
	
	reg32 PC( .clk(clk), .rst(rst), .en_reg(1'b1), .d_in(pc_next), .d_out(pc) );
	// sign-extender
	sign_extend SignExt( .immed_in(immed_out), .ext_immed_out(extend_immed) );
	
	add32 PCADD( .a(pc), .b(32'd4), .result(pc_incr) );

    add32 BRADD( .a(pc_incr_out), .b(b_offset), .result(b_tgt) );

    TotalALU ALU( .ALUOp(ALUOp_out), .funct(funct_id_out), .dataA(rfile_rd1_id_out), 
	              .dataB(alu_b), .shamt(shamt_id_out), .Output(alu_out), .zero(Zero),
                  .reset(rst), .clk(clk) );

    and BR_AND(PCSrc, Branch, zero_ex_out);

    mux2 #(5) RFMUX( .sel(RegDst_id_out), .a(rt_id_out), .b(rd_id_out), .y(rfile_wn) ); // #(5)可參數化宣告

    mux2 #(32) PCMUX( .sel(PCSrc), .a(pc_incr), .b(b_tgt), .y(branch_addr) );
	
	mux2 #(32) JMUX( .sel(Jump), .a(branch_addr), .b(jump_addr), .y(pc_next) );
	
    mux2 #(32) ALUMUX( .sel(ALUSrc_id_out), .a(rfile_rd2_id_out), .b(extend_immed_id_out), .y(alu_b) );

    mux2 #(32) WRMUX( .sel(MemtoReg_mem_out), .a(alu_mem_out), .b(dmem_rdata_mem), .y(rfile_wd) );

    control_pipeline CTL(.opcode(opcode_out), .RegDst(RegDst), .ALUSrc(ALUSrc), .MemtoReg(MemtoReg), 
                       .RegWrite(RegWrite), .MemRead(MemRead), .MemWrite(MemWrite), .Branch(Branch), 
                       .Jump(Jump), .ALUOp(ALUOp), .funct(funct_out) , .IF_flush(IF_flush) );

	reg_file RegFile( .clk(clk), .RegWrite(RegWrite_mem_out), .RN1(rs_if_out), .RN2(rt_if_out), .WN(rfile_mem_wn), 
					  .WD(rfile_wd), .RD1(rfile_rd1), .RD2(rfile_rd2) );

	memory InstrMem( .clk(clk), .MemRead(1'b1), .MemWrite(1'b0), .wd(32'd0), .addr(pc), .rd(instr) );
   
	memory DatMem( .clk(clk), .MemRead(MemRead_ex_out), .MemWrite(MemWrite_ex_out), .wd(rfile_rd2_ex_out), 
                   .addr(alu_ex_out), .rd(dmem_rdata) );       	
				   
	IF_ID if_id( .rst(rst), .clk(clk), .opcode_in(opcode), .pc_incr_in(pc_incr), .rs_in(rs_in), .rt_in(rt_in), 
	       .rd_in(rd_in), 
	       .shamt_in(shamt_in), .funct_in(funct), .immed_in(immed_in), .jumpoffset_in(jumpoffset_in), 
		   .pc_incr_out(pc_incr_out), .rs_out(rs_if_out), .rt_out(rt_if_out), .rd_out(rd_if_out), .opcode_out(opcode_out), 
		   .shamt_out(shamt_out), .funct_out(funct_out), .immed_out(immed_out), .jumpoffset_out(jumpoffset_out), .IF_flush(IF_flush));
		   
	ID_EX id_ex( .rst(rst), .clk(clk), .RegDst_in(RegDst), .ALUSrc_in(ALUSrc), .MemtoReg_in(MemtoReg), 
	       .RegWrite_in(RegWrite), .MemRead_in(MemRead), .MemWrite_in(MemWrite), .shamt_in(shamt_out),
		   .rfile_rd1(rfile_rd1), .rfile_rd2(rfile_rd2), .funct_in(funct_out), .shamt_id_out(shamt_id_out),
		   .rfile_rd1_id_out(rfile_rd1_id_out), .rfile_rd2_id_out(rfile_rd2_id_out), .rt_in(rt_if_out), .rd_in(rd_if_out), 
		   .RegDst_out(RegDst_id_out), .ALUSrc_out(ALUSrc_id_out), .MemtoReg_out(MemtoReg_id_out), 
		   .RegWrite_out(RegWrite_id_out), .MemRead_out(MemRead_id_out), .funct_id_out(funct_id_out),
		   .MemWrite_out(MemWrite_id_out), 
		   .extend_immed_id_in(extend_immed), .extend_immed_id_out(extend_immed_id_out), .rt_out(rt_id_out), .rd_out(rd_id_out), .ALUOp_in(ALUOp), .ALUOp_out(ALUOp_out) );	   
		   
	EX_MEM ex_mem( .rst(rst), .clk(clk), .RegWrite_in(RegWrite_id_out), .MemtoReg_in(MemtoReg_id_out), .MemRead_in(MemRead_id_out), 
	        .MemWrite_in(MemWrite_id_out), .zero_in(Zero),
            .alu_result_in(alu_out), .rd2_in(rfile_rd2_id_out), .rfile_wn_in(rfile_wn), 
			.RegWrite_out(RegWrite_ex_out), 
			.MemtoReg_out(MemtoReg_ex_out), .MemRead_out(MemRead_ex_out), 
			.MemWrite_out(MemWrite_ex_out), .zero_out(zero_ex_out), 
			.alu_result_out(alu_ex_out), .rd2_out(rfile_rd2_ex_out), .rfile_wn_out(rfile_ex_wn)
			) ;	   

    MEM_WB mem_wb( .rst(rst), .clk(clk), .RegWrite_in(RegWrite_ex_out), .MemtoReg_in(MemtoReg_ex_out), 
	        .alu_result_in(alu_ex_out), .rfile_wn_in(rfile_ex_wn), .rd_in(dmem_rdata), 
			.RegWrite_out(RegWrite_mem_out), .MemtoReg_out(MemtoReg_mem_out), 
			.alu_result_out(alu_mem_out), .rfile_wn_out(rfile_mem_wn), .rd_out(dmem_rdata_mem) ) ;				   
				   
				   
endmodule
