#include <iostream>
#include <random>
#include <string>
#include <set>
#include <vector>
#include <sstream>
#include <algorithm>

std::string to_string(const std::vector<int> &v) {
    std::stringstream ss;
    for(const int &n : v) {
        ss << n;
    }

    return ss.str();
}

int main() {
    // Seed the random number generator with current time
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define the range of random numbers (0-9 inclusive)
    std::uniform_int_distribution<int> dis(0, 9);

    std::vector<int> lock_code;

    // Iterate until a match is found
    while (lock_code.size() < 3) {
        int n = dis(gen);
        lock_code.push_back(n);
    }

    std::cout << "generated lock combination: " << to_string(lock_code) << "\n";

    const std::set<int> numbers{0,1,2,3,4,5,6,7,8,9};
    std::vector<int> code;
    uint32_t try_count = 0;

    for (auto &n1 : numbers) {
        for (auto &n2 : numbers) {
            for (auto &n3 : numbers) {
                code = {n1, n2, n3};
                try_count++;
                if( std::equal(lock_code.begin(), lock_code.end(), code.begin()) ) {
                    std::cout << "found the right combination after " << try_count << " tries\n";
                    break;
                }
            }
        }
    }

    return 0;
}