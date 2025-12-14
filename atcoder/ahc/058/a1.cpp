#include <bits/stdc++.h>
using namespace std;

#define main complex_main
// ==== begin: a.cpp ====
// (元ファイル先頭の #include と using は a1.cpp 側で済ませているので省略)

// -------------------- Simple non-negative BigInt (base 1e9) --------------------
struct BigInt {
    static constexpr uint32_t BASE = 1000000000u; // 1e9
    vector<uint32_t> d;                           // little-endian

    BigInt(uint64_t x = 0) { set_u64(x); }

    void trim() {
        while (!d.empty() && d.back() == 0) d.pop_back();
    }

    void set_u64(uint64_t x) {
        d.clear();
        if (x == 0) return;
        while (x) {
            d.push_back((uint32_t)(x % BASE));
            x /= BASE;
        }
    }

    bool is_zero() const { return d.empty(); }

    // compare with uint64
    int cmp_u64(uint64_t x) const {
        if (d.empty()) return (x == 0 ? 0 : -1);
        // convert x into base 1e9 digits
        uint32_t x0 = (uint32_t)(x % BASE);
        uint32_t x1 = (uint32_t)((x / BASE) % BASE);
        uint32_t x2 =
            (uint32_t)(x / (uint64_t(BASE) * uint64_t(BASE))); // up to ~1e18 -> <= 2 digits actually
        // Build vector for x
        vector<uint32_t> xd;
        if (x2) xd = {x0, x1, x2};
        else if (x1) xd = {x0, x1};
        else xd = {x0};
        while (!xd.empty() && xd.back() == 0) xd.pop_back();

        if (d.size() != xd.size()) return (d.size() < xd.size() ? -1 : 1);
        for (int i = (int)d.size() - 1; i >= 0; --i) {
            if (d[i] != xd[i]) return (d[i] < xd[i] ? -1 : 1);
        }
        return 0;
    }

    bool ge_u64(uint64_t x) const { return cmp_u64(x) >= 0; }

    // this -= x (uint64), assumes this >= x
    void sub_u64(uint64_t x) {
        uint64_t carry = x;
        size_t i       = 0;
        while (carry > 0) {
            uint64_t sub = carry % BASE;
            carry /= BASE;

            uint64_t cur = (uint64_t)(i < d.size() ? d[i] : 0);
            if (cur >= sub) {
                d[i] = (uint32_t)(cur - sub);
            } else {
                // borrow
                d[i] = (uint32_t)(cur + BASE - sub);
                // propagate borrow: subtract 1 from next digits
                size_t k = i + 1;
                while (true) {
                    if (k >= d.size()) {
                        // should not happen if this >= x
                        d.push_back(0);
                    }
                    if (d[k] > 0) {
                        d[k]--;
                        break;
                    }
                    d[k] = BASE - 1;
                    k++;
                }
            }
            i++;
        }
        trim();
    }

    // this += other * mul (mul is small-ish), non-negative
    void add_mul_small(const BigInt &other, uint64_t mul) {
        if (mul == 0 || other.d.empty()) return;
        size_t n = other.d.size();
        if (d.size() < n) d.resize(n, 0);

        uint64_t carry = 0;
        for (size_t i = 0; i < n; ++i) {
            uint64_t cur = (uint64_t)d[i] + (uint64_t)other.d[i] * mul + carry;
            d[i]         = (uint32_t)(cur % BASE);
            carry        = cur / BASE;
        }
        size_t i = n;
        while (carry > 0) {
            if (i >= d.size()) d.push_back(0);
            uint64_t cur = (uint64_t)d[i] + carry;
            d[i]         = (uint32_t)(cur % BASE);
            carry        = cur / BASE;
            ++i;
        }
        // no need trim
    }
};

// -------------------- Heuristic utilities --------------------
struct Action {
    int i, j;
};

static inline long double comb2(int R) {
    if (R < 2) return 0.0L;
    return (long double)R * (R - 1) / 2.0L;
}
static inline long double comb3(int R) {
    if (R < 3) return 0.0L;
    return (long double)R * (R - 1) * (R - 2) / 6.0L;
}
static inline long double comb4(int R) {
    if (R < 4) return 0.0L;
    return (long double)R * (R - 1) * (R - 2) * (R - 3) / 24.0L;
}

