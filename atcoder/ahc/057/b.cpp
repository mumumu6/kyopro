#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
// using mint = modint998244353;
using ll     = long long;
using ld     = long double;
using i128   = __int128_t;
const ll INF = 4e18;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
#define rrep(i, n) for (ll i = (n) - 1; i >= 0; i--)
#define rreps(i, a, b) for (ll i = (b) - 1; i >= (a); i--)
bool chmin(auto &a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto &a, auto b) { return a < b ? a = b, 1 : 0; }
vector<ll> sin45 = {0, 1, 1, 1, 0, -1, -1, -1}, cos45 = {1, 1, 0, -1, -1, -1, 0, 1};
vector<ll> dx = {0, 1, 0, -1};
vector<ll> dy = {1, 0, -1, 0};
int popcnt(ll x) { return __builtin_popcountll(x); }
int popcnt_mod_2(ll x) { return __builtin_parityll(x); }
int topbit(ll x) { return x == 0 ? -1 : 63 - __builtin_clzll(x); }
int lowbit(ll x) { return x == 0 ? -1 : __builtin_ctzll(x); }
#define x first
#define y second
#define all(x) std::begin(x), std::end(x)
#define pii pair<int, int>
#define pll pair<ll, ll>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>
#define Yes cout << "Yes" << el
#define No cout << "No" << el
#define spa " "
#define el '\n'

static inline bool inside(int x, int y, int w, int h) { return 0 <= x && x < w && 0 <= y && y < h; }

// ----------------- オーバーロード -----------------
template <class T, class U> ostream &operator<<(ostream &os, const pair<T, U> &p) {
    return os << '(' << p.first << ", " << p.second << ')';
}

// template <int MOD> ostream &operator<<(std::ostream &os, const atcoder::static_modint<MOD> &m) {
//    return os << m.val();
// }

template <class T> ostream &operator<<(ostream &os, const vector<T> &v) {
    os << '[';
    for (ll i = 0; i < v.size(); ++i) {
        if (i) os << ", ";
        os << v[i];
    }
    return os << ']';
}
template <class T> ostream &operator<<(ostream &os, const set<T> &s) {
    os << '{';
    for (auto it = s.begin(); it != s.end(); ++it) {
        if (it != s.begin()) os << ", ";
        os << *it;
    }
    return os << '}';
}

template <class K, class V, class Comp, class Alloc>
ostream &operator<<(ostream &os, const map<K, V, Comp, Alloc> &m) {
    os << '{';
    for (auto &[k, v] : m) {
        if (k != m.begin()->first) os << ", ";
        os << k << ": " << v;
    }
    return os << '}';
}

// ----------------- デバッグ用 -----------------
namespace dbg {
template <typename... Ts> void impl(const char *names, Ts &&...xs) {
    string_view sv{names};
    array<string_view, sizeof...(Ts)> labels{};
    ll pos = 0, idx = 0;
    for (ll i = 0; i <= sv.size(); ++i) {
        if (i == sv.size() || sv[i] == ',') {
            ll start = pos;
            while (start < i && sv[start] == ' ') ++start; // 前方空白除去
            labels[idx++] = sv.substr(start, i - start);
            pos           = i + 1;
        }
    }
    ll id    = 0;
    auto out = [&](auto &&v) {
        cerr << labels[id++] << " = " << v;
        if (id < sizeof...(Ts)) cerr << ", ";
    };
    (out(std::forward<Ts>(xs)), ...);
    cerr << '\n';
}
} // namespace dbg

#define debug(...) dbg::impl(#__VA_ARGS__, __VA_ARGS__)

