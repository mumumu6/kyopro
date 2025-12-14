#include <bits/stdc++.h>
using namespace std;

// -------------------- Simple non-negative BigInt (base 1e9) --------------------
struct BigInt {
    static constexpr uint32_t BASE = 1000000000u; // 1e9
    vector<uint32_t> d; // little-endian

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
        uint32_t x2 = (uint32_t)(x / (uint64_t(BASE) * uint64_t(BASE))); // up to ~1e18 -> <= 2 digits actually
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
        size_t i = 0;
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
                    if (d[k] > 0) { d[k]--; break; }
                    d[k] = BASE - 1;
                    k++;
                }
            }
            i++;
        }
        trim();
    }

    // this += other * mul (mul is small-ish), non-negative
    void add_mul_small(const BigInt& other, uint64_t mul) {
        if (mul == 0 || other.d.empty()) return;
        size_t n = other.d.size();
        if (d.size() < n) d.resize(n, 0);

        uint64_t carry = 0;
        for (size_t i = 0; i < n; ++i) {
            uint64_t cur = (uint64_t)d[i] + (uint64_t)other.d[i] * mul + carry;
            d[i] = (uint32_t)(cur % BASE);
            carry = cur / BASE;
        }
        size_t i = n;
        while (carry > 0) {
            if (i >= d.size()) d.push_back(0);
            uint64_t cur = (uint64_t)d[i] + carry;
            d[i] = (uint32_t)(cur % BASE);
            carry = cur / BASE;
            ++i;
        }
        // no need trim
    }
};

// -------------------- Heuristic utilities --------------------
struct Action { int i, j; };

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
static inline long double sum_B0(int R, long double b0, long double b1, long double b2,
                                 int p1, int p2, int p3) {
    const long double C2 = comb2(R);
    const long double C3 = comb3(R);
    const long double C4 = comb4(R);
    return (long double)R * b0
         + (long double)p1 * (b1 * C2 + (long double)p2 * (b2 * C3 + (long double)p3 * C4));
}

// estimated additional apples over remaining R turns by increasing P[i] by +1 now
// "virtual chain" trick: use max(p,1) for lower levels so P3/P2 doesn't become 0-value too easily.
static inline long double delta_gain(int i, int A,
                                     long double b0, long double b1, long double b2,
                                     int p0, int p1, int p2, int p3,
                                     int R) {
    const long double C2 = comb2(R);
    const long double C3 = comb3(R);
    const long double C4 = comb4(R);

    if (i == 0) {
        long double sb0 = sum_B0(R, b0, b1, b2, p1, p2, p3);
        return (long double)A * sb0;
    }
    if (i == 1) {
        int vp0 = max(p0, 1);
        long double term = b1 * C2 + (long double)p2 * (b2 * C3 + (long double)p3 * C4);
        return (long double)A * (long double)vp0 * term;
    }
    if (i == 2) {
        int vp0 = max(p0, 1);
        int vp1 = max(p1, 1);
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

                long double d = delta_gain(
                    i, A[j],
                    Bd[0][j], Bd[1][j], Bd[2][j],
                    P[0][j], P[1][j], P[2][j], P[3][j],
                    R
                );
                if (!isfinite(d)) d = 1e4900L;

                // penalty if chain below is missing (keeps P3 from being overvalued when totally disconnected)
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
                if (turn < 120 && i == 0 && P[1][j] == 0 && P[2][j] == 0 && P[3][j] == 0) {
                    d *= 0.75L;
                }

                long double net = d - cost;
                if (!(net > 0.0L) || !isfinite(net)) continue;

                long double denom = powl(cost, alpha);
                if (!(denom > 0.0L) || !isfinite(denom)) denom = 1.0L;

                long double score = net / denom;

                const long double EPS = 1e-15L;
                if (score > bestScore + EPS) {
                    bestScore = score;
                    best = {i, j};
                } else if (best.i != -1 && fabsl(score - bestScore) <= EPS) {
                    // deterministic tie-break
                    if (turn < 200) {
                        // early: prefer higher i to build chain
                        if (i > best.i || (i == best.i && j < best.j)) best = {i, j};
                    } else {
                        // late: prefer cheaper, then lexicographic
                        long double bestCost = (long double)C[best.i][best.j] * (long double)(P[best.i][best.j] + 1);
                        if (cost < bestCost - EPS ||
                            (fabsl(cost - bestCost) <= EPS && (i < best.i || (i == best.i && j < best.j)))) {
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