// sum of B0 over next R production steps assuming no more upgrades
static inline long double sum_B0(int R, long double b0, long double b1, long double b2, int p1, int p2,
                                 int p3) {
    const long double C2 = comb2(R);
    const long double C3 = comb3(R);
    const long double C4 = comb4(R);
    return (long double)R * b0 +
           (long double)p1 * (b1 * C2 + (long double)p2 * (b2 * C3 + (long double)p3 * C4));
}

// estimated additional apples over remaining R turns by increasing P[i] by +1 now
// "virtual chain" trick: use max(p,1) for lower levels so P3/P2 doesn't become 0-value too easily.
static inline long double delta_gain(int i, int A, long double b0, long double b1, long double b2,
                                     int p0, int p1, int p2, int p3, int R) {
    const long double C2 = comb2(R);
    const long double C3 = comb3(R);
    const long double C4 = comb4(R);

    if (i == 0) {
        long double sb0 = sum_B0(R, b0, b1, b2, p1, p2, p3);
        return (long double)A * sb0;
    }
    if (i == 1) {
        int vp0          = max(p0, 1);
        long double term = b1 * C2 + (long double)p2 * (b2 * C3 + (long double)p3 * C4);
        return (long double)A * (long double)vp0 * term;
    }
    if (i == 2) {
        int vp0          = max(p0, 1);
        int vp1          = max(p1, 1);
        long double term = b2 * C3 + (long double)p3 * C4;
        return (long double)A * (long double)vp0 * (long double)vp1 * term;
    }
    // i == 3
    int vp0 = max(p0, 1);
    int vp1 = max(p1, 1);
    int vp2 = max(p2, 1);
    return (long double)A * (long double)vp0 * (long double)vp1 * (long double)vp2 * C4;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, L, T;
    long long K;
    cin >> N >> L >> T >> K;

    vector<int> A(N);
    for (int j = 0; j < N; ++j) cin >> A[j];

    vector<vector<unsigned long long>> C(L, vector<unsigned long long>(N));
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < N; ++j) {
            unsigned long long x;
            cin >> x;
            C[i][j] = x;
        }
    }

    // Exact state (BigInt)
    vector<vector<BigInt>> B(L, vector<BigInt>(N, BigInt(1)));
    vector<vector<int>> P(L, vector<int>(N, 0));
    BigInt apples((uint64_t)K);

    // Floating state (heuristics)
    vector<vector<long double>> Bd(L, vector<long double>(N, 1.0L));
    long double applesd = (long double)K;

    vector<Action> actions;
    actions.reserve(T);

    // choose a few focus IDs to reduce "spreading"
    vector<int> ids(N);
    iota(ids.begin(), ids.end(), 0);
    sort(ids.begin(), ids.end(), [&](int x, int y) {
        long double rx = (long double)C[1][x] / (long double)A[x];
        long double ry = (long double)C[1][y] / (long double)A[y];
        if (fabsl(rx - ry) > 1e-18L) return rx < ry;
        return x < y;
    });

    vector<int> focus;
    focus.push_back(0);
    focus.push_back(N - 1);
    for (int k2 = 0; k2 < min(N, 3); ++k2) focus.push_back(ids[k2]);
    sort(focus.begin(), focus.end());
    focus.erase(unique(focus.begin(), focus.end()), focus.end());
    vector<char> is_focus(N, 0);
    for (int j : focus) is_focus[j] = 1;

    for (int turn = 0; turn < T; ++turn) {
        int R = T - turn; // remaining production steps including this turn

        // schedule parameter: early cost-sensitive, late gain-sensitive
        long double phase = (long double)turn / (long double)max(1, T - 1);
        long double alpha = 0.60L * (1.0L - phase); // 0.60 -> 0.0

        Action best{-1, -1};
        long double bestScore = 0.0L;

        for (int i = 0; i < L; ++i) {
            for (int j = 0; j < N; ++j) {
                unsigned long long cost_u64 = C[i][j] * (unsigned long long)(P[i][j] + 1);
                if (!apples.ge_u64(cost_u64)) continue;

                long double cost = (long double)C[i][j] * (long double)(P[i][j] + 1);
                if (!(cost > 0.0L) || !isfinite(cost)) continue;

                long double d = delta_gain(i, A[j], Bd[0][j], Bd[1][j], Bd[2][j], P[0][j], P[1][j],
                                           P[2][j], P[3][j], R);
                if (!isfinite(d)) d = 1e4900L;

                // penalty if chain below is missing (keeps P3 from being overvalued when totally
                // disconnected)
                int missing = 0;
                for (int lv = 0; lv < i; ++lv) missing += (P[lv][j] == 0);
                long double base = 1.0L;
                if (i == 1) base = 0.55L;
                if (i == 2) base = 0.35L;
                if (i == 3) base = 0.25L;
                d *= powl(base, (long double)missing);

                // bonus for first activation of that level
                if (P[i][j] == 0) d *= (turn < 220 ? 1.35L : 1.10L);

                // early: focus IDs a bit
                if (turn < 180 && is_focus[j]) d *= 1.10L;

                // early: discourage pure Lv0 spam on ids with no higher growth yet
                if (turn < 120 && i == 0 && P[1][j] == 0 && P[2][j] == 0 && P[3][j] == 0) { d *= 0.75L; }

                long double net = d - cost;
                if (!(net > 0.0L) || !isfinite(net)) continue;

                long double denom = powl(cost, alpha);
                if (!(denom > 0.0L) || !isfinite(denom)) denom = 1.0L;

                long double score = net / denom;

                const long double EPS = 1e-15L;
                if (score > bestScore + EPS) {
                    bestScore = score;
                    best      = {i, j};
                } else if (best.i != -1 && fabsl(score - bestScore) <= EPS) {
                    // deterministic tie-break
                    if (turn < 200) {
                        // early: prefer higher i to build chain
                        if (i > best.i || (i == best.i && j < best.j)) best = {i, j};
                    } else {
                        // late: prefer cheaper, then lexicographic
                        long double bestCost =
                            (long double)C[best.i][best.j] * (long double)(P[best.i][best.j] + 1);
                        if (cost < bestCost - EPS || (fabsl(cost - bestCost) <= EPS &&
                                                      (i < best.i || (i == best.i && j < best.j)))) {
                            best = {i, j};
                        }
                    }
                }
            }
        }

        // Apply action
        if (best.i == -1) {
            actions.push_back({-1, -1});
        } else {
            int i = best.i, j = best.j;
            unsigned long long cost_u64 = C[i][j] * (unsigned long long)(P[i][j] + 1);
            if (apples.ge_u64(cost_u64)) {
                apples.sub_u64(cost_u64);
                applesd -= (long double)cost_u64;
                if (applesd < 0) applesd = 0;
                P[i][j] += 1;
                actions.push_back(best);
            } else {
                actions.push_back({-1, -1}); // safety
            }
        }

        // Production step (Level 0 -> 3)
        for (int j = 0; j < N; ++j) {
            if (P[0][j] != 0) {
                uint64_t mul = (uint64_t)P[0][j] * (uint64_t)A[j]; // <= ~50000
                apples.add_mul_small(B[0][j], mul);
                applesd += Bd[0][j] * (long double)P[0][j] * (long double)A[j];
            }
        }
        for (int lv = 1; lv < L; ++lv) {
            for (int j = 0; j < N; ++j) {
                if (P[lv][j] != 0) {
                    uint64_t mul = (uint64_t)P[lv][j]; // <= ~500
                    B[lv - 1][j].add_mul_small(B[lv][j], mul);
                    Bd[lv - 1][j] += Bd[lv][j] * (long double)P[lv][j];
                }
            }
        }

        // keep applesd finite-ish (heuristic only)
        if (!isfinite(applesd)) applesd = 1e4900L;
        if (applesd > 1e4900L) applesd = 1e4900L;
    }

    for (auto &ac : actions) {
        if (ac.i == -1) cout << -1 << '\n';
        else cout << ac.i << ' ' << ac.j << '\n';
    }
    return 0;
}

