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

struct Solution {
    vector<vector<int>> recipe; // recipe[i] = チューブごとの使用量 for color i
    double score;
    
    Solution(int h, int k) : recipe(h, vector<int>(k, 0)), score(1e9) {}
    
    Solution copy() const {
        Solution ret(recipe.size(), recipe[0].size());
        ret.recipe = recipe;
        ret.score = score;
        return ret;
    }
};

struct SASolver {
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
    
    // 解の評価
    double evaluate(const Solution& sol) {
        double total_error = 0;
        int total_cost = 0;
        Paint current(0, 0, 0, 0);
        
        for (int i = 0; i < h; i++) {
            // 現在の色から目標色i作成
            Paint made = current;
            int used = 0;
            
            for (int j = 0; j < k; j++) {
                for (int cnt = 0; cnt < sol.recipe[i][j]; cnt++) {
                    made = made.mix(tubes[j]);
                    used++;
                }
            }
            
            total_cost += used;
            
            // 色の誤差計算
            if (made.amount > EPS) {
                total_error += made.dist(targets[i]);
                // 残りの絵具を計算
                current = Paint(made.r, made.g, made.b, max(0.0, made.amount - 1.0));
            } else {
                total_error += 1.0; // 色が作れない場合のペナルティ
            }
        }
        
        return total_error * 10000 + max(0, total_cost - h) * d;
    }
    
    // 近傍解生成
    Solution get_neighbor(const Solution& current) {
        Solution neighbor = current.copy();
        
        // ランダムに色を選択
        int target_idx = rand() % h;
        
        // ランダムに操作を選択
        int op = rand() % 4;
        
        if (op == 0) {
            // チューブの使用量を増やす
            int tube_idx = rand() % k;
            if (neighbor.recipe[target_idx][tube_idx] < 20) {
                neighbor.recipe[target_idx][tube_idx]++;
            }
        } else if (op == 1) {
            // チューブの使用量を減らす
            int tube_idx = rand() % k;
            if (neighbor.recipe[target_idx][tube_idx] > 0) {
                neighbor.recipe[target_idx][tube_idx]--;
            }
        } else if (op == 2) {
            // 2つの色のレシピを部分的に交換
            if (h > 1) {
                int other_idx = rand() % h;
                if (other_idx == target_idx) other_idx = (other_idx + 1) % h;
                
                int tube_idx = rand() % k;
                swap(neighbor.recipe[target_idx][tube_idx], neighbor.recipe[other_idx][tube_idx]);
            }
        } else {
            // 全体的な調整
            for (int j = 0; j < k; j++) {
                if (rand() % 10 == 0) {
                    int delta = (rand() % 3) - 1; // -1, 0, 1
                    neighbor.recipe[target_idx][j] = max(0, neighbor.recipe[target_idx][j] + delta);
                }
            }
        }
        
        neighbor.score = evaluate(neighbor);
        return neighbor;
    }
    
    // 初期解生成（改善された貪欲）
    Solution generate_initial() {
        Solution sol(h, k);
        
        for (int i = 0; i < h; i++) {
            double best_error = 1e9;
            vector<int> best_recipe(k, 0);
            
            // より広範囲で探索
            for (int total = 1; total <= 15; total++) {
                function<void(int, int, vector<int>&)> dfs = [&](int pos, int remaining, vector<int>& current) {
                    if (pos == k) {
                        if (remaining == 0) {
                            Paint made(0, 0, 0, 0);
                            for (int j = 0; j < k; j++) {
                                for (int cnt = 0; cnt < current[j]; cnt++) {
                                    made = made.mix(tubes[j]);
                                }
                            }
                            
                            if (made.amount > EPS) {
                                double error = made.dist(targets[i]) + total * 0.001;
                                if (error < best_error) {
                                    best_error = error;
                                    best_recipe = current;
                                }
                            }
                        }
                        return;
                    }
                    
                    for (int use = 0; use <= remaining; use++) {
                        current[pos] = use;
                        dfs(pos + 1, remaining - use, current);
                    }
                };
                
                vector<int> current(k, 0);
                dfs(0, total, current);
            }
            
            sol.recipe[i] = best_recipe;
        }
        
        sol.score = evaluate(sol);
        return sol;
    }
    
    void output_solution(const Solution& sol) {
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
        for (int i = 0; i < h; i++) {
            // 絵具を追加
            for (int j = 0; j < k; j++) {
                for (int cnt = 0; cnt < sol.recipe[i][j]; cnt++) {
                    cout << "1 0 0 " << j << "\n";
                }
            }
            // 色を取り出し
            cout << "2 0 0\n";
        }
    }
    
    void solve() {
        read_input();
        
        // 焼きなまし法のパラメータ
        const int MAX_ITER = 100000;
        const double T_START = 100.0;
        const double T_END = 0.01;
        
        // 初期解生成
        Solution current = generate_initial();
        Solution best = current.copy();
        
        for (int iter = 0; iter < MAX_ITER; iter++) {
            double t = T_START * pow(T_END / T_START, (double)iter / MAX_ITER);
            
            Solution neighbor = get_neighbor(current);
            
            double delta = neighbor.score - current.score;
            
            if (delta < 0 || exp(-delta / t) > (double)rand() / RAND_MAX) {
                current = neighbor;
                
                if (current.score < best.score) {
                    best = current.copy();
                }
            }
            
            if (iter % 10000 == 0) {
                cerr << "Iter: " << iter << ", Best: " << best.score << ", Current: " << current.score << endl;
            }
        }
        
        output_solution(best);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    srand(time(nullptr));
    
    SASolver solver;
    solver.solve();
    
    return 0;
} 