/*
	Title:	ALU
	Author: Garfield (Computer System and Architecture Lab, ICE, CYCU)
	Input Port
		1. ctl: 由alu_ctl解碼完的控制訊號
		2. a:   inputA,第一筆要處理的資料
		3. b:   inputB,第二筆要處理的資料
	Output Port
		1. result: 最後處理完的結果
		2. zero:   branch指令所需要之輸出
*/

`timescale 1ns/1ns
module alu( ALUOperation, a, b, result, zero );
  input [2:0] ALUOperation;
  input [31:0] a, b;
  output [31:0] result;
  output zero;
  wire [31:0] c, temp;
  wire Binvt, Cout;
  


  wire [31:0] result;
  wire zero;
  parameter AND = 3'b000;
  parameter OR  = 3'b001;
  parameter ADD = 3'b010;
  parameter SUB = 3'b110;
  parameter SLT = 3'b111;
  
  assign Binvt = ( ALUOperation == SUB || ALUOperation == SLT )? 1 : 0;
  assign c[0] = ( ALUOperation == SUB || ALUOperation == SLT )? 1 : 0;
  
  ALUBitSlice alu_0( .a(a[0]), .b(b[0]), .cin(c[0]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[1]), .out(temp[0]) );
  ALUBitSlice alu_1( .a(a[1]), .b(b[1]), .cin(c[1]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[2]), .out(temp[1]) );
  ALUBitSlice alu_2( .a(a[2]), .b(b[2]), .cin(c[2]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[3]), .out(temp[2]) );
  ALUBitSlice alu_3( .a(a[3]), .b(b[3]), .cin(c[3]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[4]), .out(temp[3]) );
  ALUBitSlice alu_4( .a(a[4]), .b(b[4]), .cin(c[4]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[5]), .out(temp[4]) );
  ALUBitSlice alu_5( .a(a[5]), .b(b[5]), .cin(c[5]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[6]), .out(temp[5]) );
  ALUBitSlice alu_6( .a(a[6]), .b(b[6]), .cin(c[6]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[7]), .out(temp[6]) );
  ALUBitSlice alu_7( .a(a[7]), .b(b[7]), .cin(c[7]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[8]), .out(temp[7]) );
  ALUBitSlice alu_8( .a(a[8]), .b(b[8]), .cin(c[8]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[9]), .out(temp[8]) );
  ALUBitSlice alu_9( .a(a[9]), .b(b[9]), .cin(c[9]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[10]), .out(temp[9]) );
  ALUBitSlice alu_10( .a(a[10]), .b(b[10]), .cin(c[10]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[11]), .out(temp[10]) );
  ALUBitSlice alu_11( .a(a[11]), .b(b[11]), .cin(c[11]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[12]), .out(temp[11]) );
  ALUBitSlice alu_12( .a(a[12]), .b(b[12]), .cin(c[12]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[13]), .out(temp[12]) );
  ALUBitSlice alu_13( .a(a[13]), .b(b[13]), .cin(c[13]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[14]), .out(temp[13]) );
  ALUBitSlice alu_14( .a(a[14]), .b(b[14]), .cin(c[14]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[15]), .out(temp[14]) );
  ALUBitSlice alu_15( .a(a[15]), .b(b[15]), .cin(c[15]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[16]), .out(temp[15]) );
  ALUBitSlice alu_16( .a(a[16]), .b(b[16]), .cin(c[16]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[17]), .out(temp[16]) );
  ALUBitSlice alu_17( .a(a[17]), .b(b[17]), .cin(c[17]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[18]), .out(temp[17]) );
  ALUBitSlice alu_18( .a(a[18]), .b(b[18]), .cin(c[18]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[19]), .out(temp[18]) );
  ALUBitSlice alu_19( .a(a[19]), .b(b[19]), .cin(c[19]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[20]), .out(temp[19]) );
  ALUBitSlice alu_20( .a(a[20]), .b(b[20]), .cin(c[20]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[21]), .out(temp[20]) );
  ALUBitSlice alu_21( .a(a[21]), .b(b[21]), .cin(c[21]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[22]), .out(temp[21]) );
  ALUBitSlice alu_22( .a(a[22]), .b(b[22]), .cin(c[22]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[23]), .out(temp[22]) );
  ALUBitSlice alu_23( .a(a[23]), .b(b[23]), .cin(c[23]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[24]), .out(temp[23]) );
  ALUBitSlice alu_24( .a(a[24]), .b(b[24]), .cin(c[24]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[25]), .out(temp[24]) );
  ALUBitSlice alu_25( .a(a[25]), .b(b[25]), .cin(c[25]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[26]), .out(temp[25]) );
  ALUBitSlice alu_26( .a(a[26]), .b(b[26]), .cin(c[26]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[27]), .out(temp[26]) );
  ALUBitSlice alu_27( .a(a[27]), .b(b[27]), .cin(c[27]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[28]), .out(temp[27]) );
  ALUBitSlice alu_28( .a(a[28]), .b(b[28]), .cin(c[28]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[29]), .out(temp[28]) );
  ALUBitSlice alu_29( .a(a[29]), .b(b[29]), .cin(c[29]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[30]), .out(temp[29]) );
  ALUBitSlice alu_30( .a(a[30]), .b(b[30]), .cin(c[30]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(c[31]), .out(temp[30]) );
  ALUBitSlice alu_31( .a(a[31]), .b(b[31]), .cin(c[31]), .bInvt(Binvt), .ALUOperation(ALUOperation), .cout(Cout), .out(temp[31]) );

  assign zero = (temp == 32'd0)? 1:0;
  assign result = ( ALUOperation == SLT )? {31'b0, temp[31]} : temp;

endmodule

