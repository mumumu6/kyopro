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

struct State {
    Paint current_color;
    vector<vector<int>> recipes; // これまでに使ったレシピ
    double total_score;
    int colors_made;
    
    State() : current_color(0, 0, 0, 0), total_score(0), colors_made(0) {}
    
    double evaluate_next(const Paint& target, const vector<int>& recipe, 
                        const vector<Paint>& tubes, int d) const {
        Paint made = current_color;
        int cost = 0;
        
        for (int i = 0; i < recipe.size(); i++) {
            for (int cnt = 0; cnt < recipe[i]; cnt++) {
                made = made.mix(tubes[i]);
                cost++;
            }
        }
        
        if (made.amount < EPS) return 1e9;
        
        double color_error = made.dist(target);
        double cost_penalty = max(0, cost - 1) * d;
        
        return color_error * 10000 + cost_penalty;
    }
};

struct BeamSolver {
    int n, k, h, t, d;
    vector<Paint> tubes, targets;
    
    void read_input() {
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
    }
    
    // 効率的なレシピ候補生成
    vector<vector<int>> generate_recipe_candidates(const Paint& current, const Paint& target) {
        vector<vector<int>> candidates;
        
        // 単色での候補
        for (int i = 0; i < k; i++) {
            for (int amount = 1; amount <= 8; amount++) {
                vector<int> recipe(k, 0);
                recipe[i] = amount;
                candidates.push_back(recipe);
            }
        }
        
        // 2色の組み合わせ
        for (int i = 0; i < k; i++) {
            for (int j = i + 1; j < k; j++) {
                for (int a1 = 1; a1 <= 6; a1++) {
                    for (int a2 = 1; a2 <= 6; a2++) {
                        if (a1 + a2 <= 10) {
                            vector<int> recipe(k, 0);
                            recipe[i] = a1;
                            recipe[j] = a2;
                            candidates.push_back(recipe);
                        }
                    }
                }
            }
        }
        
        // 3色の組み合わせ（厳選）
        for (int i = 0; i < k; i++) {
            for (int j = i + 1; j < k; j++) {
                for (int l = j + 1; l < k; l++) {
                    for (int a1 = 1; a1 <= 4; a1++) {
                        for (int a2 = 1; a2 <= 4; a2++) {
                            for (int a3 = 1; a3 <= 4; a3++) {
                                if (a1 + a2 + a3 <= 8) {
                                    vector<int> recipe(k, 0);
                                    recipe[i] = a1;
                                    recipe[j] = a2;
                                    recipe[l] = a3;
                                    candidates.push_back(recipe);
                                }
                            }
                        }
                    }
                }
            }
        }
        
        return candidates;
    }
    
    // ビームサーチで解を構築
    State beam_search() {
        const int BEAM_WIDTH = 200; // ビーム幅
        const int LOOKAHEAD = 3;    // 先読み深度
        
        vector<State> beam = {State()};
        
        for (int color_idx = 0; color_idx < h; color_idx++) {
            vector<State> next_beam;
            
            for (const State& state : beam) {
                // 複数ステップ先まで考慮
                vector<vector<int>> candidates = generate_recipe_candidates(
                    state.current_color, targets[color_idx]);
                
                for (const auto& recipe : candidates) {
                    State new_state = state;
                    
                    // 現在の色の評価
                    double immediate_score = state.evaluate_next(
                        targets[color_idx], recipe, tubes, d);
                    
                    // 将来の色も考慮した評価
                    double future_score = 0;
                    Paint hypothetical = state.current_color;
                    
                    // レシピを適用
                    for (int i = 0; i < k; i++) {
                        for (int cnt = 0; cnt < recipe[i]; cnt++) {
                            hypothetical = hypothetical.mix(tubes[i]);
                        }
                    }
                    
                    if (hypothetical.amount > EPS) {
                        // 残り絵具を計算
                        Paint remaining(hypothetical.r, hypothetical.g, hypothetical.b, 
                                      max(0.0, hypothetical.amount - 1.0));
                        
                        // 次の数色への影響を評価
                        for (int look = 1; look <= LOOKAHEAD && color_idx + look < h; look++) {
                            Paint target_future = targets[color_idx + look];
                            future_score += remaining.dist(target_future) * (1.0 / look);
                        }
                        
                        new_state.current_color = remaining;
                        new_state.recipes.push_back(recipe);
                        new_state.total_score += immediate_score + future_score * 1000;
                        new_state.colors_made++;
                        
                        next_beam.push_back(new_state);
                    }
                }
            }
            
            // 評価順にソート
            sort(next_beam.begin(), next_beam.end(), 
                 [](const State& a, const State& b) {
                     return a.total_score < b.total_score;
                 });
            
            // 上位BEAM_WIDTH個を保持
            if (next_beam.size() > BEAM_WIDTH) {
                next_beam.resize(BEAM_WIDTH);
            }
            
            beam = next_beam;
            
            if (beam.empty()) {
                cerr << "Beam search failed at color " << color_idx << endl;
                break;
            }
            
            if (color_idx % 100 == 0) {
                cerr << "Color " << color_idx << ", best score: " << beam[0].total_score << endl;
            }
        }
        
        return beam.empty() ? State() : beam[0];
    }
    
    void output_solution(const State& solution) {
        // パレット設定
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n - 1; j++) {
                cout << 0 << " ";
            }
            cout << "\n";
        }
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n; j++) {
                cout << 0 << " ";
            }
            cout << "\n";
        }
        
        // 操作出力
        for (int color_idx = 0; color_idx < solution.recipes.size(); color_idx++) {
            const auto& recipe = solution.recipes[color_idx];
            
            // 絵具を追加
            for (int j = 0; j < k; j++) {
                for (int cnt = 0; cnt < recipe[j]; cnt++) {
                    cout << "1 0 0 " << j << "\n";
                }
            }
            // 色を取り出し
            cout << "2 0 0\n";
        }
    }
    
    void solve() {
        read_input();
        
        State best_solution = beam_search();
        
        cerr << "Final score: " << best_solution.total_score << endl;
        cerr << "Colors made: " << best_solution.colors_made << endl;
        
        output_solution(best_solution);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    BeamSolver solver;
    solver.solve();
    
    return 0;
} 