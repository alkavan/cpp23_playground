export module hello;

#ifdef _MSC_VER
import std.core;
#else
import <iostream>;
import <string>;
#endif

class Cow {
public:
    Cow() = default;
    ~Cow() = default;
    std::string operator()() const
    {
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
    const Cow cow;
    std::cout << "The cow says " << cow() << "\n";
}