#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const double EPS = 1e-9;

struct Paint {
    double r, g, b, amount;
    Paint(double r = 0, double g = 0, double b = 0, double a = 0) : r(r), g(g), b(b), amount(a) {}
    
    Paint mix(const Paint& other) const {
        if (amount + other.amount < EPS) return Paint();
        double total = amount + other.amount;
        return Paint(
            (r * amount + other.r * other.amount) / total,
            (g * amount + other.g * other.amount) / total,
            (b * amount + other.b * other.amount) / total,
            total
        );
    }
    
    double dist(const Paint& target) const {
        return sqrt((r - target.r) * (r - target.r) + 
                   (g - target.g) * (g - target.g) + 
                   (b - target.b) * (b - target.b));
    }
};

struct Action {
    int area;           // 使用エリア (0-3)
    vector<int> tubes;  // 使用絵具 [id, amount] ペア
    vector<int> amounts;
    int discard;        // 捨てる量
    
    Action(int a = 0) : area(a), discard(0) {}
};

struct State {
    vector<Paint> areas;     // 4エリアの現在色
    int target_idx;          // 現在の目標色インデックス
    double total_score;      // 累積スコア
    vector<Action> actions;  // 行動履歴
    
    State(int n_areas = 4) : areas(n_areas), target_idx(0), total_score(0) {}
    
    State copy() const {
        State s;
        s.areas = areas;
        s.target_idx = target_idx;
        s.total_score = total_score;
        s.actions = actions;
        return s;
    }
};

class UltimateSolver {
    int n, k, h, t, d;
    vector<Paint> tubes, targets;
    chrono::steady_clock::time_point start_time;
    mt19937 rng;
    
    bool time_up() {
        auto now = chrono::steady_clock::now();
        return chrono::duration_cast<chrono::milliseconds>(now - start_time).count() > 2800;
    }
    
    // 3手先を見越した評価関数
    double evaluate_action(const State& state, const Action& action) {
        if (action.target_idx >= h) return 0;
        
        // 現在のアクションを実行した状態を計算
        State next_state = state.copy();
        Paint current_color = next_state.areas[action.area];
        
        // 絵具を追加
        for (int i = 0; i < action.tubes.size(); i++) {
            for (int j = 0; j < action.amounts[i]; j++) {
                current_color = current_color.mix(tubes[action.tubes[i]]);
            }
        }
        
        // 現在スコア
        double current_score = current_color.dist(targets[action.target_idx]) * 10000;
        int paint_cost = 0;
        for (int amount : action.amounts) paint_cost += amount;
        current_score += max(0, paint_cost - 1) * d;
        
        // 1手先評価
        Paint remaining(current_color.r, current_color.g, current_color.b, 
                       max(0.0, current_color.amount - 1.0));
        double future_score1 = 0;
        if (action.target_idx + 1 < h) {
            future_score1 = estimate_future_cost(remaining, targets[action.target_idx + 1]);
        }
        
        // 2手先評価（軽量版）
        double future_score2 = 0;
        if (action.target_idx + 2 < h && remaining.amount > 1) {
            Paint remaining2(remaining.r, remaining.g, remaining.b, remaining.amount - 1);
            future_score2 = estimate_future_cost(remaining2, targets[action.target_idx + 2]) * 0.3;
        }
        
        return current_score + future_score1 * 0.4 + future_score2;
    }
    
    // 軽量将来コスト推定
    double estimate_future_cost(const Paint& state, const Paint& target) {
        if (state.amount <= 0) return 1e6;
        
        double best_score = 1e9;
        
        // 各絵具1-3個での最良スコア
        for (int i = 0; i < k; i++) {
            for (int amount = 1; amount <= 3; amount++) {
                Paint mixed = state;
                for (int j = 0; j < amount; j++) {
                    mixed = mixed.mix(tubes[i]);
                }
                double score = mixed.dist(target) * 10000 + amount * d;
                best_score = min(best_score, score);
            }
        }
        
        // 何もしない場合
        if (state.amount > 0) {
            best_score = min(best_score, state.dist(target) * 10000);
        }
        
        return best_score;
    }
    
