#ifndef SZEGED_WIENER_EXACT_GRAPH_HPP
#define SZEGED_WIENER_EXACT_GRAPH_HPP

#include <algorithm>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <vector>

namespace szw {

using Mask = std::uint16_t;
using Graph = std::vector<Mask>;

inline int popcount(Mask x) {
    return __builtin_popcount(static_cast<unsigned>(x));
}

inline int first_bit(Mask x) {
    return __builtin_ctz(static_cast<unsigned>(x));
}

inline bool adjacent(const Graph& g, int u, int v) {
    return ((g[u] >> v) & 1U) != 0U;
}

// Compute eta(G)=Sz(G)-W(G) directly from the defining distance formulas.
// All arithmetic is integral. The verifier only calls this on connected graphs.
inline long long eta(const Graph& g) {
    const int n = static_cast<int>(g.size());
    if (n <= 0 || n > 16) {
        throw std::runtime_error("graph order outside verifier range");
    }

    int dist[16][16]{};
    long long wiener = 0;
    const Mask all = static_cast<Mask>((1U << n) - 1U);

    for (int source = 0; source < n; ++source) {
        Mask seen = static_cast<Mask>(1U << source);
        Mask frontier = seen;
        int level = 0;

        while (seen != all) {
            ++level;
            Mask next = 0;
            for (Mask f = frontier; f != 0; f = static_cast<Mask>(f & (f - 1))) {
                const int v = first_bit(f);
                next = static_cast<Mask>(next | g[v]);
            }
            next = static_cast<Mask>(next & static_cast<Mask>(~seen) & all);
            if (next == 0) {
                throw std::runtime_error("eta called on a disconnected graph");
            }

            for (Mask q = next; q != 0; q = static_cast<Mask>(q & (q - 1))) {
                const int v = first_bit(q);
                dist[source][v] = level;
                if (source < v) {
                    wiener += level;
                }
            }
            seen = static_cast<Mask>(seen | next);
            frontier = next;
        }
    }

    long long szeged = 0;
    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {
            if (!adjacent(g, u, v)) {
                continue;
            }
            int n_u = 0;
            int n_v = 0;
            for (int x = 0; x < n; ++x) {
                if (dist[x][u] < dist[x][v]) {
                    ++n_u;
                } else if (dist[x][v] < dist[x][u]) {
                    ++n_v;
                }
            }
            szeged += static_cast<long long>(n_u) * n_v;
        }
    }
    return szeged - wiener;
}

inline bool connected_after_deleting(const Graph& g, int removed) {
    const int n = static_cast<int>(g.size());
    const Mask full = static_cast<Mask>((1U << n) - 1U);
    const Mask allowed = static_cast<Mask>(full ^ static_cast<Mask>(1U << removed));
    if (allowed == 0) {
        return true;
    }

    const int start = first_bit(allowed);
    Mask seen = static_cast<Mask>(1U << start);
    Mask frontier = seen;
    while (frontier != 0) {
        Mask next = 0;
        for (Mask f = frontier; f != 0; f = static_cast<Mask>(f & (f - 1))) {
            const int v = first_bit(f);
            next = static_cast<Mask>(next | g[v]);
        }
        next = static_cast<Mask>(next & allowed & static_cast<Mask>(~seen));
        seen = static_cast<Mask>(seen | next);
        frontier = next;
    }
    return seen == allowed;
}

inline bool biconnected(const Graph& g) {
    const int n = static_cast<int>(g.size());
    if (n < 3) {
        return false;
    }
    for (int v = 0; v < n; ++v) {
        if (!connected_after_deleting(g, v)) {
            return false;
        }
    }
    return true;
}

inline std::vector<int> degree_sequence(const Graph& g) {
    std::vector<int> d;
    d.reserve(g.size());
    for (Mask m : g) {
        d.push_back(popcount(m));
    }
    std::sort(d.begin(), d.end());
    return d;
}

// The degree sequences below uniquely determine K_n, K_n^2, and K_n^{n-2}.
inline bool exceptional_family(const Graph& g) {
    const int n = static_cast<int>(g.size());
    const auto d = degree_sequence(g);

    if (d == std::vector<int>(n, n - 1)) {
        return true;
    }
    if (n < 4) {
        return false;
    }

    std::vector<int> k2;
    k2.push_back(2);
    for (int i = 0; i < n - 3; ++i) {
        k2.push_back(n - 2);
    }
    k2.push_back(n - 1);
    k2.push_back(n - 1);
    std::sort(k2.begin(), k2.end());
    if (d == k2) {
        return true;
    }

    std::vector<int> kn2{n - 2, n - 2};
    for (int i = 0; i < n - 2; ++i) {
        kn2.push_back(n - 1);
    }
    std::sort(kn2.begin(), kn2.end());
    return d == kn2;
}

inline Graph add_vertex(const Graph& h, Mask neighbourhood) {
    const int n = static_cast<int>(h.size());
    Graph g = h;
    g.push_back(neighbourhood);
    for (int v = 0; v < n; ++v) {
        if (((neighbourhood >> v) & 1U) != 0U) {
            g[v] = static_cast<Mask>(g[v] | static_cast<Mask>(1U << n));
        }
    }
    return g;
}

// Add a new vertex u with N(u)={v} union T, where T is encoded as a
// nonempty subset of the neighbours of v. This is exactly Lemma 2.6 in the paper.
inline Graph dominated_extension(const Graph& h, int v, unsigned subset_index) {
    std::vector<int> neighbours;
    for (int w = 0; w < static_cast<int>(h.size()); ++w) {
        if (adjacent(h, v, w)) {
            neighbours.push_back(w);
        }
    }
    if (subset_index == 0 || subset_index >= (1U << neighbours.size())) {
        throw std::runtime_error("invalid dominated-extension subset");
    }

    Mask neighbourhood = static_cast<Mask>(1U << v);
    for (int j = 0; j < static_cast<int>(neighbours.size()); ++j) {
        if (((subset_index >> j) & 1U) != 0U) {
            neighbourhood = static_cast<Mask>(
                neighbourhood | static_cast<Mask>(1U << neighbours[j]));
        }
    }
    return add_vertex(h, neighbourhood);
}

inline Graph complete_graph(int n) {
    Graph g(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            g[i] = static_cast<Mask>(g[i] | static_cast<Mask>(1U << j));
            g[j] = static_cast<Mask>(g[j] | static_cast<Mask>(1U << i));
        }
    }
    return g;
}

inline Graph Knt(int n, int t) {
    Graph g = complete_graph(n - 1);
    Mask neighbourhood = 0;
    for (int i = 0; i < t; ++i) {
        neighbourhood = static_cast<Mask>(neighbourhood | static_cast<Mask>(1U << i));
    }
    return add_vertex(g, neighbourhood);
}

}  // namespace szw

#endif
