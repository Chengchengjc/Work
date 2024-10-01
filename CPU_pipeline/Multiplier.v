`timescale 1ns/1ns
module Multiplier( clk, dataA, dataB, Signal, dataOut, reset, mulreset );
input clk, mulreset;
input reset ;
input [31:0] dataA ;
input [31:0] dataB ;
input [5:0] Signal ;
reg [63:0] temp;
output [63:0] dataOut ;

//   Signal ( 6-bits)?
//   Multiplier  : 25

reg [63:0] prod;
reg [63:0] a_in_reg;
reg [31:0] b_in_reg;
parameter MULTU = 6'b011001;
parameter OUT = 6'b111111;

/*
�w�q�U�ذT��
*/
always@(posedge reset)
	begin
        prod <= 64'b0 ;
		temp <= 64'b0;
		a_in_reg <= 64'b0;
		b_in_reg <= 32'b0;
	end
/*
reset�T�� �p�G�Oreset�N���k0
*/
	
always@(posedge mulreset)
	begin
		prod <= 64'b0 ;
		a_in_reg <= {32'b0, dataA};
		b_in_reg <= dataB;					
		
	end

always@(negedge clk)
    begin
		case(Signal)
		MULTU:
		begin
		if(b_in_reg[0] == 1)
			begin
				prod <= prod + a_in_reg;
			end
		a_in_reg <= a_in_reg << 1;
		b_in_reg <= b_in_reg >> 1;
		end
		OUT:
			temp <= prod;
		endcase
	end
/*
���k�B��
OUT�������O�n��control���A���O�A�~����⵪�׿�X��HILO�Ȧs��
*/

assign dataOut = temp;

endmodule