    // 近傍解生成
    vector<Action> generate_neighbors(const State& state) {
        vector<Action> neighbors;
        if (state.target_idx >= h) return neighbors;
        
        const Paint& target = targets[state.target_idx];
        
        // 各エリアで最適解候補を生成
        for (int area = 0; area < 4; area++) {
            const Paint& current = state.areas[area];
            
            // 基本的な組み合わせ
            for (int i = 0; i < k; i++) {
                for (int j = i; j < k; j++) {
                    for (int k1 = j; k1 < k; k1++) {
                        for (int a1 = 0; a1 <= 5; a1++) {
                            for (int a2 = 0; a2 <= 3; a2++) {
                                for (int a3 = 0; a3 <= 2; a3++) {
                                    if (a1 + a2 + a3 == 0) continue;
                                    if (a1 + a2 + a3 > 8) continue;
                                    
                                    Action action(area);
                                    if (a1 > 0) { action.tubes.push_back(i); action.amounts.push_back(a1); }
                                    if (a2 > 0) { action.tubes.push_back(j); action.amounts.push_back(a2); }
                                    if (a3 > 0) { action.tubes.push_back(k1); action.amounts.push_back(a3); }
                                    
                                    // 廃棄量の決定
                                    ll qq = (ll)current.amount / 4;
                                    ll rr = (ll)current.amount % 4;
                                    for (int dis = 0; dis < 4; dis++) {
                                        action.discard = current.amount - (dis < rr ? qq + 1 : qq);
                                        action.target_idx = state.target_idx;
                                        neighbors.push_back(action);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        return neighbors;
    }
    
    // 焼きなまし最適化
    State simulated_annealing(State initial_state) {
        State current = initial_state;
        State best = current;
        double temperature = 10000.0;
        const double cooling_rate = 0.9995;
        int iterations = 0;
        
        while (!time_up() && temperature > 1e-8) {
            vector<Action> neighbors = generate_neighbors(current);
            if (neighbors.empty()) break;
            
            // ランダムに近傍解を選択
            Action candidate = neighbors[rng() % neighbors.size()];
            double candidate_score = evaluate_action(current, candidate);
            
            double delta = candidate_score - current.total_score;
            
            // 受容判定
            bool accept = false;
            if (delta < 0) {
                accept = true;  // 改善解
            } else {
                double prob = exp(-delta / temperature);
                accept = (rng() / (double)rng.max()) < prob;
            }
            
            if (accept) {
                // 状態更新
                State next_state = current.copy();
                next_state.total_score = candidate_score;
                next_state.target_idx++;
                next_state.actions.push_back(candidate);
                
                // エリアの色を更新
                Paint new_color = next_state.areas[candidate.area];
                for (int i = 0; i < candidate.tubes.size(); i++) {
                    for (int j = 0; j < candidate.amounts[i]; j++) {
                        new_color = new_color.mix(tubes[candidate.tubes[i]]);
                    }
                }
                new_color.amount = max(0.0, new_color.amount - 1.0);
                next_state.areas[candidate.area] = new_color;
                
                current = next_state;
                
                if (current.total_score < best.total_score) {
                    best = current;
                }
            }
            
            temperature *= cooling_rate;
            iterations++;
            
            if (iterations % 1000 == 0) {
                cerr << "Iter: " << iterations << ", Temp: " << temperature 
                     << ", Best: " << best.total_score << endl;
            }
        }
        
        return best;
    }
    
    void output_palette() {
        // 4エリア分割パレット
        // Area0: (0,0)-(n/2-1, n/2-1)
        // Area1: (n/2,0)-(n-1, n/2-1)  
        // Area2: (0,n/2)-(n/2-1, n-1)
        // Area3: (n/2,n/2)-(n-1, n-1)
        
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
    
    void output_actions(const State& solution) {
        for (const Action& action : solution.actions) {
            int area_x = (action.area >= 2) ? 0 : (n/2);
            int area_y = (action.area % 2 == 1) ? 0 : (n/2);
            
            // 廃棄
            for (int i = 0; i < action.discard; i++) {
                cout << "3 " << area_x << " " << area_y << "\n";
            }
            
            // 絵具追加
            for (int i = 0; i < action.tubes.size(); i++) {
                for (int j = 0; j < action.amounts[i]; j++) {
                    cout << "1 " << area_x << " " << area_y << " " << action.tubes[i] << "\n";
                }
            }
            
            // 色取り出し
            cout << "2 " << area_x << " " << area_y << "\n";
        }
    }
    
public:
    void solve() {
        start_time = chrono::steady_clock::now();
        rng.seed(42);
        
        cin >> n >> k >> h >> t >> d;
        tubes.resize(k);
        targets.resize(h);
        
        for (int i = 0; i < k; i++) {
            cin >> tubes[i].r >> tubes[i].g >> tubes[i].b;
            tubes[i].amount = 1.0;
        }
        for (int i = 0; i < h; i++) {
            cin >> targets[i].r >> targets[i].g >> targets[i].b;
            targets[i].amount = 1.0;
        }
        
        output_palette();
        
        // 初期状態
        State initial_state(4);
        
        // 焼きなまし最適化
        State solution = simulated_annealing(initial_state);
        
        output_actions(solution);
        
        cerr << "Final score: " << solution.total_score << endl;
        cerr << "Time: " << chrono::duration_cast<chrono::milliseconds>(
            chrono::steady_clock::now() - start_time).count() << "ms" << endl;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    UltimateSolver solver;
    solver.solve();
    
    return 0;
} 