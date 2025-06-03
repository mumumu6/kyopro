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

struct FastSolver {
    int n, k, h, t, d;
    vector<Paint> tubes, targets;
    chrono::steady_clock::time_point start_time;
    
    bool time_up() {
        auto now = chrono::steady_clock::now();
        return chrono::duration_cast<chrono::milliseconds>(now - start_time).count() > 2800; // 2.8秒で切り上げ
    }
    
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
    
    // 高速な最適レシピ探索（元の7^3から大幅拡張）
    vector<int> find_best_recipe(const Paint& current, const Paint& target) {
        vector<int> best_recipe(k, 0);
        double best_score = 1e9;
        
        // まず単色での最適解を探す
        for (int i = 0; i < k; i++) {
            for (int amount = 1; amount <= 12; amount++) {
                Paint made = current;
                for (int cnt = 0; cnt < amount; cnt++) {
                    made = made.mix(tubes[i]);
                }
                
                if (made.amount > EPS) {
                    double score = made.dist(target) * 10000 + max(0, amount - 1) * d;
                    if (score < best_score) {
                        best_score = score;
                        fill(best_recipe.begin(), best_recipe.end(), 0);
                        best_recipe[i] = amount;
                    }
                }
            }
        }
        
        // 2色の組み合わせ（効率化）
        for (int i = 0; i < k; i++) {
            for (int j = i + 1; j < k; j++) {
                for (int a1 = 1; a1 <= 8; a1++) {
                    for (int a2 = 1; a2 <= 8; a2++) {
                        if (a1 + a2 > 15) continue; // 制限
                        
                        Paint made = current;
                        for (int cnt = 0; cnt < a1; cnt++) made = made.mix(tubes[i]);
                        for (int cnt = 0; cnt < a2; cnt++) made = made.mix(tubes[j]);
                        
                        if (made.amount > EPS) {
                            double score = made.dist(target) * 10000 + max(0, a1 + a2 - 1) * d;
                            if (score < best_score) {
                                best_score = score;
                                fill(best_recipe.begin(), best_recipe.end(), 0);
                                best_recipe[i] = a1;
                                best_recipe[j] = a2;
                            }
                        }
                    }
                }
            }
        }
        
        // 3色の組み合わせ（さらに限定）
        if (!time_up()) {
            for (int i = 0; i < k; i += 2) { // 間引いて高速化
                for (int j = i + 1; j < k; j += 2) {
                    for (int l = j + 1; l < k; l += 2) {
                        for (int a1 = 1; a1 <= 5; a1++) {
                            for (int a2 = 1; a2 <= 5; a2++) {
                                for (int a3 = 1; a3 <= 5; a3++) {
                                    if (a1 + a2 + a3 > 10) continue;
                                    
                                    Paint made = current;
                                    for (int cnt = 0; cnt < a1; cnt++) made = made.mix(tubes[i]);
                                    for (int cnt = 0; cnt < a2; cnt++) made = made.mix(tubes[j]);
                                    for (int cnt = 0; cnt < a3; cnt++) made = made.mix(tubes[l]);
                                    
                                    if (made.amount > EPS) {
                                        double score = made.dist(target) * 10000 + max(0, a1 + a2 + a3 - 1) * d;
                                        if (score < best_score) {
                                            best_score = score;
                                            fill(best_recipe.begin(), best_recipe.end(), 0);
                                            best_recipe[i] = a1;
                                            best_recipe[j] = a2;
                                            best_recipe[l] = a3;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        return best_recipe;
    }
    
    // 軽量版2ステップ先読み
    vector<int> find_recipe_with_lookahead(const Paint& current, int color_idx) {
        if (color_idx >= h - 1) return find_best_recipe(current, targets[color_idx]);
        
        vector<int> best_recipe(k, 0);
        double best_combined_score = 1e9;
        
        // 現在色に対する複数の候補をテスト
        vector<vector<int>> candidates;
        
        // 代表的な候補を高速生成
        for (int i = 0; i < k; i++) {
            for (int amount = 1; amount <= 6; amount++) {
                vector<int> recipe(k, 0);
                recipe[i] = amount;
                candidates.push_back(recipe);
            }
        }
        
        for (const auto& recipe : candidates) {
            if (time_up()) break;
            
            // 現在色を作成
            Paint made = current;
            int cost = 0;
            for (int i = 0; i < k; i++) {
                for (int cnt = 0; cnt < recipe[i]; cnt++) {
                    made = made.mix(tubes[i]);
                    cost++;
                }
            }
            
            if (made.amount < EPS) continue;
            
            double current_score = made.dist(targets[color_idx]) * 10000 + max(0, cost - 1) * d;
            
            // 次の色への影響評価（簡易版）
            Paint remaining(made.r, made.g, made.b, max(0.0, made.amount - 1.0));
            double future_penalty = remaining.dist(targets[color_idx + 1]) * 1000; // 軽量化
            
            double combined_score = current_score + future_penalty;
            
            if (combined_score < best_combined_score) {
                best_combined_score = combined_score;
                best_recipe = recipe;
            }
        }
        
        return best_recipe;
    }
    
    void output_palette() {
        // シンプルなパレット（全体を1つのウェル）
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
    }
    
    void solve() {
        start_time = chrono::steady_clock::now();
        read_input();
        output_palette();
        
        Paint current(0, 0, 0, 0);
        
        for (int i = 0; i < h; i++) {
            if (time_up()) {
                // 時間切れの場合は残りを簡単な貪欲で
                for (int j = i; j < h; j++) {
                    cout << "1 0 0 0\n2 0 0\n";
                }
                break;
            }
            
            vector<int> recipe;
            
            // 10色おきに先読み実行（パフォーマンス調整）
            if (i % 10 == 0 && i < h - 1) {
                recipe = find_recipe_with_lookahead(current, i);
            } else {
                recipe = find_best_recipe(current, targets[i]);
            }
            
            // 絵具追加
            for (int j = 0; j < k; j++) {
                for (int cnt = 0; cnt < recipe[j]; cnt++) {
                    cout << "1 0 0 " << j << "\n";
                    current = current.mix(tubes[j]);
                }
            }
            
            // 色取り出し
            cout << "2 0 0\n";
            current.amount = max(0.0, current.amount - 1.0);
            
            if (i % 200 == 0) {
                cerr << "Progress: " << i << "/" << h << ", Time: " 
                     << chrono::duration_cast<chrono::milliseconds>(
                         chrono::steady_clock::now() - start_time).count() << "ms" << endl;
            }
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    FastSolver solver;
    solver.solve();
    
    return 0;
} 