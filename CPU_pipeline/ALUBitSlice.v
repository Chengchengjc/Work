`timescale 1ns/1ns
module ALUBitSlice( a, b, cin, bInvt, ALUOperation, cout, out );
	input a, b, cin, bInvt;
	input [2:0] ALUOperation;
	output cout, out;
	
	wire b_in, sum;
	
	/* ALUOperation //
	AND : 36
	OR  : 37
	ADD : 32
	SUB : 34
	SLT : 42
	*/
	parameter AND = 3'b000;
	parameter OR  = 3'b001;
	parameter ADD = 3'b010;
	parameter SUB = 3'b110;
	parameter SLT = 3'b111;
	
	assign b_in = ( bInvt )? ~b : b;
	FA u_fa(.a(a), .b(b_in), .c(cin), .cout(cout), .sum(sum) );
	
	assign out = ALUOperation[1]? sum : ( ALUOperation[0]? a|b_in : a&b_in );
	  
endmodule