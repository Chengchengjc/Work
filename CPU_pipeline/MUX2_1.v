`timescale 1ns/1ns
module MUX2_1(out, in0, in1, sel);
    input in0, in1 ;
    input sel ;
    output out ;
	
    assign out = sel? in1 : in0 ;
endmodule
