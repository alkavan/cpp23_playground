import hello;
import scopes;

#include <iostream>

int main()
{
    Foo foo;
    foo.helloWorld();
    foo.cowSay();

    const int answer = example::answer_to_the_ultimate_question();

    std::cout << "The answer to the ultimate question of"
                 " Life, The Universe, and Everything is "
                 << answer << "\n";

    return 0;
}
