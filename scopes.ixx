export module scopes;

#define ANSWER 42

export namespace example {
    constexpr int answer_42() {
        return ANSWER;
    }

    constexpr int answer_to_the_ultimate_question() {
        return answer_42();
    }
}
