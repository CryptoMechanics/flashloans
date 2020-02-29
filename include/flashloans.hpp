#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
using namespace eosio;

CONTRACT flashloans : public contract {
   public:
      using contract::contract;

      struct [[eosio::table]] loan {
      	name borrower;
       	asset loan;
       	asset repaid;

        uint64_t primary_key()const { return borrower.value; }

      };

      ACTION complete(name borrower, asset amount);
      ACTION borrow(name borrower, name callback, asset amount);
      ACTION clear();

      [[eosio::on_notify("eosio.token::transfer")]]
	    void on_repayment( const name&    from,
                    const name&    to,
                    const asset&   quantity,
                    const std::string&  memo );


      typedef eosio::multi_index< "loans"_n, loan > loans;

      using borrow_action = action_wrapper<"borrow"_n, &flashloans::borrow>;
      using complete_action = action_wrapper<"complete"_n, &flashloans::complete>;
      using clear_action = action_wrapper<"clear"_n, &flashloans::clear>;
};



/*

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/name.hpp>

using namespace eosio;

CONTRACT fltest : public contract {
   public:
      using contract::contract;

      struct [[eosio::table]] param {
      	std::string value;
      	std::string type;
      };

      struct [[eosio::table]] cAction {
      	name contract;
      	name name;
       	std::vector<param> params;
      };

      struct [[eosio::table]] loans {
      	name borrower;
       	asset loan;
       	asset repaid;

        uint64_t primary_key()const { return borrower.value; }

      };

    [[eosio::on_notify("eosio.token::transfer")]]
         void deposit( const name&    from,
                              const name&    to,
                              const asset&   quantity,
                              const std::string&  memo );

     typedef eosio::multi_index<"loans"_n, loans> loans_table;
        
      ACTION borrow(	eosio::asset amount, 
      								eosio::name borrower,
      								std::vector<cAction> actions);

      using borrow_action = action_wrapper<"borrow"_n, &fltest::borrow>;
};

*/