module vending_machine(clk, reset, howManyTicket, origin, destination, money, 
                       costOfTicket, moneyToPay, totalMoney);

input clk, reset;
input [2:0] howManyTicket, origin, destination;
input [5:0] money;
output reg [9:0] costOfTicket; 
output reg [9:0] moneyToPay;
output reg [9:0] totalMoney;

reg two_again = 1'b0;
reg [1:0] state;
reg [1:0] next_state;


parameter s0 = 3'd0; 
parameter s1 = 3'd1; 
parameter s2 = 3'd2; 
parameter s3 = 3'd3; 


always @(posedge clk)
begin
    if(reset) begin
        state <= s3;
    end
    else
    begin
      if(state == next_state) two_again = ~two_again;
      state <= next_state; 
    end
end

always @(state or two_again) begin
case(state)
s0:
	begin
    totalMoney = 10'd0;
    moneyToPay = 10'd0;
    costOfTicket = 10'd0;
    $display("origin = S%d", origin);
    $display("destination = S%d", destination);
    if( destination < origin ) costOfTicket <= (origin-destination+3'd1) * 5'd5;
    else costOfTicket <= (destination-origin+3'd1) * 5'd5;

	end
s1: 
	begin
	$display("howManyTicket = %d", howManyTicket);
	moneyToPay <= howManyTicket*costOfTicket;
	end
s2: 
	begin
    totalMoney = totalMoney + money;
    if( moneyToPay > totalMoney ) $display("totalMoney = %d, notEnoughMoney = %d", totalMoney, moneyToPay-totalMoney);
    else $display("totalMoney = %d, notEnoughMoney = 0", totalMoney);
	end
s3: 
	if ( !reset ) 
	begin
		if( totalMoney > moneyToPay ) $display("change = %d", totalMoney-moneyToPay);
		$display("done ! howManyTicket = %d", howManyTicket);    
	end
	else $display("cancel ! change = %d", totalMoney);

endcase
end 

// state_machine
always @(state or two_again) begin
case(state) 
	s0: next_state = s1;
	s1: next_state = s2;
	s2: 
	begin
		if( totalMoney < moneyToPay ) next_state = s2;
		else next_state = s3;
	end
	s3: next_state = s0;
endcase
end

endmodule
