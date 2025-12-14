#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using i128 = __int128_t;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, L, T, K;
    if (!(cin >> N >> L >> T >> K)) return 0;
    vector<ll> A(N);
    vector<vector<ll>> C(L, vector<ll>(N));
    for (ll &x : A) cin >> x;
    for (int i = 0; i < L; ++i) for (ll &x : C[i]) cin >> x;

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
            int best_id = -1;
            i128 best_cost = numeric_limits<i128>::max();
            for (int id : active_ids) {
                i128 cost = (i128)C[0][id] * (P[0][id] + 1);
                if (apples >= cost && cost < best_cost) {
                    best_cost = cost;
                    best_id = id;
                }
            }
            if (best_id == -1) {
                i128 best_any = numeric_limits<i128>::max();
                int best_lvl = -1, best_machine = -1;
                for (int lvl = 0; lvl < L; ++lvl) {
                    for (int id = 0; id < N; ++id) {
                        i128 cost = (i128)C[lvl][id] * (P[lvl][id] + 1);
                        if (apples >= cost && cost < best_any) {
                            best_any = cost;
                            best_lvl = lvl;
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

        if (!acted) {
            actions.emplace_back(-1, -1);
        }
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
