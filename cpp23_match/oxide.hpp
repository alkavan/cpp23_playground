#pragma once

#include <variant>
#include <string>
#include <utility>
#include <expected>
#include <optional>
#include <vector>

namespace oxide {
    template<typename T>
    using Option = std::optional<T>;

    // Optional type
    template<typename T>
    constexpr Option<std::decay_t<T>> Some(T&& value) {
        return std::make_optional(std::forward<T>(value));
    }

    constexpr std::nullopt_t None = std::nullopt;

    // Vector type
    template<typename T>
    using Vec = std::vector<T>;

    // Union type (like enum)
    template <typename... Variants>
    using Union = std::variant<Variants...>;

    // For pattern matching (from your previous setup, adapted for exhaustiveness)
    template <class... Handlers>
    struct match : Handlers... {
        using Handlers::operator()...;
    };

    // Overload >> for visitation (as in your history)
    template <typename Variant, typename Matcher>
    void operator>>(Variant&& v, Matcher&& m) {
        std::visit(std::forward<Matcher>(m), std::forward<Variant>(v));
    }

    // Overloaded helper for macro-based matching (same as match but for separation)
    template <class... Ts>
    struct overloaded : Ts... {
        using Ts::operator()...;
    };

    // Use C++23 std::expected for Result<T, E> (Rust-like, with built-in methods)
    template <typename T, typename E = std::string>
    using Result = std::expected<T, E>;  // Holds T (success) or unexpected<E> (error)
}

// Macro-based match syntax for a more Rust-like feel (inspired by common emulations)
#define ox_match std::visit([](auto&&... args) { return oxide::overloaded{std::forward<decltype(args)>(args)...}; }
#define ox_match_case(param) [](param)
#define ox_match_value(value) , std::forward<decltype(value)>(value))
