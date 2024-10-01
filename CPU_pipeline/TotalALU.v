`timescale 1ns/1ns
module TotalALU( ALUOp, dataA, dataB, shamt, funct, Output, reset, zero, clk );
input reset, clk ;
wire mulreset;
input [1:0] ALUOp;
input [4:0] shamt;
input [31:0] dataA ;
input [31:0] dataB ;
input [5:0] funct ;
output [31:0] Output ;
output	zero ;


//   Signal ( 6-bits)?
//   AND  : 36
//   OR   : 37
//   ADD  : 32
//   SUB  : 34
//   SRL  : 2
//   SLT  : 42
//   MULTU : 25
wire [2:0] ALUOperation;
wire [31:0] temp ;
wire [31:0] new_shamt = {27'b0, shamt} ;

parameter AND = 6'b100100;
parameter OR  = 6'b100101;
parameter ADD = 6'b100000;
parameter SUB = 6'b100010;
parameter SLT = 6'b101010;
parameter SRL = 6'b000010;
parameter MULTU= 6'b011001;
parameter MFHI= 6'b010000;
parameter MFLO= 6'b010010;
/*
定義各種訊號
*/
//============================
wire [5:0]  SignaltoALU ;
wire [5:0]  SignaltoSHT ;
wire [5:0]  SignaltoMUL ;
wire [5:0]  SignaltoMUX ;
wire [31:0] ALUOut, HiOut, LoOut, ShifterOut ;
wire [31:0] dataOut ;
wire [63:0] MulAns ;
/*
定義各種接線
*/
//============================

ALUControl ALUControl( .ALUOp(ALUOp), .funct(funct), .SignaltoALU(SignaltoALU), 
                       .SignaltoSHT(SignaltoSHT), .SignaltoMUL(SignaltoMUL), 
					   .SignaltoMUX(SignaltoMUX), .mulreset(mulreset),
					   .ALUOperation(ALUOperation), .clk(clk) );
alu ALU( .ALUOperation(ALUOperation), .a(dataA), .b(dataB), .result(ALUOut), .zero(zero));
Multiplier Multiplier( .dataA(dataA), .dataB(dataB), .Signal(SignaltoMUL), .dataOut(MulAns), .reset(reset), .mulreset(mulreset), .clk(clk) );
Shifter Shifter( .dataA(dataA), .dataB(new_shamt), .Signal(SignaltoSHT), .dataOut(ShifterOut) );
HiLo HiLo( .MulAns(MulAns), .HiOut(HiOut), .LoOut(LoOut), .reset(reset), .clk(clk) );
MUX MUX( .ALUOut(ALUOut), .HiOut(HiOut), .LoOut(LoOut), .Shifter(ShifterOut), .Signal(SignaltoMUX), .dataOut(dataOut) );
/*
建立各種module
*/
assign Output = dataOut ;


endmodule