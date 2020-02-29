#include <flashloans.hpp>

//emplace loan

static int test = 0;


ACTION flashloans::borrow(name borrower, name callback, asset amount) {

	print("flashloans::borrow ", borrower, " ", callback, " ", amount, "\n");

	print("test: ", test, "\n");

	test = 1;

	print("test: ", test, "\n");


	require_auth(borrower);

	loans ltable(get_self(), get_self().value);

	auto ptr = ltable.find(borrower.value);

   check(ptr == ltable.end(), "only one flashloan at a time"); //prevents reentry attacks

   ltable.emplace( borrower, [&]( auto& a ){
     a.borrower = borrower;
     a.loan = amount;
     a.repaid = asset(0, amount.symbol);
   });

  action act1(
    permission_level{_self, "active"_n},
    "eosio.token"_n, "transfer"_n,
    std::make_tuple(get_self(), callback, amount, std::string(""))
  );
  act1.send();

  action act2(
    permission_level{_self, "active"_n},
    _self, "complete"_n,
    std::make_tuple(borrower, amount)
  );
  act2.send();

}


void flashloans::complete(name borrower, asset amount){

	
	flashloans::loans ltable(_self, _self.value);
   
   auto ptr = ltable.find(borrower.value);

   print("ptr->repaid", ptr->repaid, "\n");
   check(ptr->repaid >= amount, "flashloan not repaid, rolling back transaction");

   ltable.erase(ptr);

   print("success\n");

   test = 3;

	print("test: ", test, "\n");


}


void flashloans::on_repayment( const name&    from,
              const name&    to,
              const asset&   quantity,
              const std::string&  memo ){

	print("flashloans::on_repayment handler: ", from, " ", to, " ", quantity, "\n");

	test = 2;

	print("test: ", test, "\n");

	flashloans::loans ltable(_self, _self.value);

	if (from!=get_self()){

		print("payment from ", from, "\n");

		auto ptr = ltable.find(from.value);

		if (ptr != ltable.end()){


	   ltable.modify(ptr, same_payer, [&]( auto& a ){
	     a.repaid += quantity;
	   });

			print("repayment successfully processed\n");

		}


	}
}

void flashloans::clear(){

  loans ltable( get_self(), get_self().value );

  while (ltable.begin() != ltable.end()) {
      auto itr = ltable.end();
      itr--;
      ltable.erase(itr);
  }

}

/*

#include <fltest.hpp>
ACTION fltest::borrow(	eosio::asset amount,  
											eosio::name borrower,  
											std::vector<cAction> actions) {

	require_auth(borrower);

	//TODO : check asset is allowed

  loans_table ltable( get_self(), get_self().value );
	auto existing = ltable.find(borrower.value );
  
  check( existing == ltable.end(), "loan already exists for user" ); //prevent reentry attacks

  ltable.emplace( borrower, [&]( auto& s ) {
     s.borrower = borrower;
     s.loan = amount;
     s.repaid    = asset(0, amount.symbol);
  });

	int actionsCount = actions.size();

	//check (actionsCount==paramsCount==typesCount, "must supply same number of actions, params and types");

	for (int i = 0; i<actionsCount;i++){

		print("action :", i, " -> ", actions[i].name, "\n");
		print("contract :", i, " -> ", actions[i].contract, "\n");
		
		int cParamsCount = actions[i].params.size();
		
		std::tuple previous = std::make_tuple();
		std::tuple next;
		

		for (int j = 0; j<cParamsCount;j++){	
			print("  (", actions[i].params[j].type, ") ");
			print(actions[i].params[j].value, "\n");

			// next;

			if (actions[i].params[j].type == "string"){
				std::tuple<std::string> ctup = std::make_tuple(actions[i].params[j].value);
				//next = std::tuple_cat(previous, ctup);
			}
			else if (actions[i].params[j].type == "asset"){
				//std::tuple<asset> ctup = std::make_tuple(eosio::asset(actions[i].params[j].int, actions[i].params[j].value));
			}
			else if (actions[i].params[j].type == "name"){
				std::tuple<name> ctup = std::make_tuple(eosio::name(actions[i].params[j].value));
				next = std::tuple_cat(previous, ctup);
			}

			//delete previous;

			previous = next;

		}
	  
	  action act(
	    permission_level{borrower, "active"_n},
	    actions[i].contract, 
	    actions[i].name,
	    //std::make_tuple()
	  );
		act.send();

		print("\n");

	}

	existing = ltable.find(borrower.value );

	check(existing->repaid >= existing->loan, "couldn't repay loan within transaction, aborting.");

	ltable.erase(existing);

}

//deposit handler for system_symbol
void fltest::deposit( const name&    from,
                      const name&    to,
                      const asset&   quantity,
                      const std::string&  memo )
{

  print("transfer handler\n");

  //TODO : check quantity symbol

  if (from != _self && to == _self){

	  loans_table ltable( get_self(), get_self().value );
		auto existing = ltable.find(from.value );
	  
	  if (existing!= ltable.end()){

	    ltable.modify( existing, same_payer, [&]( auto& s ) {
	       s.repaid += quantity;
	    });
	    
	  }

  }

}

*/