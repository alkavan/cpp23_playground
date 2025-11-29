#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <random>
#include <chrono>

struct TransactionResult {
    bool        success;      // true  → transaction applied
    double      new_balance; // balance after the operation
    std::string tx_id;       // unique transaction identifier
};

inline auto make_rng()
{
    std::array<std::mt19937_64::result_type, 4> seed{};
    std::random_device rd;
    std::ranges::generate(seed, std::ref(rd));
    std::seed_seq seq(seed.begin(), seed.end());
    return std::mt19937_64(seq);
}
thread_local std::mt19937_64 rng = make_rng();

[[nodiscard]]  // never ignore the result
auto withdraw(const double balance, const double amount, const std::string& id) -> TransactionResult
{
    // Preconditions (runtime checks + assume for opt)
    if (!(balance >= 0.0)) { throw std::invalid_argument("Balance must be non-negative"); }
    __assume(balance >= 0.0);
    if (!(amount > 0.0)) { throw std::invalid_argument("Amount must be positive"); }
    __assume(amount > 0.0);
    if (id.empty()) { throw std::invalid_argument("ID must not be empty"); }
    __assume(!id.empty());
    if (!(amount <= balance)) { throw std::invalid_argument("Insufficient balance"); }
    __assume(amount <= balance);

    const double new_bal = balance - amount;

    // "Postcondition" check (audit invariant)
    if (!(new_bal <= balance)) { throw std::logic_error("Invariant violation: new balance exceeds old"); }
    __assume(new_bal <= balance && new_bal >= 0.0);  // Post: new_balance >= 0.0

    std::uniform_int_distribution<std::uint64_t> dist;
    const std::string tx = id + "-TX" + std::to_string(dist(rng));

    const TransactionResult result{true, new_bal, tx};

    return result;
}

int main()
{
    const std::string account = "ACC-12345";

    try {
        constexpr double balance = 0.;
        auto [ok, new_balance, txid] = withdraw(balance, 250.0, account);
        std::cout << "Transaction result:\n"
                  << "  success      : " << std::boolalpha << ok << '\n'
                  << "  new balance  : $" << new_balance << '\n'
                  << "  transaction id: " << txid << "\n\n";

        if (ok) {
            std::cout << "  -> Funds withdrawn, balance updated.\n";
        } else {
            std::cout << "  -> Withdrawal failed.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Transaction failed: " << e.what() << '\n';
    }
    return 0;
}
