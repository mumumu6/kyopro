#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

using ll  = long long;
using pll = pair<ll, ll>;

// ---------- Torus distance ----------
static inline ll torus_dist2(const pll &a, const pll &b, ll L) {
    ll dx = abs(a.first - b.first);
    dx    = min(dx, L - dx);
    ll dy = abs(a.second - b.second);
    dy    = min(dy, L - dy);
    return dx * dx + dy * dy;
}

struct AssignResult {
    long long cost = (1LL << 60);
    vector<vector<int>> clusters;
};

// Balanced assignment with fixed seeds using min-cost flow.
static AssignResult assign_with_flow(const vector<int> &seeds, const vector<pll> &pos, ll L, int M, int K, int N) {
    const int S          = 0;
    const int seed_off   = 1;
    const int point_off  = seed_off + M;
    const int sink       = point_off + N;
    mcf_graph<int, long long> g(sink + 1);

    // source -> seed (capacity K-1 because seed itself is already included)
    for (int si = 0; si < M; ++si) g.add_edge(S, seed_off + si, K - 1, 0);

    vector<char> is_seed(N, 0);
    for (int s : seeds) is_seed[s] = 1;

    // seed -> point edges (skip the seed point itself)
    for (int si = 0; si < M; ++si) {
        int sidx   = seeds[si];
        int snode  = seed_off + si;
        for (int pi = 0; pi < N; ++pi) {
            if (pi == sidx) continue;
            ll c = torus_dist2(pos[sidx], pos[pi], L);
            g.add_edge(snode, point_off + pi, 1, c);
        }
    }

    // point -> sink
    for (int pi = 0; pi < N; ++pi) {
        int cap = is_seed[pi] ? 0 : 1;
        g.add_edge(point_off + pi, sink, cap, 0);
    }

    auto [flow, cost] = g.flow(S, sink);
    AssignResult res;
    if (flow != N - M) return res; // invalid
    res.cost     = cost;
    res.clusters.assign(M, {});
    for (int i = 0; i < M; ++i) res.clusters[i].push_back(seeds[i]);

    for (auto &e : g.edges()) {
        if (e.from >= seed_off && e.from < seed_off + M &&
            e.to >= point_off && e.to < point_off + N && e.flow > 0) {
            int ci = e.from - seed_off;
            int pi = e.to - point_off;
            res.clusters[ci].push_back(pi);
        }
    }
    return res;
}

// Compute medoid (point with minimum sum of squared torus distances) inside a cluster.
static int medoid(const vector<int> &cluster, const vector<pll> &pos, ll L) {
    long long best = (1LL << 60);
    int best_idx   = cluster[0];
    for (int c : cluster) {
        long long cur = 0;
        for (int d : cluster) cur += torus_dist2(pos[c], pos[d], L);
        if (cur < best) {
            best     = cur;
            best_idx = c;
        }
    }
    return best_idx;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, T, M, K, L;
    if (!(cin >> N >> T >> M >> K >> L)) return 0;

    vector<pll> pos(N), vel(N);
    for (ll i = 0; i < N; ++i) cin >> pos[i].first >> pos[i].second >> vel[i].first >> vel[i].second;

    mt19937 rng(1234567);
    const int TRY      = 80;
    const int IMPROVE  = 4; // k-medoids improvement rounds per start

    long long global_best_cost = (1LL << 60);
    vector<int> global_best_seeds;
    vector<vector<int>> global_best_clusters;

    vector<int> indices(N);
    iota(indices.begin(), indices.end(), 0);

    for (int trial = 0; trial < TRY; ++trial) {
        // k-means++ style seeding
        vector<int> seeds;
        seeds.reserve(M);
        seeds.push_back(indices[rng() % N]);
        while ((int)seeds.size() < M) {
            long long best_d = -1;
            int best_i       = -1;
            for (int i = 0; i < N; ++i) {
                bool already = false;
                for (int s : seeds) if (s == i) { already = true; break; }
                if (already) continue;
                long long mind = (1LL << 60);
                for (int s : seeds) mind = min(mind, torus_dist2(pos[i], pos[s], L));
                if (mind > best_d) {
                    best_d = mind;
                    best_i = i;
                }
            }
            seeds.push_back(best_i);
        }

        long long last_cost = (1LL << 60);
        for (int it = 0; it < IMPROVE; ++it) {
            auto res = assign_with_flow(seeds, pos, L, (int)M, (int)K, (int)N);
            if (res.cost >= (1LL << 60)) break;
            if (res.cost < last_cost) last_cost = res.cost;

            // update seeds to medoids
            vector<int> new_seeds(M);
            for (int i = 0; i < M; ++i) new_seeds[i] = medoid(res.clusters[i], pos, L);
            if (new_seeds == seeds) {
                // converged
                if (res.cost < global_best_cost) {
                    global_best_cost     = res.cost;
                    global_best_seeds    = seeds;
                    global_best_clusters = move(res.clusters);
                }
                break;
            }
            seeds = move(new_seeds);

            if (res.cost < global_best_cost) {
                global_best_cost     = res.cost;
                global_best_seeds    = seeds;
                global_best_clusters = move(res.clusters);
            }
        }
    }

    // Fallback: if no assignment succeeded (should not happen), connect greedily to avoid empty output.
    if (global_best_clusters.empty()) {
        dsu uf(N);
        vector<pair<ll, pair<int, int>>> edges;
        for (int i = 0; i < N; ++i) for (int j = i + 1; j < N; ++j)
            edges.push_back({torus_dist2(pos[i], pos[j], L), {i, j}});
        sort(edges.begin(), edges.end());
        for (auto &e : edges) {
            auto [i, j] = e.second;
            if (!uf.same(i, j)) {
                cout << 0 << ' ' << i << ' ' << j << '\n';
                uf.merge(i, j);
            }
        }
        return 0;
    }

    // Output merges at t = 0 as star from each seed to its cluster members.
    for (int ci = 0; ci < (int)global_best_clusters.size(); ++ci) {
        int root = global_best_seeds[ci];
        for (int node : global_best_clusters[ci]) {
            if (node == root) continue;
            cout << 0 << ' ' << root << ' ' << node << '\n';
        }
    }
    return 0;
}
