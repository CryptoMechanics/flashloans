#include <borrower.hpp>
ACTION borrower::execute(asset amount) {

	print("borrower::execute ", amount, "\n");

  action act(
    permission_level{_self, "active"_n},
    "flashloans"_n, "borrow"_n,
    std::make_tuple(get_self(), get_self(), amount)
  );
  act.send();

}

void borrower::on_loan( const name&    from,
              const name&    to,
              const asset&   quantity,
              const std::string&  memo ){

	print("borrower::on_loan handler: ", from, " ", to, " ", quantity, "\n");

	if (from == "flashloans"_n){

	  action act1(
	    permission_level{_self, "active"_n},
	    "eosio.token"_n, "transfer"_n,
	    std::make_tuple(get_self(), "ict"_n, quantity, std::string(""))
	  );
	  act1.send();

	  action act2(
	    permission_level{_self, "active"_n},
	    "ict"_n, "withdraw"_n,
	    std::make_tuple(get_self(), quantity)
	  );
	  act2.send();

	  asset final_quantity = quantity;

	  //final_quantity = asset(quantity.amount-1, quantity.symbol);

	  action act3(
	    permission_level{_self, "active"_n},
	    "eosio.token"_n, "transfer"_n,
	    std::make_tuple(get_self(), "flashloans"_n, final_quantity, std::string(""))
	  );
	  act3.send();

	}


}