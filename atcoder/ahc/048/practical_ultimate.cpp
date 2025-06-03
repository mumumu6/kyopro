#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Paint {
    double r, g, b, count;
    Paint(double r = 0, double g = 0, double b = 0, double c = 0) : r(r), g(g), b(b), count(c) {}
    
    Paint mix(const Paint& other, int amount) const {
        if (count + amount <= 0) return Paint();
        double total = count + amount;
        return Paint(
            (r * count + other.r * amount) / total,
            (g * count + other.g * amount) / total,
            (b * count + other.b * amount) / total,
            total
        );
    }
    
    double dist(const Paint& target) const {
        return sqrt((r - target.r) * (r - target.r) + 
                   (g - target.g) * (g - target.g) + 
                   (b - target.b) * (b - target.b));
    }
};

struct Solution {
    int area;
    vector<int> tube_ids;
    vector<int> amounts;
    int discard;
    double score;
    
    Solution() : area(0), discard(0), score(1e9) {}
};

class PracticalUltimateSolver {
    int n, k, h, t, d;
    vector<Paint> tubes, targets;
    vector<Paint> areas;  // 4エリアの現在状態
    chrono::steady_clock::time_point start_time;
    mt19937 rng;
    
    bool time_up() {
        auto now = chrono::steady_clock::now();
        return chrono::duration_cast<chrono::milliseconds>(now - start_time).count() > 2800;
    }
    
    // 軽量2手先読み評価
    double evaluate_with_lookahead(int target_idx, const Paint& current_state, 
                                  const vector<int>& tube_ids, const vector<int>& amounts) {
        if (target_idx >= h) return 1e9;
        
        // 現在の混色結果
        Paint mixed = current_state;
        int total_paint_cost = 0;
        for (int i = 0; i < tube_ids.size(); i++) {
            for (int j = 0; j < amounts[i]; j++) {
                mixed = mixed.mix(tubes[tube_ids[i]], 1);
            }
            total_paint_cost += amounts[i];
        }
        
        // 現在スコア
        double current_score = mixed.dist(targets[target_idx]) * 10000 + max(0, total_paint_cost - 1) * d;
        
        // 1手先簡易評価
        double future_penalty = 0;
        if (target_idx + 1 < h && mixed.count > 1) {
            Paint next_state(mixed.r, mixed.g, mixed.b, max(0.0, mixed.count - 1.0));
            double best_future = 1e9;
            
            // 軽量探索：各絵具1-2個での最良解
            for (int i = 0; i < k; i++) {
                for (int amt = 1; amt <= 2; amt++) {
                    Paint test = next_state;
                    for (int j = 0; j < amt; j++) {
                        test = test.mix(tubes[i], 1);
                    }
                    double score = test.dist(targets[target_idx + 1]) * 10000 + amt * d;
                    best_future = min(best_future, score);
                }
            }
            
            // 何もしない場合
            if (next_state.count > 0) {
                best_future = min(best_future, next_state.dist(targets[target_idx + 1]) * 10000);
            }
            
            future_penalty = best_future * 0.3;  // 割引係数
        }
        
        return current_score + future_penalty;
    }
    
