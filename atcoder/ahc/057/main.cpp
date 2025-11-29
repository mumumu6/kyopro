#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using ll  = long long;
using pll = pair<ll, ll>;
#define rep(i, n) for (ll i = 0; i < (n); i++)

struct Timer {
    chrono::steady_clock::time_point st;
    Timer() : st(chrono::steady_clock::now()) {}
    double ms() const { return chrono::duration<double, std::milli>(chrono::steady_clock::now() - st).count(); }
};

// min-cost flow assignment for fixed seeds, returns {cost, edges}
static pair<ll, vector<pair<int, int>>> assign_with_flow(const vector<int> &seeds, const vector<pll> &pos, ll L, int M, int K, int N) {
    const int S          = 0;
    const int seed_off   = 1;
    const int point_off  = seed_off + M;
    const int T          = point_off + N;
    mcf_graph<int, ll> g(T + 1);

    for (int si = 0; si < M; ++si) g.add_edge(S, seed_off + si, K - 1, 0); // seed already counts itself

    vector<char> is_seed(N, 0);
    for (int s : seeds) is_seed[s] = 1;

    auto torus_dist2 = [&](const pll &a, const pll &b) {
        ll dx = abs(a.first - b.first);
        dx    = min(dx, L - dx);
        ll dy = abs(a.second - b.second);
        dy    = min(dy, L - dy);
        return dx * dx + dy * dy;
    };

    for (int si = 0; si < M; ++si) {
        int sidx  = seeds[si];
        int snode = seed_off + si;
        for (int pi = 0; pi < N; ++pi) {
            if (pi == sidx) continue; // seed itself already counted
            ll d2 = torus_dist2(pos[sidx], pos[pi]);
            g.add_edge(snode, point_off + pi, 1, d2);
        }
    }
    for (int pi = 0; pi < N; ++pi) g.add_edge(point_off + pi, T, 1, 0);

    auto res = g.flow(S, T, M * (K - 1));
    if (res.first != (ll)M * (K - 1)) return { (ll)4e18, {} }; // infeasible

    vector<pair<int, int>> edges;
    for (auto &e : g.edges()) {
        if (e.from >= seed_off && e.from < seed_off + M && e.flow > 0) {
            int si = e.from - seed_off;
            int pi = e.to - point_off;
            edges.push_back({ seeds[si], pi });
        }
    }
    return { res.second, edges };
}

// k-means++ style seeding
static vector<int> kpp_seeds(mt19937 &rng, int N, int M, const vector<pll> &pos, ll L) {
    uniform_int_distribution<int> uni(0, N - 1);
    vector<int> seeds;
    seeds.reserve(M);
    seeds.push_back(uni(rng));

    vector<ll> dist2(N, (ll)4e18);
    auto torus_dist2 = [&](const pll &a, const pll &b) {
        ll dx = abs(a.first - b.first);
        dx    = min(dx, L - dx);
        ll dy = abs(a.second - b.second);
        dy    = min(dy, L - dy);
        return dx * dx + dy * dy;
    };

    while ((int)seeds.size() < M) {
        ll sum = 0;
        rep(i, N) {
            ll d2 = torus_dist2(pos[i], pos[seeds.back()]);
            dist2[i] = min(dist2[i], d2);
            sum += dist2[i];
        }
        if (sum == 0) {
            // all same point; just pick unused lowest index
            for (int i = 0; i < N; ++i) {
                if (find(seeds.begin(), seeds.end(), i) == seeds.end()) {
                    seeds.push_back(i);
                    break;
                }
            }
            continue;
        }
        uniform_real_distribution<double> pick(0.0, (double)sum);
        double r = pick(rng);
        int nxt  = -1;
        rep(i, N) {
            r -= (double)dist2[i];
            if (r <= 0) {
                nxt = i;
                break;
            }
        }
        if (nxt == -1) nxt = N - 1;
        seeds.push_back(nxt);
    }
    return seeds;
}

int main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    int N, T, M, K;
    ll L;
    cin >> N >> T >> M >> K >> L;
    vector<pll> pos(N), vel(N);
    rep(i, N) cin >> pos[i].first >> pos[i].second >> vel[i].first >> vel[i].second;

    Timer timer;
    const double TIME_LIMIT = 1900.0; // ms
    mt19937 rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count());

    // initial seeds and plan
    auto seeds           = kpp_seeds(rng, N, M, pos, L);
    auto [best_cost, ans] = assign_with_flow(seeds, pos, L, M, K, N);

    vector<char> is_seed(N, 0);
    for (int s : seeds) is_seed[s] = 1;

    // simulated annealing on seed swaps
    double T0 = 1e6, T1 = 1e-3;
    int iter  = 0;
    while (true) {
        double elapsed = timer.ms();
        if (elapsed > TIME_LIMIT) break;
        double t = elapsed / TIME_LIMIT;
        double temp = T0 * pow(T1 / T0, t); // exponential cooling

        int si = rng() % M;
        int old_seed = seeds[si];
        int cand;
        do {
            cand = rng() % N;
        } while (is_seed[cand]);

        seeds[si]         = cand;
        is_seed[old_seed] = 0;
        is_seed[cand]     = 1;

        auto [cost, edges] = assign_with_flow(seeds, pos, L, M, K, N);
        ll delta           = cost - best_cost;
        bool accept        = false;
        if (cost < best_cost) {
            accept    = true;
            best_cost = cost;
            ans       = std::move(edges);
        } else if (cost < (ll)4e18) {
            double prob = exp(-delta / temp);
            uniform_real_distribution<double> ur(0.0, 1.0);
            if (ur(rng) < prob) accept = true;
        }

        if (!accept) {
            // revert
            is_seed[cand]     = 0;
            is_seed[old_seed] = 1;
            seeds[si]         = old_seed;
        }
        ++iter;
    }

    // output merges at t=0
    for (auto [u, v] : ans) cout << 0 << " " << u << " " << v << '\n';
}