template <class T> using P = pair<T, T>;
#define x first
#define y second
template <class T> P<T> operator+(const P<T> &a, const P<T> &b) { return {a.x + b.x, a.y + b.y}; }
template <class T> P<T> operator-(const P<T> &a, const P<T> &b) { return {a.x - b.x, a.y - b.y}; }
template <class T> P<T> operator-(const P<T> &a) { return {-a.x, -a.y}; }
template <class T, class U> P<T> operator*(const P<T> &a, const U &b) { return {a.x * b, a.y * b}; }
template <class T, class U> P<T> operator/(const P<T> &a, const U &b) { return {a.x / b, a.y / b}; }
template <class T> P<T> &operator+=(P<T> &a, const P<T> &b) { return a = a + b; }
template <class T> P<T> &operator-=(P<T> &a, const P<T> &b) { return a = a - b; }
template <class T, class U> P<T> &operator*=(P<T> &a, const U &b) { return a = a * b; }
template <class T, class U> P<T> &operator/=(P<T> &a, const U &b) { return a = a / b; }
template <class T> P<T> rotate(const P<T> &a) { return {-a.y, a.x}; } // 90 degree ccw
template <class T> T dot(const P<T> &a, const P<T> &b) { return a.x * b.x + a.y * b.y; }
template <class T> T cross(const P<T> &a, const P<T> &b) { return dot(rotate(a), b); }
template <class T> T square(const P<T> &a) { return dot(a, a); }
template <class T> ld abs(const P<T> &a) { return hypotl(a.x, a.y); }
template <class T> T gcd(const P<T> &a) { return gcd(a.x, a.y); }
template <class T> P<T> normalize(P<T> a) {
    if (a == P<T>{}) return a;
    a /= gcd(a);
    if (a < P<T>{}) a = -a;
    return a;
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, t, m, k, l;
    cin >> n >> t >> m >> k >> l;

    // トーラス距離（double版）
    auto torus_dist2_d = [&](pair<double, double> a, pair<double, double> b) {
        double dx = std::abs(a.first - b.first);
        dx        = min(dx, (double)l - dx);
        double dy = std::abs(a.second - b.second);
        dy        = min(dy, (double)l - dy);
        return dx * dx + dy * dy;
    };

    // 位置と速度（double で管理）
    vector<pair<double, double>> pos(n), vel(n);
    rep(i, n) {
        ll px, py, vx, vy;
        cin >> px >> py >> vx >> vy;
        pos[i] = {(double)px, (double)py};
        vel[i] = {(double)vx, (double)vy};
    }

    dsu uf(n);
    vector<tuple<int, int, int>> ans; // (time, i, j)
    ans.reserve(n - m);

    const int STEP                 = 30;    // 30ステップごとに結合チェック
    const double THRESHOLD_NEAR    = 10000; // 少なくとも1点は結合する大きめ閾値
    const double THRESHOLD_PACK    = 3000;  // この距離以内はまとめて結40
    const double THRESHOLD_NEAR_SQ = THRESHOLD_NEAR * THRESHOLD_NEAR;
    const double THRESHOLD_PACK_SQ = THRESHOLD_PACK * THRESHOLD_PACK;

    auto seed_distance = [&](int a, int b) { return torus_dist2_d(pos[a], pos[b]); };

    auto build_seeds = [&](int first_idx) {
        vector<int> seeds_local;
        vector<bool> used(n, false);
        seeds_local.reserve(m);
        seeds_local.push_back(first_idx);
        used[first_idx] = true;

        while ((int)seeds_local.size() < m) {
            double best_d = -1;
            int best_i    = -1;
            rep(i, n) {
                if (used[i]) continue;
                double mind = 1e18;
                for (int s : seeds_local) { chmin(mind, seed_distance(i, s)); }
                if (mind > best_d) {
                    best_d = mind;
                    best_i = i;
                }
            }
            if (best_i == -1) break;
            seeds_local.push_back(best_i);
            used[best_i] = true;
        }
        return seeds_local;
    };

    auto score_seeds = [&](const vector<int> &seeds_local) {
        if ((int)seeds_local.size() < m) return -1.0;
        double score = 0;
        rep(i, n) {
            double mind = 1e18;
            for (int s : seeds_local) { chmin(mind, seed_distance(i, s)); }
            score += mind;
        }
        return score;
    };

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(0, n - 1);
    auto global_start             = chrono::steady_clock::now();
    const double SEED_SEARCH_TIME = 1.2; // seconds

    vector<int> best_seeds = build_seeds(0);
    double best_score      = score_seeds(best_seeds);

    while (chrono::duration<double>(chrono::steady_clock::now() - global_start).count() <
           SEED_SEARCH_TIME) {
        int start_idx         = dist(rng);
        vector<int> candidate = build_seeds(start_idx);
        double sc             = score_seeds(candidate);
        if (sc > best_score) {
            best_score = sc;
            best_seeds = candidate;
        }
    }

    if ((int)best_seeds.size() < m) { best_seeds = build_seeds(0); }

    vector<int> seeds = best_seeds;
    vector<bool> is_seed(n, false);
    for (int s : seeds) { is_seed[s] = true; }

    // dt ステップ後の予測位置を計算（実際には進めない）
    auto predict_pos = [&](int i, int dt) {
        int li    = uf.leader(i);
        double px = fmod(pos[i].first + vel[li].first * dt + l * 1000, (double)l);
        double py = fmod(pos[i].second + vel[li].second * dt + l * 1000, (double)l);
        return make_pair(px, py);
    };

    // 位置を 1 ステップ進める
    auto advance_one = [&]() {
        rep(i, n) {
            int li        = uf.leader(i);
            pos[i].first  = fmod(pos[i].first + vel[li].first + l * 1000, (double)l);
            pos[i].second = fmod(pos[i].second + vel[li].second + l * 1000, (double)l);
        }
    };

    auto advance_steps = [&](int dt) {
        rep(_, dt) { advance_one(); }
    };

    // i step目に各seedに必要な目標サイズを計算
    // T=1000, K=30 なので、均等に増やす
    auto target_size_at = [&](int step) -> int {
        // step 0-32: 1, step 33-65: 2, ..., step 967-999: 30
        int target = step / (t / k) + 1;
        return min(target, (int)k);
    };

    // 連結成分の速度を更新（結合時に呼ぶ）
    // from_pt: seed集合内の点, to_pt: 結合対象の点
    auto merge_components = [&](int from_pt, int to_pt, int time) {
        if (uf.same(from_pt, to_pt)) return false;

        int li = uf.leader(from_pt), lj = uf.leader(to_pt);
        int si = uf.size(li), sj = uf.size(lj);

        // 結合後サイズがkを超えるなら結合しない
        if (si + sj > k) return false;

        // 新しい速度（運動量保存）
        double new_vx = (vel[li].first * si + vel[lj].first * sj) / (si + sj);
        double new_vy = (vel[li].second * si + vel[lj].second * sj) / (si + sj);

        uf.merge(from_pt, to_pt);
        int new_leader = uf.leader(from_pt);

        vel[new_leader] = {new_vx, new_vy};

        ans.push_back({time, from_pt, to_pt});
        return true;
    };

    // seed集合のメンバーを取得
    auto get_seed_members = [&](int seed_idx) {
        vector<int> members;
        int seed_leader = uf.leader(seeds[seed_idx]);
        rep(i, n) {
            if (uf.leader(i) == seed_leader) { members.push_back(i); }
        }
        return members;
    };

    // 30ステップごとに処理
    struct Candidate {
        double d2;
        int seed_idx;
        int from;
        int target;
    };

    for (int time = 0; time < t && (int)ans.size() < n - m; time += STEP) {
        int merge_time = min(time + STEP, (int)t - 1);
        int dt         = merge_time - time;
        int target_sz  = target_size_at(merge_time);

        vector<pair<double, double>> pred_pos(n);
        rep(i, n) { pred_pos[i] = predict_pos(i, dt); }

        vector<Candidate> merge_candidates;

        for (int si = 0; si < m; si++) {
            int seed        = seeds[si];
            int seed_leader = uf.leader(seed);
            int cur_sz      = uf.size(seed_leader);
            if (cur_sz >= k) continue;

            vector<int> members = get_seed_members(si);
            vector<Candidate> seed_cands;

            rep(j, n) {
                if (uf.same(seed, j)) continue;
                if (is_seed[j]) continue;
                if (uf.size(uf.leader(j)) > 1) continue; // 単独点のみ

                double best_d2 = 1e18;
                int best_from  = -1;
                for (int mem : members) {
                    double d2 = torus_dist2_d(pred_pos[mem], pred_pos[j]);
                    if (d2 < best_d2) {
                        best_d2   = d2;
                        best_from = mem;
                    }
                }
                if (best_from == -1) continue;
                Candidate cand;
                cand.d2       = best_d2;
                cand.seed_idx = si;
                cand.from     = best_from;
                cand.target   = (int)j;
                seed_cands.push_back(cand);
            }

            sort(seed_cands.begin(), seed_cands.end(),
                 [](const Candidate &a, const Candidate &b) { return a.d2 < b.d2; });

            int need  = max(0, target_sz - cur_sz);
            int added = 0;
            for (auto &cand : seed_cands) {
                if (cur_sz + added >= k) break;
                if (added < need || cand.d2 <= THRESHOLD_NEAR_SQ || cand.d2 <= THRESHOLD_PACK_SQ) {
                    merge_candidates.push_back(cand);
                    added++;
                } else {
                    break;
                }
            }
        }

        sort(merge_candidates.begin(), merge_candidates.end(),
             [](const Candidate &a, const Candidate &b) { return a.d2 < b.d2; });

        vector<int> seed_added(m, 0);
        vector<bool> target_used(n, false);
        vector<bool> loose_attached(m, false);
        vector<pair<int, int>> pending; // (seed_idx, target)

        for (auto &cand : merge_candidates) {
            if (target_used[cand.target]) continue;

            int seed = seeds[cand.seed_idx];
            int cur_sz =
                uf.size(uf.leader(seed)) + seed_added[cand.seed_idx]; // まだ結合していない分を加味
            if (cur_sz >= k) continue;

            bool force      = (cur_sz < target_sz);
            bool very_close = cand.d2 <= THRESHOLD_PACK_SQ;
            bool near       = cand.d2 <= THRESHOLD_NEAR_SQ;

            bool allow = false;
            if (force || very_close) {
                allow = true;
            } else if (near && !loose_attached[cand.seed_idx]) {
                allow = true;
            }
            if (!allow) continue;

            target_used[cand.target] = true;
            seed_added[cand.seed_idx]++;
            if (near) { loose_attached[cand.seed_idx] = true; }
            pending.push_back({cand.seed_idx, cand.target});
        }

        if (dt > 0) { advance_steps(dt); }

        for (auto &[si, target] : pending) {
            vector<int> members = get_seed_members(si);
            if (members.empty()) continue;

            int best_from  = members[0];
            double best_d2 = torus_dist2_d(pos[best_from], pos[target]);
            for (int mem : members) {
                double d2 = torus_dist2_d(pos[mem], pos[target]);
                if (d2 < best_d2) {
                    best_d2   = d2;
                    best_from = mem;
                }
            }

            if (best_d2 <= THRESHOLD_NEAR_SQ) { merge_components(best_from, target, merge_time); }
        }
    }

    // まだ足りなければ貪欲に結合（閾値なしで最近傍を取る）
    int final_time = (int)t - 1;
    while ((int)ans.size() < n - m) {
        bool merged_any = false;

        for (int si = 0; si < m; si++) {
            int seed = seeds[si];
            if (uf.size(uf.leader(seed)) >= k) continue;

            vector<int> members = get_seed_members(si);

            double best_d2  = 1e18;
            int best_target = -1;
            int best_from   = -1;

            rep(j, n) {
                if (uf.same(seed, j)) continue;
                if (is_seed[j]) continue;
                if (uf.size(uf.leader(j)) > 1) continue;

                for (int mem : members) {
                    double d2 = torus_dist2_d(pos[mem], pos[j]);
                    if (d2 < best_d2) {
                        best_d2     = d2;
                        best_target = j;
                        best_from   = mem;
                    }
                }
            }

            if (best_target != -1) {
                merge_components(best_from, best_target, final_time);
                merged_any = true;
            }
        }

        if (!merged_any) break;
    }

    // 出力
    for (auto [time, i, j] : ans) { cout << time << " " << i << " " << j << el; }
}
