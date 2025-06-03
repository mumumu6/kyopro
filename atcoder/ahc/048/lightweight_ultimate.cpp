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

class LightweightUltimateSolver {
    int n, k, h, t, d;
    vector<Paint> tubes, targets;
    vector<Paint> areas;  // 4エリアの現在状態
    chrono::steady_clock::time_point start_time;
    mt19937 rng;
    
    bool time_up() {
        auto now = chrono::steady_clock::now();
        return chrono::duration_cast<chrono::milliseconds>(now - start_time).count() > 2800;
    }
    
    // 超軽量1手先読み評価
    double lightweight_lookahead(int target_idx, const Paint& current_state, 
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
        
        // 超軽量1手先評価（5%のペナルティ追加のみ）
        double future_penalty = 0;
        if (target_idx + 1 < h && mixed.count > 1) {
            Paint next_state(mixed.r, mixed.g, mixed.b, max(0.0, mixed.count - 1.0));
            // 現在色のまま使った場合の推定ペナルティ
            double stay_penalty = next_state.dist(targets[target_idx + 1]) * 10000;
            future_penalty = stay_penalty * 0.05;  // 極軽量
        }
        
        return current_score + future_penalty;
    }
    
    // 軽量最適化（a2.cppベース + 4エリア + 軽量先読み）
    pair<int,double> solve_one_color_lightweight(int target_idx) {
        double best_score = 1e9;
        int best_area = 0;
        vector<int> best_tubes, best_amounts;
        int best_discard = 0;
        
        // 4エリア全てで試行（ただし軽量）
        for (int area = 0; area < 4; area++) {
            const Paint& current = areas[area];
            
            // a2.cpp風の軽量探索: 7×5×3 = 105通りのみ
            for (int c1 = 0; c1 < k; c1++) {
                for (int c2 = c1; c2 < k; c2++) {
                    for (int c3 = c2; c3 < k; c3++) {
                        for (int a1 = 0; a1 <= 6; a1++) {
                            for (int a2 = 0; a2 <= 4; a2++) {
                                for (int a3 = 0; a3 <= 2; a3++) {
                                    if (a1 + a2 + a3 == 0) continue;
                                    if (a1 + a2 + a3 > 7) continue; // さらに制限
                                    
                                    vector<int> tube_ids, amounts;
                                    if (a1 > 0) { tube_ids.push_back(c1); amounts.push_back(a1); }
                                    if (a2 > 0) { tube_ids.push_back(c2); amounts.push_back(a2); }
                                    if (a3 > 0) { tube_ids.push_back(c3); amounts.push_back(a3); }
                                    
                                    // 廃棄量：最大2パターンのみ
                                    ll qq = (ll)current.count / 4;
                                    ll rr = (ll)current.count % 4;
                                    
                                    for (int dis_opt = 0; dis_opt < 2; dis_opt++) { // 4→2に削減
                                        int discard = current.count - (dis_opt < rr ? qq + 1 : qq);
                                        discard = max(0, discard);
                                        
                                        Paint test_state(current.r, current.g, current.b, 
                                                       max(0.0, current.count - discard));
                                        
                                        double score = lightweight_lookahead(target_idx, test_state, 
                                                                            tube_ids, amounts);
                                        
                                        if (score < best_score) {
                                            best_score = score;
                                            best_area = area;
                                            best_tubes = tube_ids;
                                            best_amounts = amounts;
                                            best_discard = discard;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        // 軽量SA（時間があれば、各色最大50回のみ）
        if (!time_up() && best_score < 1e8) {
            double temp = best_score * 0.1;
            vector<int> sa_tubes = best_tubes;
            vector<int> sa_amounts = best_amounts;
            
            for (int iter = 0; iter < 50 && !time_up(); iter++) {
                vector<int> neighbor_amounts = sa_amounts;
                
                // 軽微な変更のみ
                if (!neighbor_amounts.empty()) {
                    int idx = rng() % neighbor_amounts.size();
                    int delta = (rng() % 2) ? 1 : -1;
                    neighbor_amounts[idx] = max(1, min(6, neighbor_amounts[idx] + delta));
                    
                    Paint test_state(areas[best_area].r, areas[best_area].g, areas[best_area].b, 
                                   max(0.0, areas[best_area].count - best_discard));
                    
                    double neighbor_score = lightweight_lookahead(target_idx, test_state, 
                                                                 sa_tubes, neighbor_amounts);
                    
                    double delta_score = neighbor_score - best_score;
                    if (delta_score < 0 || exp(-delta_score / temp) > rng() / (double)rng.max()) {
                        sa_amounts = neighbor_amounts;
                        if (neighbor_score < best_score) {
                            best_score = neighbor_score;
                            best_amounts = sa_amounts;
                        }
                    }
                }
                temp *= 0.95;
            }
        }
        
        // 解を適用
        apply_solution(best_area, best_tubes, best_amounts, best_discard);
        return {best_area, best_score};
    }
    
    void apply_solution(int area, const vector<int>& tube_ids, const vector<int>& amounts, int discard) {
        // エリア座標計算
        int ax = (area < 2) ? 0 : n/2;
        int ay = (area % 2 == 0) ? 0 : n/2;
        
        // 廃棄
        for (int i = 0; i < discard; i++) {
            cout << "3 " << ax << " " << ay << "\n";
        }
        
        // 絵具追加
        for (int i = 0; i < tube_ids.size(); i++) {
            for (int j = 0; j < amounts[i]; j++) {
                cout << "1 " << ax << " " << ay << " " << tube_ids[i] << "\n";
            }
        }
        
        // 色取り出し
        cout << "2 " << ax << " " << ay << "\n";
        
        // エリア状態更新
        Paint& area_color = areas[area];
        area_color.count = max(0.0, area_color.count - discard);
        
        for (int i = 0; i < tube_ids.size(); i++) {
            for (int j = 0; j < amounts[i]; j++) {
                area_color = area_color.mix(tubes[tube_ids[i]], 1);
            }
        }
        area_color.count = max(0.0, area_color.count - 1.0);
    }
    
    void output_palette() {
        // 4エリア分割: 2x2グリッド
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n-1; j++) {
                cout << (j == n/2 - 1 ? "1" : "0") << " ";
            }
            cout << "\n";
        }
        
        for (int i = 0; i < n-1; i++) {
            for (int j = 0; j < n; j++) {
                cout << (i == n/2 - 1 ? "1" : "0") << " ";
            }
            cout << "\n";
        }
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
        
        double total_score = 0;
        
        // メイン処理：1色ずつ最適化（軽量版）
        for (int hi = 0; hi < h && !time_up(); hi++) {
            auto [area, score] = solve_one_color_lightweight(hi);
            total_score += score;
            
            // 定期的なエリア間バランス調整（軽量版）
            if (hi % 20 == 19) {
                int max_area = 0, min_area = 0;
                for (int a = 1; a < 4; a++) {
                    if (areas[a].count > areas[max_area].count) max_area = a;
                    if (areas[a].count < areas[min_area].count) min_area = a;
                }
                
                if (areas[max_area].count > areas[min_area].count + 4) {
                    int max_ax = (max_area < 2) ? 0 : n/2;
                    int max_ay = (max_area % 2 == 0) ? 0 : n/2;
                    int min_ax = (min_area < 2) ? 0 : n/2;
                    int min_ay = (min_area % 2 == 0) ? 0 : n/2;
                    
                    cout << "4 2 " << max_ax << " " << max_ay << " " << min_ax << " " << min_ay << "\n";
                    
                    double move_amount = 2.0;
                    areas[min_area] = areas[min_area].mix(areas[max_area], move_amount);
                    areas[max_area].count = max(0.0, areas[max_area].count - move_amount);
                }
            }
        }
        
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_time).count();
        cerr << "Total Score: " << total_score << ", Time: " << elapsed << "ms" << endl;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    LightweightUltimateSolver solver;
    solver.solve();
    
    return 0;
} 