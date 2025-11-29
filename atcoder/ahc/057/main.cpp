#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

using ll  = long long;
using pll = pair<ll, ll>;

// ---------- Utility ----------
static inline ll torus_dist2(const pll &a, const pll &b, ll L) {
    ll dx = abs(a.first - b.first);
    dx    = min(dx, L - dx);
    ll dy = abs(a.second - b.second);
    dy    = min(dy, L - dy);
    return dx * dx + dy * dy;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, T, M, K, L;
    if (!(cin >> N >> T >> M >> K >> L)) return 0;

    vector<pll> pos(N), vel(N);
    for (ll i = 0; i < N; ++i) cin >> pos[i].first >> pos[i].second >> vel[i].first >> vel[i].second;

    // --------------- Seed generation ----------------
    // We only merge at t = 0, so the task reduces to partitioning the 300 points into
    // 10 clusters of size 30 that minimize sum of squared torus distances to a seed.
    // We try multiple randomized farthest-point seed sets and keep the best by min-cost flow assignment.
    mt19937 rng(712367);
    const int TRY = 40; // moderate iterations; fast and deterministic with fixed seed

    vector<int> best_seeds;
    ll best_cost = (1LL << 60);
    vector<int> candidate(N);
    iota(candidate.begin(), candidate.end(), 0);

    for (int iter = 0; iter < TRY; ++iter) {
        // k-means++ like seeding on torus
        vector<int> seeds;
        seeds.reserve(M);
        seeds.push_back(candidate[rng() % N]);
        while ((int)seeds.size() < M) {
            ll best_d = -1;
            int best_i = -1;
            for (int i = 0; i < N; ++i) {
                ll mind = (1LL << 60);
                for (int s : seeds) mind = min(mind, torus_dist2(pos[i], pos[s], L));
                if (mind > best_d) {
                    best_d = mind;
                    best_i = i;
                }
            }
            seeds.push_back(best_i);
        }

        // --------------- Balanced assignment by min-cost flow ----------------
        // Seeds are fixed; each seed must include itself, remaining capacity 29.
        // Graph: source -> seed nodes -> point nodes -> sink.
        int S = 0;
        int seed_offset = 1;
        int point_offset = seed_offset + M;
        int sink = point_offset + N;
        mcf_graph<int, long long> g(sink + 1);

        // source to seeds
        for (int si = 0; si < M; ++si) {
            int node = seed_offset + si;
            g.add_edge(S, node, (int)(K - 1), 0); // already contains itself
        }

        // seeds to points (excluding the seed point itself)
        for (int si = 0; si < M; ++si) {
            int sidx = seeds[si];
            int snode = seed_offset + si;
            for (int pi = 0; pi < N; ++pi) {
                if (pi == sidx) continue;
                ll c = torus_dist2(pos[sidx], pos[pi], L);
                g.add_edge(snode, point_offset + pi, 1, c);
            }
        }

        // points to sink
        for (int pi = 0; pi < N; ++pi) {
            // seed points have been pre-assigned, skip them in the flow
            bool is_seed = false;
            for (int s : seeds) {
                if (s == pi) {
                    is_seed = true;
                    break;
                }
            }
            int cap = is_seed ? 0 : 1;
            g.add_edge(point_offset + pi, sink, cap, 0);
        }

        auto [flow, cost] = g.flow(S, sink);
        if (flow != (int)(N - M)) continue; // should always succeed

        if (cost < best_cost) {
            best_cost = cost;
            best_seeds = seeds;
        }
    }

    // --------------- Build clusters with best seeds ----------------
    vector<int> seed_id(N, -1);
    for (int i = 0; i < (int)best_seeds.size(); ++i) seed_id[best_seeds[i]] = i;

    // Re-run assignment for the best seed set to recover cluster members.
    int S = 0;
    int seed_offset = 1;
    int point_offset = seed_offset + M;
    int sink = point_offset + N;
    mcf_graph<int, long long> g(sink + 1);
    for (int si = 0; si < M; ++si) g.add_edge(S, seed_offset + si, (int)(K - 1), 0);
    for (int si = 0; si < M; ++si) {
        int sidx = best_seeds[si];
        int snode = seed_offset + si;
        for (int pi = 0; pi < N; ++pi) {
            if (pi == sidx) continue;
            ll c = torus_dist2(pos[sidx], pos[pi], L);
            g.add_edge(snode, point_offset + pi, 1, c);
        }
    }
    for (int pi = 0; pi < N; ++pi) {
        bool is_seed = seed_id[pi] != -1;
        g.add_edge(point_offset + pi, sink, is_seed ? 0 : 1, 0);
    }
    g.flow(S, sink);

    vector<vector<int>> clusters(M);
    for (int i = 0; i < M; ++i) clusters[i].push_back(best_seeds[i]);
    // Inspect edges from seeds to points to reconstruct assignment.
    for (auto &e : g.edges()) {
        if (e.from >= seed_offset && e.from < seed_offset + M &&
            e.to >= point_offset && e.to < point_offset + N && e.flow > 0) {
            int si = e.from - seed_offset;
            int pi = e.to - point_offset;
            clusters[si].push_back(pi);
        }
    }

    // --------------- Output merges ----------------
    // Merge each non-seed node to its seed at t = 0.
    for (int ci = 0; ci < M; ++ci) {
        int root = clusters[ci][0];
        for (int j = 1; j < (int)clusters[ci].size(); ++j) {
            cout << 0 << ' ' << root << ' ' << clusters[ci][j] << '\n';
        }
    }

    return 0;
}
