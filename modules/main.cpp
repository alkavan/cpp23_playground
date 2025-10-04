import hello;
import scopes;

#ifdef _MSC_VER
#include <iostream>
#else
import <iostream>;
#endif

int main()
{
    Foo foo;
    foo.helloWorld();
    foo.cowSay();

    constexpr int answer = example::answer_to_the_ultimate_question();

    std::cout << "The answer to the ultimate question of"
                 " Life, The Universe, and Everything is "
                 << answer << "\n";

    return 0;
}
