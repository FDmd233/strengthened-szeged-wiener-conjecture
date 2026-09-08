#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <vector>
#include "exact_graph.hpp"

using szw::Graph;
using szw::Mask;

struct Piece {
    Graph block;
    int cut_vertex = -1;
    Mask selected_neighbours = 0; // T in N(u)={v} union T
    long long eta_value = 0;
};

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

static Mask subset_as_vertex_mask(const Graph& c, int v, unsigned subset_index) {
    std::vector<int> neighbours;
    for (int w = 0; w < static_cast<int>(c.size()); ++w) {
        if (szw::adjacent(c, v, w)) {
            neighbours.push_back(w);
        }
    }
    Mask t = 0;
    for (int j = 0; j < static_cast<int>(neighbours.size()); ++j) {
        if (((subset_index >> j) & 1U) != 0U) {
            t = static_cast<Mask>(t | static_cast<Mask>(1U << neighbours[j]));
        }
    }
    return t;
}

static Graph make_piece(const Piece& p) {
    const Mask neighbourhood = static_cast<Mask>(
        static_cast<Mask>(1U << p.cut_vertex) | p.selected_neighbours);
    return szw::add_vertex(p.block, neighbourhood);
}

// Glue two pieces along their common dominated vertex u and dominator/cut
// vertex v. Their blocks meet only in v and there are no other cross edges.
static Graph glue(const Piece& a, const Piece& b) {
    const int m1 = static_cast<int>(a.block.size());
    const int m2 = static_cast<int>(b.block.size());
    const int n = m1 + m2; // (m1+1)+(m2+1)-2 shared vertices
    Graph g(n, 0);

    const int u = 0;
    const int v = 1;
    int next = 2;
    std::vector<int> map_a(m1), map_b(m2);
    map_a[a.cut_vertex] = v;
    for (int x = 0; x < m1; ++x) {
        if (x != a.cut_vertex) {
            map_a[x] = next++;
        }
    }
    map_b[b.cut_vertex] = v;
    for (int x = 0; x < m2; ++x) {
        if (x != b.cut_vertex) {
            map_b[x] = next++;
        }
    }

    const auto add_edge = [&g](int x, int y) {
        g[x] = static_cast<Mask>(g[x] | static_cast<Mask>(1U << y));
        g[y] = static_cast<Mask>(g[y] | static_cast<Mask>(1U << x));
    };
    add_edge(u, v);

    for (int x = 0; x < m1; ++x) {
        for (int y = x + 1; y < m1; ++y) {
            if (szw::adjacent(a.block, x, y)) {
                add_edge(map_a[x], map_a[y]);
            }
        }
    }
    for (int x = 0; x < m2; ++x) {
        for (int y = x + 1; y < m2; ++y) {
            if (szw::adjacent(b.block, x, y)) {
                add_edge(map_b[x], map_b[y]);
            }
        }
    }

    for (int x = 0; x < m1; ++x) {
        if (((a.selected_neighbours >> x) & 1U) != 0U) {
            add_edge(u, map_a[x]);
        }
    }
    for (int x = 0; x < m2; ++x) {
        if (((b.selected_neighbours >> x) & 1U) != 0U) {
            add_edge(u, map_b[x]);
        }
    }
    return g;
}

int main() {
    using namespace szw;
    const auto bases = load_biconnected_bases();

    std::vector<Piece> p6_low;   // eta 8 or 9
    std::vector<Piece> p6_min;   // eta 8
    std::vector<Piece> p7_min;   // eta 11

    for (const Graph& c : bases.at(5)) {
        for (int v = 0; v < 5; ++v) {
            const int d = popcount(c[v]);
            for (unsigned subset = 1; subset < (1U << d); ++subset) {
                const Mask t = subset_as_vertex_mask(c, v, subset);
                Piece p{c, v, t, 0};
                p.eta_value = eta(make_piece(p));
                if (p.eta_value == 8 || p.eta_value == 9) {
                    p6_low.push_back(p);
                }
                if (p.eta_value == 8) {
                    p6_min.push_back(p);
                }
            }
        }
    }

    for (const Graph& c : bases.at(6)) {
        for (int v = 0; v < 6; ++v) {
            const int d = popcount(c[v]);
            for (unsigned subset = 1; subset < (1U << d); ++subset) {
                const Mask t = subset_as_vertex_mask(c, v, subset);
                Piece p{c, v, t, 0};
                p.eta_value = eta(make_piece(p));
                if (p.eta_value == 11) {
                    p7_min.push_back(p);
                }
            }
        }
    }

    std::cout << "P6 " << p6_low.size()
              << " P6min " << p6_min.size()
              << " P7min " << p7_min.size() << '\n';

    long long count10 = 0;
    long long bad10 = 0;
    long long minimum10 = std::numeric_limits<long long>::max();
    for (std::size_t i = 0; i < p6_low.size(); ++i) {
        for (std::size_t j = i; j < p6_low.size(); ++j) {
            if (p6_low[i].eta_value + p6_low[j].eta_value >= 18) {
                continue;
            }
            const long long e = eta(glue(p6_low[i], p6_low[j]));
            ++count10;
            minimum10 = std::min(minimum10, e);
            if (e < 20) {
                ++bad10;
            }
        }
    }
    std::cout << "n10 " << count10 << " min " << minimum10
              << " bad " << bad10 << '\n';

    long long count11 = 0;
    long long bad11 = 0;
    long long minimum11 = std::numeric_limits<long long>::max();
    for (const Piece& a : p6_min) {
        for (const Piece& b : p7_min) {
            const long long e = eta(glue(a, b));
            ++count11;
            minimum11 = std::min(minimum11, e);
            if (e < 22) {
                ++bad11;
            }
        }
    }
    std::cout << "n11 " << count11 << " min " << minimum11
              << " bad " << bad11 << '\n';

    return (bad10 == 0 && bad11 == 0) ? 0 : 3;
}