// ==== end: a.cpp ====
#undef main

#define main simple_main
// ==== begin: a_simple.cpp ====
// (元ファイル先頭の #include と using は a1.cpp 側で済ませているので省略)

using ll   = long long;
using i128 = __int128_t;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, L, T, K;
    if (!(cin >> N >> L >> T >> K)) return 0;
    vector<ll> A(N);
    vector<vector<ll>> C(L, vector<ll>(N));
    for (ll &x : A) cin >> x;
    for (int i = 0; i < L; ++i)
        for (ll &x : C[i]) cin >> x;

    vector<vector<ll>> P(L, vector<ll>(N, 0));
    vector<vector<i128>> B(L, vector<i128>(N, 1));
    i128 apples = K;

    vector<int> upgrade_cnt(L, 0);

    vector<int> ids(N);
    iota(ids.begin(), ids.end(), 0);
    vector<int> sorted = ids;
    sort(sorted.begin(), sorted.end(), [&](int x, int y) {
        long double rx = (long double)A[x] / (long double)C[0][x];
        long double ry = (long double)A[y] / (long double)C[0][y];
        if (fabsl(rx - ry) > 1e-12L) return rx > ry;
        return x < y;
    });
    vector<int> active_ids;
    active_ids.push_back(0);
    for (int id : sorted) {
        if (id == 0) continue;
        active_ids.push_back(id);
    }
    int keep = min<int>(active_ids.size(), 4);
    if (keep == 0) keep = 1;
    active_ids.resize(keep);

    auto can_pay = [&](int lvl, int id) -> bool {
        i128 cost = (i128)C[lvl][id] * (P[lvl][id] + 1);
        return apples >= cost;
    };

    auto pay = [&](int lvl, int id) {
        i128 cost = (i128)C[lvl][id] * (P[lvl][id] + 1);
        apples -= cost;
        P[lvl][id] += 1;
        if (lvl >= 1) upgrade_cnt[lvl]++;
    };

    auto produce = [&]() {
        for (int j = 0; j < N; ++j) {
            if (P[0][j] == 0) continue;
            apples += B[0][j] * (i128)P[0][j] * (i128)A[j];
        }
        for (int lv = 1; lv < L; ++lv) {
            for (int j = 0; j < N; ++j) {
                if (P[lv][j] == 0) continue;
                B[lv - 1][j] += B[lv][j] * (i128)P[lv][j];
            }
        }
    };

    auto best_level = [&]() -> int {
        if (L <= 1) return 0;
        int best = 1;
        for (int lv = 2; lv < L; ++lv) {
            if (upgrade_cnt[lv] < upgrade_cnt[best]) best = lv;
        }
        return best;
    };

    vector<pair<int, int>> actions;
    actions.reserve(T);

    for (int turn = 0; turn < T; ++turn) {
        bool acted = false;
        if (L >= 2) {
            int target_lvl = best_level();
            for (int id : active_ids) {
                for (int shift = 0; shift < L - 1; ++shift) {
                    int cur_lvl = 1 + ((target_lvl - 1 + shift) % (L - 1));
                    if (can_pay(cur_lvl, id)) {
                        pay(cur_lvl, id);
                        actions.emplace_back(cur_lvl, id);
                        acted = true;
                        break;
                    }
                }
                if (acted) break;
            }
        }

        if (!acted) {
            int best_id    = -1;
            i128 best_cost = numeric_limits<i128>::max();
            for (int id : active_ids) {
                i128 cost = (i128)C[0][id] * (P[0][id] + 1);
                if (apples >= cost && cost < best_cost) {
                    best_cost = cost;
                    best_id   = id;
                }
            }
            if (best_id == -1) {
                i128 best_any = numeric_limits<i128>::max();
                int best_lvl = -1, best_machine = -1;
                for (int lvl = 0; lvl < L; ++lvl) {
                    for (int id = 0; id < N; ++id) {
                        i128 cost = (i128)C[lvl][id] * (P[lvl][id] + 1);
                        if (apples >= cost && cost < best_any) {
                            best_any     = cost;
                            best_lvl     = lvl;
                            best_machine = id;
                        }
                    }
                }
                if (best_lvl != -1) {
                    pay(best_lvl, best_machine);
                    actions.emplace_back(best_lvl, best_machine);
                    acted = true;
                }
            } else {
                pay(0, best_id);
                actions.emplace_back(0, best_id);
                acted = true;
            }
        }

        if (!acted) { actions.emplace_back(-1, -1); }
        produce();
    }

    for (auto [i, j] : actions) {
        if (i < 0) {
            cout << -1 << '\n';
        } else {
            cout << i << ' ' << j << '\n';
        }
    }
    return 0;
}

