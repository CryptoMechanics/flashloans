#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;
CONTRACT borrower : public contract {
   public:
      using contract::contract;

      ACTION execute(asset amount);

      [[eosio::on_notify("eosio.token::transfer")]]
	    void on_loan( const name&    from,
                    const name&    to,
                    const asset&   quantity,
                    const std::string&  memo );

      using execute_action = action_wrapper<"execute"_n, &borrower::execute>;
};