/*
	Title: MIPS Single-Cycle Control Unit
	Editor: Selene (Computer System and Architecture Lab, ICE, CYCU)
	
	Input Port
		1. opcode: ��J�����O�N���A�ڦ����͹���������T��
	Input Port
		1. RegDst: ����RFMUX
		2. ALUSrc: ����ALUMUX
		3. MemtoReg: ����WRMUX
		4. RegWrite: ����Ȧs���O�_�i�g�J
		5. MemRead:  ����O����O�_�iŪ�X
		6. MemWrite: ����O����O�_�i�g�J
		7. Branch: �PALU��X��zero�T����AND�B�ⱱ��PCMUX
		8. ALUOp: ��X��ALU Control
*/
`timescale 1ns/1ns
module control_pipeline( opcode, RegDst, ALUSrc, MemtoReg, RegWrite, 
					   MemRead, MemWrite, Branch, Jump, ALUOp, funct, IF_flush );
    input[5:0] opcode, funct ;
    output RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, Jump;
    output[1:0] ALUOp;
    reg RegDst, ALUSrc, MemtoReg, RegWrite, MemRead, MemWrite, Branch, Jump;
    reg[1:0] ALUOp;
   	output reg IF_flush;
    parameter R_FORMAT = 6'd0;     // nop �OR_FORMAT
    parameter LW = 6'd35;
    parameter SW = 6'd43;
    parameter BEQ = 6'd4;
	parameter J = 6'd2;
	parameter ADDIU = 6'd9;

	// funct
	parameter NOP = 6'd0;


    always @( opcode or funct) begin
        case ( opcode )
		  R_FORMAT : case ( funct )
			NOP:
			begin
				RegDst = 1'b0; ALUSrc = 1'b0; MemtoReg = 1'b0; RegWrite = 1'b0; MemRead = 1'b0; 
				MemWrite = 1'b0; Branch = 1'b0; Jump = 1'b0; ALUOp = 2'b00; IF_flush = 1'b0;
			end
			default:
			begin
				RegDst = 1'b1; ALUSrc = 1'b0; MemtoReg = 1'b0; RegWrite = 1'b1; MemRead = 1'b0; 
				MemWrite = 1'b0; Branch = 1'b0; Jump = 1'b0; ALUOp = 2'b10; IF_flush = 1'b0;
				//$display( "regWrite:", RegWrite );

			end
		  endcase
          LW :
          begin
				RegDst = 1'b0; ALUSrc = 1'b1; MemtoReg = 1'b1; RegWrite = 1'b1; MemRead = 1'b1; 
				MemWrite = 1'b0; Branch = 1'b0; Jump = 1'b0; ALUOp = 2'b00; IF_flush = 1'b0;
          end
          SW :
          begin
				RegDst = 1'bx; ALUSrc = 1'b1; MemtoReg = 1'bx; RegWrite = 1'b0; MemRead = 1'b0; 
				MemWrite = 1'b1; Branch = 1'b0; Jump = 1'b0; ALUOp = 2'b00; IF_flush = 1'b0;
          end
          BEQ :
          begin
				RegDst = 1'bx; ALUSrc = 1'b0; MemtoReg = 1'bx; RegWrite = 1'b0; MemRead = 1'b0; 
				MemWrite = 1'b0; Branch = 1'b1; Jump = 1'b0; ALUOp = 2'b01; IF_flush = 1'b1;
          end
		  J :
		  begin
				RegDst = 1'bx; ALUSrc = 1'b0; MemtoReg = 1'bx; RegWrite = 1'b0; MemRead = 1'b0; 
				MemWrite = 1'b0; Branch = 1'b1; Jump = 1'b1; ALUOp = 2'b01; IF_flush = 1'b1;
		  end
		  ADDIU :
		  begin
				RegDst = 1'b0; ALUSrc = 1'b1; MemtoReg = 1'b0; RegWrite = 1'b1; MemRead = 1'b0; 
				MemWrite = 1'b0; Branch = 1'b0; Jump = 1'b0; ALUOp = 2'b00; IF_flush = 1'b0;
		  end
		  
          default
          begin
				$display("control_single unimplemented opcode %d", opcode);
				RegDst=1'bx; ALUSrc=1'bx; MemtoReg=1'bx; RegWrite=1'bx; MemRead=1'bx; 
				MemWrite=1'bx; Branch=1'bx; Jump = 1'bx; ALUOp = 2'bxx; IF_flush = 1'bx;
          end

        endcase
    end
endmodule

