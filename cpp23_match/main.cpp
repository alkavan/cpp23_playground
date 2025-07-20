#include "oxide.hpp"

#include <functional>
#include <array>
#include <iostream>

struct Quit {};  // Unit variant
struct Move { int x, y; };  // Struct variant
struct Write { std::string text; };  // Tuple-like (but using struct for named field)
struct Read { std::function<void()> callback; };  // Variant holding a lambda or function

using Message = oxide::Enum<Quit, Move, Write, Read>;

// Non-template factory functions
constexpr Message quit() { return Message{Quit{}}; }
constexpr Message move_to(const int x, const int y) { return Message{Move{x, y}}; }
Message write(std::string text) { return Message{Write{std::move(text)}}; }
Message read(std::function<void()> callback) { return Message{Read{std::move(callback)}}; }

// Usage example
int main() {

    namespace ox = oxide;
    using ox::Result;

    // std::array<Message, 4> msgs = {
    //     Quit{},
    //     Move{1, 2},
    //     Write{"hello"},
    //     Read{[](){std::cout << "Reading...\n";}}
    // };

    std::array msgs = {
        quit(),           // No template parameter needed
        move_to(1, 2),
        write("hello"),
        read([](){std::cout << "Reading...\n";})
    };

    for (auto& msg : msgs) {
        // Existing match syntax
        msg >> ox::match {
            [](const Quit&) { std::cout << "Quit\n"; },
            [](const Move& m) { std::cout << "Move: (" << m.x << ", " << m.y << ")\n"; },
            [](const Write& w) { std::cout << "Write: " << w.text << "\n"; },
            [](const Read& r) { r.callback(); }
        };

        // Macro-based match with fixed syntax (use parentheses for the cases)
        do_match (
            match_case(const Quit&) { std::cout << "Quit (macro)\n"; },
            match_case(const Move& m) { std::cout << "Move (macro): (" << m.x << ", " << m.y << ")\n"; },
            match_case(const Write& w) { std::cout << "Write (macro): " << w.text << "\n"; },
            match_case(const Read& r) { r.callback(); },
            // Optional: Catch-all for unhandled types (runtime fallback; defeats compile-time exhaustiveness)
            match_case(auto&&) { std::cout << "Unhandled variant\n"; }
        ) match_value(msg);
    }

    // Rust-like example with std::expected (built-in monadic ops: and_then, transform, etc.)
    auto divide = [](const int a, const int b) -> ox::Result<int> {
        if (b == 0) return std::unexpected("Division by zero");
        return a / b;
    };

    Result<int> ok_res = divide(84, 2);
    Result<int> err_res = divide(84, 0);

    // Built-in methods: has_value() like is_ok, value() like unwrap (throws if error), error(), value_or(default)
    if (ok_res.has_value()) {
        std::cout << "Ok: " << ok_res.value() << "\n";
    } else {
        std::cout << "Err: " << ok_res.error() << "\n";
    }

    // Monadic chaining (Rust-like map/and_then) - capture divide by reference
    const auto chained = ok_res.and_then([&divide](const int val) -> Result<int> {
        return divide(val, 3);  // Now divide is accessible via capture
    }).transform([](const int val) { return val * 2; });  // Maps success value

    if (chained.has_value()) {
        std::cout << "Chained Ok: " << chained.value() << "\n";
    }

    // For err_res, similar handling or or_else for recovery
    const auto recovered = err_res.or_else([](const std::string&) -> Result<int> {
        return 0;  // Recover from error
    });
    std::cout << "Recovered: " << recovered.value_or(-1) << "\n";

    return 0;
}