    // 焼きなまし付き最適化（1色ずつ）
    Solution solve_one_color(int target_idx, int preferred_area = -1) {
        Solution best_sol;
        
        vector<int> areas_to_try;
        if (preferred_area >= 0) {
            areas_to_try = {preferred_area};
        } else {
            areas_to_try = {0, 1, 2, 3};
        }
        
        for (int area : areas_to_try) {
            const Paint& current = areas[area];
            
            // 基本貪欲解
            for (int c1 = 0; c1 < k; c1++) {
                for (int c2 = c1; c2 < k; c2++) {
                    for (int c3 = c2; c3 < k; c3++) {
                        for (int a1 = 0; a1 <= 6; a1++) {
                            for (int a2 = 0; a2 <= 4; a2++) {
                                for (int a3 = 0; a3 <= 3; a3++) {
                                    if (a1 + a2 + a3 == 0) continue;
                                    if (a1 + a2 + a3 > 8) continue;
                                    
                                    vector<int> tube_ids, amounts;
                                    if (a1 > 0) { tube_ids.push_back(c1); amounts.push_back(a1); }
                                    if (a2 > 0) { tube_ids.push_back(c2); amounts.push_back(a2); }
                                    if (a3 > 0) { tube_ids.push_back(c3); amounts.push_back(a3); }
                                    
                                    // 廃棄量の最適化（軽量SA）
                                    ll qq = (ll)current.count / 4;
                                    ll rr = (ll)current.count % 4;
                                    
                                    for (int dis_opt = 0; dis_opt < 4; dis_opt++) {
                                        int discard = current.count - (dis_opt < rr ? qq + 1 : qq);
                                        discard = max(0, discard);
                                        
                                        Paint test_state(current.r, current.g, current.b, 
                                                       max(0.0, current.count - discard));
                                        
                                        double score = evaluate_with_lookahead(target_idx, test_state, 
                                                                              tube_ids, amounts);
                                        
                                        if (score < best_sol.score) {
                                            best_sol.area = area;
                                            best_sol.tube_ids = tube_ids;
                                            best_sol.amounts = amounts;
                                            best_sol.discard = discard;
                                            best_sol.score = score;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            // 時間があれば軽量SA（近傍：±1調整）
            if (!time_up() && best_sol.score < 1e8) {
                Solution sa_sol = best_sol;
                double temp = best_sol.score * 0.1;
                
                for (int iter = 0; iter < 200 && !time_up(); iter++) {
                    Solution neighbor = sa_sol;
                    
                    // 近傍操作：amounts[i] ± 1
                    if (!neighbor.amounts.empty()) {
                        int idx = rng() % neighbor.amounts.size();
                        int delta = (rng() % 2) ? 1 : -1;
                        neighbor.amounts[idx] = max(0, min(8, neighbor.amounts[idx] + delta));
                        
                        if (neighbor.amounts[idx] == 0) {
                            neighbor.tube_ids.erase(neighbor.tube_ids.begin() + idx);
                            neighbor.amounts.erase(neighbor.amounts.begin() + idx);
                        }
                        
                        if (!neighbor.amounts.empty()) {
                            Paint test_state(current.r, current.g, current.b, 
                                           max(0.0, current.count - neighbor.discard));
                            neighbor.score = evaluate_with_lookahead(target_idx, test_state, 
                                                                    neighbor.tube_ids, neighbor.amounts);
                            
                            double delta_score = neighbor.score - sa_sol.score;
                            if (delta_score < 0 || exp(-delta_score / temp) > rng() / (double)rng.max()) {
                                sa_sol = neighbor;
                                if (sa_sol.score < best_sol.score) {
                                    best_sol = sa_sol;
                                }
                            }
                        }
                    }
                    
                    temp *= 0.98;
                }
            }
        }
        
        return best_sol;
    }
    
    void output_palette() {
        // 4エリア分割: 2x2グリッド
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n-1; j++) {
                if (j == n/2 - 1) {
                    cout << "1 ";
                } else {
                    cout << "0 ";
                }
            }
            cout << "\n";
        }
        
        for (int i = 0; i < n-1; i++) {
            for (int j = 0; j < n; j++) {
                if (i == n/2 - 1) {
                    cout << "1 ";
                } else {
                    cout << "0 ";
                }
            }
            cout << "\n";
        }
    }
    
    void apply_solution(const Solution& sol, int target_idx) {
        // エリア座標計算
        int ax = (sol.area < 2) ? 0 : n/2;
        int ay = (sol.area % 2 == 0) ? 0 : n/2;
        
        // 廃棄
        for (int i = 0; i < sol.discard; i++) {
            cout << "3 " << ax << " " << ay << "\n";
        }
        
        // 絵具追加
        for (int i = 0; i < sol.tube_ids.size(); i++) {
            for (int j = 0; j < sol.amounts[i]; j++) {
                cout << "1 " << ax << " " << ay << " " << sol.tube_ids[i] << "\n";
            }
        }
        
        // 色取り出し
        cout << "2 " << ax << " " << ay << "\n";
        
        // エリア状態更新
        Paint& area_color = areas[sol.area];
        area_color.count = max(0.0, area_color.count - sol.discard);
        
        for (int i = 0; i < sol.tube_ids.size(); i++) {
            for (int j = 0; j < sol.amounts[i]; j++) {
                area_color = area_color.mix(tubes[sol.tube_ids[i]], 1);
            }
        }
        area_color.count = max(0.0, area_color.count - 1.0);
    }
    
public:
    void solve() {
        start_time = chrono::steady_clock::now();
        rng.seed(chrono::duration_cast<chrono::nanoseconds>(start_time.time_since_epoch()).count());
        
        cin >> n >> k >> h >> t >> d;
        tubes.resize(k);
        targets.resize(h);
        areas.resize(4);
        
        for (int i = 0; i < k; i++) {
            cin >> tubes[i].r >> tubes[i].g >> tubes[i].b;
            tubes[i].count = 1.0;
        }
        for (int i = 0; i < h; i++) {
            cin >> targets[i].r >> targets[i].g >> targets[i].b;
        }
        
        output_palette();
        
        // メイン処理：1色ずつ最適化
        for (int hi = 0; hi < h; hi++) {
            // エリア負荷分散（簡易）
            int preferred_area = -1;
            double min_usage = 1e9;
            for (int a = 0; a < 4; a++) {
                if (areas[a].count < min_usage) {
                    min_usage = areas[a].count;
                    preferred_area = a;
                }
            }
            
            Solution sol = solve_one_color(hi, preferred_area);
            apply_solution(sol, hi);
            
            // 定期的なエリア間バランス調整
            if (hi % 15 == 14) {
                // 最も多いエリアから最も少ないエリアに移動
                int max_area = 0, min_area = 0;
                for (int a = 1; a < 4; a++) {
                    if (areas[a].count > areas[max_area].count) max_area = a;
                    if (areas[a].count < areas[min_area].count) min_area = a;
                }
                
                if (areas[max_area].count > areas[min_area].count + 3) {
                    int max_ax = (max_area < 2) ? 0 : n/2;
                    int max_ay = (max_area % 2 == 0) ? 0 : n/2;
                    int min_ax = (min_area < 2) ? 0 : n/2;
                    int min_ay = (min_area % 2 == 0) ? 0 : n/2;
                    
                    cout << "4 2 " << max_ax << " " << max_ay << " " << min_ax << " " << min_ay << "\n";
                    
                    // 状態更新
                    double move_amount = 2.0;
                    areas[min_area] = areas[min_area].mix(areas[max_area], move_amount);
                    areas[max_area].count = max(0.0, areas[max_area].count - move_amount);
                }
            }
        }
        
        cerr << "Execution time: " << chrono::duration_cast<chrono::milliseconds>(
            chrono::steady_clock::now() - start_time).count() << "ms" << endl;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    PracticalUltimateSolver solver;
    solver.solve();
    
    return 0;
} 