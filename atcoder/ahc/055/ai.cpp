#include <bits/stdc++.h>
using namespace std;

struct AttackPlan {
    long long attacks = 0;
    vector<int> use_count;
};

static constexpr int INF_DURABILITY = 1'000'000'000;

AttackPlan build_attack_plan(int target, const vector<int> &remain, const vector<int> &durability,
                             const vector<char> &available, const vector<vector<int>> &damage,
                             const vector<vector<int>> &weapon_order, int bare_idx) {
    AttackPlan plan;
    if (target < 0 || target >= (int)remain.size()) return plan;
    int hp = remain[target];
    if (hp <= 0) return plan;

    const int weapon_count = durability.size();
    plan.use_count.assign(weapon_count, 0);

    long long attack_total = 0;
    for (int weapon : weapon_order[target]) {
        if (hp <= 0) break;
        if (weapon == bare_idx) {
            plan.use_count[bare_idx] += hp;
            attack_total += hp;
            hp = 0;
            break;
        }
        if (!available[weapon]) continue;
        int dmg = damage[weapon][target];
        if (dmg <= 0) continue;
        int can_use = durability[weapon];
        if (can_use <= 0) continue;
        int need = (hp + dmg - 1) / dmg;
        int use = min(can_use, need);
        if (use <= 0) continue;
        plan.use_count[weapon] += use;
        attack_total += use;
        hp -= use * dmg;
    }
    if (hp > 0) {
        plan.use_count[bare_idx] += hp;
        attack_total += hp;
        hp = 0;
    }
    plan.attacks = attack_total;
    return plan;
}

long long estimate_hits(int target, const vector<int> &remain, const vector<int> &durability,
                        const vector<char> &available, const vector<vector<int>> &damage,
                        const vector<vector<int>> &weapon_order, int bare_idx) {
    if (target < 0 || target >= (int)remain.size()) return 0;
    int hp = remain[target];
    if (hp <= 0) return 0;

    long long attacks = 0;
    for (int weapon : weapon_order[target]) {
        if (hp <= 0) break;
        if (weapon == bare_idx) {
            attacks += hp;
            hp = 0;
            break;
        }
        if (!available[weapon]) continue;
        int dmg = damage[weapon][target];
        if (dmg <= 0) continue;
        int can_use = durability[weapon];
        if (can_use <= 0) continue;
        int need = (hp + dmg - 1) / dmg;
        int use = min(can_use, need);
        if (use <= 0) continue;
        attacks += use;
        hp -= use * dmg;
    }
    if (hp > 0) attacks += hp;
    return attacks;
}

