#include <iostream>
#include "exact_graph.hpp"

int main() {
    using namespace szw;
    for (int n = 4; n <= 12; ++n) {
        for (int t = 1; t <= n - 1; ++t) {
            const Graph g = Knt(n, t);
            const long long expected = 2LL * (t - 1) * (n - t - 1);
            if (eta(g) != expected) {
                std::cerr << "K_n^t self-test failed at n=" << n
                          << " t=" << t << '\n';
                return 2;
            }
        }
    }
    std::cout << "K_n^t formula self-test: OK\n";
    return 0;
}
