#include <fstream>
#include <iostream>
#include <limits>
#include <vector>
#include "exact_graph.hpp"

using szw::Graph;
using szw::Mask;

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
        graphs.push_back(g);
    }
    return graphs;
}

int main() {
    using namespace szw;
    const auto bases = load_connected_seven();
    if (bases.size() != 853) {
        std::cerr << "expected 853 connected seven-vertex Atlas graphs, got "
                  << bases.size() << '\n';
        return 2;
    }

    // F has order 8, is 2-connected, and eta(F)<=15.
    // Every such F is covered: delete any vertex of F, identify the resulting
    // connected seven-vertex graph with an Atlas representative, then restore
    // the deleted vertex by its nonempty neighbourhood mask.
    std::vector<Graph> F8;
    for (const Graph& h : bases) {
        for (unsigned mask = 1; mask < (1U << 7); ++mask) {
            Graph f = add_vertex(h, static_cast<Mask>(mask));
            if (biconnected(f) && eta(f) <= 15) {
                F8.push_back(std::move(f));
            }
        }
    }
    std::cout << "F8 " << F8.size() << '\n';

    // Generate every dominated extension H with eta(H)<=17, retaining
    // repetitions and discarding only the three exceptional order-nine graphs.
    std::vector<Graph> H9;
    for (const Graph& f : F8) {
        for (int v = 0; v < 8; ++v) {
            const int d = popcount(f[v]);
            for (unsigned subset = 1; subset < (1U << d); ++subset) {
                Graph h = dominated_extension(f, v, subset);
                if (!exceptional_family(h) && eta(h) <= 17) {
                    H9.push_back(std::move(h));
                }
            }
        }
    }
    std::cout << "H9 " << H9.size() << '\n';

    // Final dominated extensions. No isomorphism reduction is used, and we do
    // not impose an extra 2-connectivity filter: this only enlarges the checked
    // universe relative to what the proof needs.
    long long generated = 0;
    long long bad = 0;
    long long minimum = std::numeric_limits<long long>::max();
    for (const Graph& h : H9) {
        for (int v = 0; v < 9; ++v) {
            const int d = popcount(h[v]);
            for (unsigned subset = 1; subset < (1U << d); ++subset) {
                Graph g = dominated_extension(h, v, subset);
                if (exceptional_family(g)) {
                    continue;
                }
                const long long e = eta(g);
                ++generated;
                minimum = std::min(minimum, e);
                if (e < 20) {
                    ++bad;
                }
            }
        }
    }

    std::cout << "G10 " << generated << " min " << minimum
              << " bad " << bad << '\n';
    return bad == 0 ? 0 : 3;
}
