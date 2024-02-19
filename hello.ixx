// global module fragment where #includes can happen
module;

// first thing after the global module fragment must
// be a module command.
export module hello;

import std.core;

class Cow {
public:
    Cow() = default;
    ~Cow() = default;
    std::string operator()() {
        return "moo";
    };
};

export class Foo {
public:
    Foo();
    ~Foo() = default;
    void helloWorld();
    void cowSay();
};

Foo::Foo() = default;

void Foo::helloWorld() {
    std::cout << "Hello world!\n";
}

void Foo::cowSay() {
    Cow cow;
    std::cout << "The cow says " << cow() << "\n";
}