void apply_plan(int target, const AttackPlan &plan, const vector<vector<int>> &damage,
                const vector<int> &weapon_capacity, const vector<vector<int>> &weapon_order,
                int bare_idx, vector<int> &remain, vector<int> &durability,
                vector<char> &available, vector<pair<int, int>> &actions) {
    if (target < 0 || target >= (int)remain.size()) return;
    if (remain[target] <= 0) return;

    int hp = remain[target];
    for (int weapon : weapon_order[target]) {
        int use = (weapon < (int)plan.use_count.size()) ? plan.use_count[weapon] : 0;
        if (use <= 0) continue;
        int dmg = (weapon == bare_idx) ? 1 : damage[weapon][target];
        for (int i = 0; i < use && hp > 0; ++i) {
            int weapon_id = (weapon == bare_idx) ? -1 : weapon;
            if (weapon != bare_idx && durability[weapon] > 0) durability[weapon]--;
            hp -= dmg;
            actions.emplace_back(weapon_id, target);
        }
    }
    while (hp > 0) {
        actions.emplace_back(-1, target);
        hp--;
    }

    remain[target] = 0;
    if (!available[target]) {
        available[target] = 1;
        durability[target] += weapon_capacity[target];
    }
    durability[bare_idx] = INF_DURABILITY;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;
    const int BARE = N;

    vector<int> H(N), C(N);
    for (int i = 0; i < N; ++i) cin >> H[i];
    for (int i = 0; i < N; ++i) cin >> C[i];

    vector<vector<int>> A(N, vector<int>(N));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) cin >> A[i][j];

    vector<vector<int>> weapon_order(N);
    for (int target = 0; target < N; ++target) {
        vector<pair<int, int>> arr;
        arr.reserve(N);
        for (int weapon = 0; weapon < N; ++weapon) arr.emplace_back(A[weapon][target], weapon);
        sort(arr.begin(), arr.end(), [](const pair<int, int> &lhs, const pair<int, int> &rhs) {
            if (lhs.first != rhs.first) return lhs.first > rhs.first;
            return lhs.second < rhs.second;
        });
        weapon_order[target].reserve(N + 1);
        for (auto [_, weapon] : arr) weapon_order[target].push_back(weapon);
        weapon_order[target].push_back(BARE);
    }

    vector<vector<int>> weapon_targets(N);
    for (int weapon = 0; weapon < N; ++weapon) {
        vector<pair<int, int>> arr;
        arr.reserve(N);
        for (int target = 0; target < N; ++target) arr.emplace_back(A[weapon][target], target);
        sort(arr.begin(), arr.end(), [](const pair<int, int> &lhs, const pair<int, int> &rhs) {
            if (lhs.first != rhs.first) return lhs.first > rhs.first;
            return lhs.second < rhs.second;
        });
        weapon_targets[weapon].reserve(N);
        for (auto [_, target] : arr) weapon_targets[weapon].push_back(target);
    }

    vector<int> remain = H;
    vector<int> durability(N + 1, 0);
    vector<char> available(N + 1, 0);
    vector<char> opened(N, 0);
    durability[BARE] = INF_DURABILITY;
    available[BARE] = 1;

    vector<pair<int, int>> actions;
    actions.reserve(120000);

    vector<long long> hits_cache(N, 0);

    const int NEXT_TARGET_SAMPLE = 12;
    const int WEAPON_TARGET_SAMPLE = 18;
    const double NEXT_WEIGHT = 0.35;
    const double BENEFIT_WEIGHT = 0.25;
    const double BARE_PENALTY = 0.02;

    int opened_cnt = 0;
    while (opened_cnt < N) {
        vector<int> candidates;
        candidates.reserve(N - opened_cnt);
        for (int i = 0; i < N; ++i) {
            if (opened[i]) continue;
            if (remain[i] <= 0) continue;
            candidates.push_back(i);
        }
        if (candidates.empty()) break;

        for (int idx : candidates) {
            hits_cache[idx] =
                estimate_hits(idx, remain, durability, available, A, weapon_order, BARE);
        }

        vector<int> best_by_hits = candidates;
        if ((int)best_by_hits.size() > NEXT_TARGET_SAMPLE) {
            partial_sort(best_by_hits.begin(), best_by_hits.begin() + NEXT_TARGET_SAMPLE,
                         best_by_hits.end(), [&](int lhs, int rhs) {
                             if (hits_cache[lhs] != hits_cache[rhs])
                                 return hits_cache[lhs] < hits_cache[rhs];
                             return lhs < rhs;
                         });
            best_by_hits.resize(NEXT_TARGET_SAMPLE);
        }

        int best_target = -1;
        double best_score = 1e100;
        AttackPlan best_plan;

        for (int target : candidates) {
            AttackPlan plan =
                build_attack_plan(target, remain, durability, available, A, weapon_order, BARE);

            vector<int> dur_after = durability;
            bool feasible = true;
            for (int weapon = 0; weapon < N; ++weapon) {
                dur_after[weapon] -= plan.use_count.empty() ? 0 : plan.use_count[weapon];
                if (dur_after[weapon] < 0) {
                    feasible = false;
                    break;
                }
            }
            if (!feasible) continue;

            vector<char> avail_after = available;
            vector<int> remain_after = remain;
            remain_after[target] = 0;
            if (!avail_after[target]) {
                avail_after[target] = 1;
                dur_after[target] += C[target];
            }
            dur_after[BARE] = INF_DURABILITY;

            vector<int> sample_targets;
            sample_targets.reserve(NEXT_TARGET_SAMPLE + WEAPON_TARGET_SAMPLE + 2);
            for (int idx : best_by_hits) {
                if (idx == target) continue;
                if (opened[idx]) continue;
                if (remain[idx] <= 0) continue;
                sample_targets.push_back(idx);
            }
            int added = 0;
            for (int idx : weapon_targets[target]) {
                if (added >= WEAPON_TARGET_SAMPLE) break;
                if (idx == target) continue;
                if (opened[idx]) continue;
                if (remain[idx] <= 0) continue;
                sample_targets.push_back(idx);
                ++added;
            }
            sort(sample_targets.begin(), sample_targets.end());
            sample_targets.erase(unique(sample_targets.begin(), sample_targets.end()),
                                 sample_targets.end());

            double min_next = 0.0;
            double benefit = 0.0;
            bool has_next = false;
            for (int nxt : sample_targets) {
                long long after_hits =
                    estimate_hits(nxt, remain_after, dur_after, avail_after, A, weapon_order, BARE);
                if (!has_next || after_hits < min_next) {
                    min_next = static_cast<double>(after_hits);
                    has_next = true;
                }
                long long improvement = hits_cache[nxt] - after_hits;
                if (improvement > 0) benefit += improvement;
            }
            if (!has_next) min_next = 0.0;

            long long bare_usage =
                plan.use_count.empty() ? 0 : plan.use_count[BARE];
            double score = static_cast<double>(plan.attacks) + NEXT_WEIGHT * min_next -
                           BENEFIT_WEIGHT * benefit + BARE_PENALTY * bare_usage +
                           1e-6 * remain[target] + 1e-9 * target;

            if (score < best_score) {
                best_score = score;
                best_target = target;
                best_plan = plan;
            }
        }

        if (best_target == -1) {
            for (int target : candidates) {
                while (!opened[target] && remain[target] > 0) {
                    remain[target]--;
                    actions.emplace_back(-1, target);
                }
                if (!opened[target]) {
                    opened[target] = 1;
                    if (!available[target]) {
                        available[target] = 1;
                        durability[target] += C[target];
                    }
                    opened_cnt++;
                }
            }
            break;
        }

        apply_plan(best_target, best_plan, A, C, weapon_order, BARE, remain, durability, available,
                   actions);
        if (!opened[best_target]) {
            opened[best_target] = 1;
            opened_cnt++;
        }
    }

    for (auto [weapon, box] : actions) {
        cout << weapon << ' ' << box << '\n';
    }

    return 0;
}
