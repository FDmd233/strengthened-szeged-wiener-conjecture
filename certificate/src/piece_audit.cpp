#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <vector>
#include "exact_graph.hpp"

using szw::Graph;
using szw::Mask;

static std::map<int, std::vector<Graph>> load_biconnected_bases() {
    std::ifstream in("input/atlas_bases_4_7.txt");
    if (!in) {
        throw std::runtime_error("cannot open input/atlas_bases_4_7.txt");
    }
    std::map<int, std::vector<Graph>> bases;
    while (true) {
        int n = 0;
        if (!(in >> n)) {
            break;
        }
        Graph g(n);
        for (int i = 0; i < n; ++i) {
            int x = 0;
            in >> x;
            g[i] = static_cast<Mask>(x);
        }
        if (!szw::exceptional_family(g)) {
            bases[n].push_back(std::move(g));
        }
    }
    return bases;
}

static std::vector<Graph> load_connected_seven() {
    std::ifstream in("input/connected7_masks.txt");
    if (!in) {
        throw std::runtime_error("cannot open input/connected7_masks.txt");
    }
    std::vector<Graph> graphs;
    while (true) {
        Graph g(7);
        int x = 0;
        if (!(in >> x)) {
            break;
        }
        g[0] = static_cast<Mask>(x);
        for (int i = 1; i < 7; ++i) {
            in >> x;
            g[i] = static_cast<Mask>(x);
        }
        graphs.push_back(std::move(g));
    }
    return graphs;
}

int main() {
    using namespace szw;
    const auto bases = load_biconnected_bases();

    // Exact piece minima L(5),...,L(8). A piece is a dominated extension of a
    // biconnected, non-special deletion block C.
    for (int m = 4; m <= 7; ++m) {
        long long minimum = std::numeric_limits<long long>::max();
        long long generated = 0;
        for (const Graph& c : bases.at(m)) {
            for (int v = 0; v < m; ++v) {
                const int d = popcount(c[v]);
                for (unsigned subset = 1; subset < (1U << d); ++subset) {
                    const Graph g = dominated_extension(c, v, subset);
                    minimum = std::min(minimum, eta(g));
                    ++generated;
                }
            }
        }
        std::cout << "L" << (m + 1) << '=' << minimum
                  << " pieces " << generated << '\n';
    }

    // For L(9), Lemma 2.2 of the paper reduces the task to biconnected,
    // non-special order-eight deletion graphs C with eta(C)<=14.
    const auto seven = load_connected_seven();
    long long low_order8_occurrences = 0;
    long long minimum_order9 = std::numeric_limits<long long>::max();
    std::map<long long, long long> histogram;

    for (const Graph& h : seven) {
        for (unsigned mask = 1; mask < (1U << 7); ++mask) {
            Graph c = add_vertex(h, static_cast<Mask>(mask));
            if (!biconnected(c) || exceptional_family(c) || eta(c) > 14) {
                continue;
            }
            ++low_order8_occurrences;
            for (int v = 0; v < 8; ++v) {
                const int d = popcount(c[v]);
                for (unsigned subset = 1; subset < (1U << d); ++subset) {
                    const Graph g = dominated_extension(c, v, subset);
                    const long long e = eta(g);
                    minimum_order9 = std::min(minimum_order9, e);
                    ++histogram[e];
                }
            }
        }
    }

    std::cout << "order8 nonspecial eta<=14 labelled_ext "
              << low_order8_occurrences
              << " L9_from_low=" << minimum_order9 << '\n';
    for (const auto& [e, count] : histogram) {
        if (e <= 20) {
            std::cout << " eta9 " << e << " count " << count << '\n';
        }
    }
    return 0;
}
