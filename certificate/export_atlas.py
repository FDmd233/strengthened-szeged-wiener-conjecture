"""Optional regeneration of the fixed Atlas snapshots.

Requires NetworkX 3.6.1.  The proof verifiers do not import NetworkX; they read
only the fixed files under input/.  After regeneration, run verify.sh, which
checks the expected SHA256 digests.
"""
from pathlib import Path
import hashlib
import networkx as nx

ROOT = Path(__file__).resolve().parent
OUT = ROOT / "input"
OUT.mkdir(exist_ok=True)


def adjacency_masks(G):
    G = nx.convert_node_labels_to_integers(G)
    result = []
    for i in range(len(G)):
        mask = 0
        for j in G.neighbors(i):
            mask |= 1 << j
        result.append(mask)
    return result


atlas = nx.graph_atlas_g()

with (OUT / "connected7_masks.txt").open("w", encoding="ascii") as fm, \
     (OUT / "connected7_graph6.txt").open("wb") as fg:
    count = 0
    for G in atlas:
        if len(G) == 7 and nx.is_connected(G):
            G = nx.convert_node_labels_to_integers(G)
            fm.write(" ".join(map(str, adjacency_masks(G))) + "\n")
            fg.write(nx.to_graph6_bytes(G, header=False))
            count += 1
    assert count == 853

with (OUT / "atlas_bases_4_7.txt").open("w", encoding="ascii") as f:
    count = 0
    for G in atlas:
        n = len(G)
        if 4 <= n <= 7 and nx.is_biconnected(G):
            f.write(str(n) + " " + " ".join(map(str, adjacency_masks(G))) + "\n")
            count += 1
    assert count == 537

for name in ("connected7_graph6.txt", "connected7_masks.txt", "atlas_bases_4_7.txt"):
    path = OUT / name
    print(name, hashlib.sha256(path.read_bytes()).hexdigest())
