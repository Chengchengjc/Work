module stimulus;
reg clk, reset;
reg [2:0] howManyTicket;
reg [2:0] origin;
reg [2:0] destination;
reg [5:0] money;
wire [9:0] costOfTicket;
wire [9:0] moneyToPay;
wire [9:0] totalMoney;
vending_machine v1(clk, reset, howManyTicket, origin, destination, money, 
                       costOfTicket, moneyToPay, totalMoney);


initial clk = 1'b0;
always #5 clk = ~clk;

initial
begin
reset = 1'b1;
#10
reset = 1'b0;
origin = 3'b010; //2
destination = 3'b101; // 5
#10 howManyTicket = 3'b010; // 20*2=40
#10 money = 7'b0001010; // 10
#10 money = 7'b0001010; // 10

#10 reset = 1'd1;
#10 reset = 1'd0;
origin = 3'd5;
destination = 3'd1;
#10 howManyTicket = 3'd4; // 25*4 = 100
#10 money = 7'd50; 
#10 money = 7'd10;
#10 money = 7'd50; 

#10 origin = 3'b011; // 3
destination = 3'b101; // 5
#10 howManyTicket = 3'b001; // 15*1=15
#10 reset = 1'b1;

#10 reset = 1'b0;
#10 origin = 3'b011;
destination = 3'b101;
#10 reset = 1'b1;



#20 $stop;
end



endmodule