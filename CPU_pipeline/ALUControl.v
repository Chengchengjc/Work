`timescale 1ns/1ns
module ALUControl(clk, funct, SignaltoALU, SignaltoSHT, SignaltoMUL, SignaltoMUX, mulreset, ALUOp, ALUOperation );
input clk;
input [5:0] funct;
input [1:0] ALUOp;
output reg [2:0] ALUOperation;
output [5:0] SignaltoALU ;
output [5:0] SignaltoSHT ;
output [5:0] SignaltoMUL ;
output [5:0] SignaltoMUX ;
output  mulreset ;

reg mulreset;
reg [5:0] temp ;
reg [6:0] counter ;


parameter AND = 6'b100100;
parameter OR  = 6'b100101;
parameter ADD = 6'b100000;
parameter SUB = 6'b100010;
parameter SLT = 6'b101010;
parameter SRL = 6'b000010;
parameter MULTU = 6'b011001;
parameter MFHI = 6'b010000;
parameter MFLO = 6'b010010;

// symbolic constants for instruction function code
parameter F_add = 6'd32;
parameter F_sub = 6'd34;
parameter F_and = 6'd36;
parameter F_or  = 6'd37;
parameter F_slt = 6'd42;
	
// symbolic constants for ALU Operations
parameter ALU_add = 3'b010;
parameter ALU_sub = 3'b110;
parameter ALU_and = 3'b000;
parameter ALU_or  = 3'b001;
parameter ALU_slt = 3'b111;


always@( funct )
begin
  if ( funct == MULTU )
  begin
	mulreset = 1'b1;
    counter = 6'b0 ;
  end
/*
如果訊號改變成乘法 就把counter歸0
*/
end

always@( posedge clk )
begin
  if ( funct == MULTU )
  begin
    counter = counter + 1 ;
	
    if ( counter == 32 )
    begin
      temp = 6'b111111 ; // Open HiLo reg for Div
	  mulreset = 1'b0; // 復原為0 讓下次呼叫乘法器時mulreset trigger用
      counter = 0 ;
    end
  end
  else if ( funct != AND && funct != OR && funct != ADD && funct != SUB &&
		funct != SLT && funct != SRL && funct != MULTU && funct != MFHI &&
		funct != MFLO )
		temp = ADD;
  else temp = funct ;
/*
數32個clk然後開啟HiLo暫存器給乘法器放值進去
*/
end

always @(ALUOp or funct)
begin
    case (ALUOp) 
        2'b00 : ALUOperation = ALU_add;
        2'b01 : ALUOperation = ALU_sub;
        2'b10 : case (funct) 
                    F_add : ALUOperation = ALU_add;
                    F_sub : ALUOperation = ALU_sub;
                    F_and : ALUOperation = ALU_and;
                    F_or  : ALUOperation = ALU_or;
                    F_slt : ALUOperation = ALU_slt;
                    default ALUOperation = 3'bxxx;
                endcase
        default ALUOperation = 3'bxxx;
    endcase
end



assign SignaltoALU = temp ;
assign SignaltoSHT = temp ;
assign SignaltoMUL = temp ;
assign SignaltoMUX = temp ;


endmodule