// ==== end: a_simple.cpp ====
#undef main

struct InputData {
    int N       = 0;
    int L       = 0;
    int T       = 0;
    long long K = 0;
    vector<long long> A;
    vector<vector<long long>> C;
    bool valid = false;
};

namespace {

string trim(const string &s) {
    size_t l = 0;
    while (l < s.size() && isspace(static_cast<unsigned char>(s[l]))) l++;
    size_t r = s.size();
    while (r > l && isspace(static_cast<unsigned char>(s[r - 1]))) r--;
    return s.substr(l, r - l);
}

int cmp_bigint(const BigInt &a, const BigInt &b) {
    if (a.d.size() != b.d.size()) { return (a.d.size() < b.d.size()) ? -1 : 1; }
    for (int i = static_cast<int>(a.d.size()) - 1; i >= 0; --i) {
        if (a.d[i] != b.d[i]) { return (a.d[i] < b.d[i]) ? -1 : 1; }
    }
    return 0;
}

InputData parse_input(const string &text) {
    InputData res;
    istringstream iss(text);
    if (!(iss >> res.N >> res.L >> res.T >> res.K)) { return res; }
    res.A.assign(res.N, 0);
    for (int i = 0; i < res.N; ++i) {
        if (!(iss >> res.A[i])) return res;
    }
    res.C.assign(res.L, vector<long long>(res.N, 0));
    for (int lv = 0; lv < res.L; ++lv) {
        for (int j = 0; j < res.N; ++j) {
            if (!(iss >> res.C[lv][j])) return res;
        }
    }
    res.valid = true;
    return res;
}

struct SimulationResult {
    bool ok = false;
    BigInt apples;
    string error;
};

SimulationResult simulate(const InputData &input, const vector<pair<int, int>> &actions) {
    SimulationResult res;
    if (!input.valid) {
        res.error = "input parse failed";
        return res;
    }
    if (static_cast<int>(actions.size()) != input.T) {
        res.error = "action length mismatch";
        return res;
    }
    vector<vector<BigInt>> B(input.L, vector<BigInt>(input.N, BigInt(1)));
    vector<vector<int>> P(input.L, vector<int>(input.N, 0));
    BigInt apples(input.K);

    for (int turn = 0; turn < input.T; ++turn) {
        int i = actions[turn].first;
        int j = actions[turn].second;
        if (i >= 0) {
            if (!(0 <= i && i < input.L && 0 <= j && j < input.N)) {
                res.error = "invalid machine index";
                return res;
            }
            unsigned long long cost = static_cast<unsigned long long>(input.C[i][j]) *
                                      static_cast<unsigned long long>(P[i][j] + 1);
            if (!apples.ge_u64(cost)) {
                res.error = "not enough apples";
                return res;
            }
            apples.sub_u64(cost);
            P[i][j] += 1;
        } else {
            if (j != -1) {
                res.error = "malformed action";
                return res;
            }
        }

        for (int id = 0; id < input.N; ++id) {
            int p = P[0][id];
            if (p == 0) continue;
            unsigned long long mul =
                static_cast<unsigned long long>(input.A[id]) * static_cast<unsigned long long>(p);
            apples.add_mul_small(B[0][id], mul);
        }
        for (int lv = 1; lv < input.L; ++lv) {
            for (int id = 0; id < input.N; ++id) {
                int p = P[lv][id];
                if (p == 0) continue;
                B[lv - 1][id].add_mul_small(B[lv][id], static_cast<unsigned long long>(p));
            }
        }
    }

    res.ok     = true;
    res.apples = apples;
    return res;
}

struct ParseResult {
    bool ok = false;
    vector<pair<int, int>> actions;
    string sanitized;
    string error;
};

ParseResult parse_actions(const string &raw_output, int T) {
    ParseResult res;
    vector<string> sanitized_lines;
    istringstream iss(raw_output);
    string line;
    bool too_many = false;
    while (getline(iss, line)) {
        string trimmed  = trim(line);
        size_t hash_pos = trimmed.find('#');
        if (hash_pos != string::npos) { trimmed = trim(trimmed.substr(0, hash_pos)); }
        if (trimmed.empty()) continue;
        if (res.actions.size() >= static_cast<size_t>(T)) {
            too_many = true;
            break;
        }
        stringstream ls(trimmed);
        int a = 0, b = 0;
        if (!(ls >> a)) {
            res.error = "failed to parse action";
            return res;
        }
        if (a == -1) {
            res.actions.emplace_back(-1, -1);
            sanitized_lines.emplace_back("-1");
            continue;
        }
        if (!(ls >> b)) {
            res.error = "failed to parse action";
            return res;
        }
        res.actions.emplace_back(a, b);
        sanitized_lines.emplace_back(to_string(a) + " " + to_string(b));
    }
    if (too_many) {
        res.error = "too many actions";
        return res;
    }
    if (static_cast<int>(res.actions.size()) != T) {
        res.error = "insufficient actions";
        return res;
    }
    res.ok = true;
    string out;
    out.reserve(T * 6);
    for (size_t idx = 0; idx < sanitized_lines.size(); ++idx) {
        out.append(sanitized_lines[idx]);
        out.push_back('\n');
    }
    res.sanitized = move(out);
    return res;
}

struct StrategyResult {
    bool ok = false;
    BigInt apples;
    string sanitized_output;
    string raw_output;
    string error;
};

StrategyResult run_strategy(int (*strategy_main)(), const string &input_text, const InputData &input) {
    StrategyResult res;
    istringstream fake_in(input_text);
    ostringstream fake_out;
    auto *orig_in  = cin.rdbuf(fake_in.rdbuf());
    auto *orig_out = cout.rdbuf(fake_out.rdbuf());
    strategy_main();
    cout.flush();
    res.raw_output = fake_out.str();
    cin.rdbuf(orig_in);
    cout.rdbuf(orig_out);

    ParseResult parsed = parse_actions(res.raw_output, input.T);
    if (!parsed.ok) {
        res.error = parsed.error;
        return res;
    }
    SimulationResult sim = simulate(input, parsed.actions);
    if (!sim.ok) {
        res.error = sim.error;
        return res;
    }
    res.ok               = true;
    res.apples           = sim.apples;
    res.sanitized_output = parsed.sanitized;
    return res;
}

} // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string input_data((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());
    if (input_data.empty()) { return 0; }
    InputData input = parse_input(input_data);
    if (!input.valid) {
        // fall back to complex_main output
        istringstream fake_in(input_data);
        ostringstream fake_out;
        auto *orig_in  = cin.rdbuf(fake_in.rdbuf());
        auto *orig_out = cout.rdbuf(fake_out.rdbuf());
        complex_main();
        cout.flush();
        string raw = fake_out.str();
        cin.rdbuf(orig_in);
        cout.rdbuf(orig_out);
        cout << raw;
        if (!raw.empty() && raw.back() != '\n') cout << '\n';
        return 0;
    }

    StrategyResult complex_res = run_strategy(complex_main, input_data, input);
    StrategyResult simple_res  = run_strategy(simple_main, input_data, input);

    int best_idx = -1;
    BigInt best_value;
    if (complex_res.ok) {
        best_idx   = 0;
        best_value = complex_res.apples;
    }
    if (simple_res.ok) {
        if (best_idx == -1 || cmp_bigint(simple_res.apples, best_value) > 0) {
            best_idx   = 1;
            best_value = simple_res.apples;
        }
    }

    if (best_idx == 0) {
        cout << complex_res.sanitized_output;
        return 0;
    }
    if (best_idx == 1) {
        cout << simple_res.sanitized_output;
        return 0;
    }

    string fallback = complex_res.raw_output.empty() ? simple_res.raw_output : complex_res.raw_output;
    if (!fallback.empty()) {
        cout << fallback;
        if (fallback.back() != '\n') cout << '\n';
    } else {
        for (int t = 0; t < input.T; ++t) cout << "-1\n";
    }
    return 0;
}
