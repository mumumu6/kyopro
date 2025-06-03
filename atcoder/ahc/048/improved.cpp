#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const double EPS = 1e-9;

struct Paint {
    double red, green, blue, count;
    Paint(double r = 0, double g = 0, double b = 0, double c = 0) 
        : red(r), green(g), blue(b), count(c) {}
    
    Paint operator+(const Paint& other) const {
        if (count + other.count < EPS) return Paint();
        double total = count + other.count;
        return Paint(
            (red * count + other.red * other.count) / total,
            (green * count + other.green * other.count) / total,
            (blue * count + other.blue * other.count) / total,
            total
        );
    }
    
    double distance(const Paint& target) const {
        return sqrt((red - target.red) * (red - target.red) + 
                   (green - target.green) * (green - target.green) + 
                   (blue - target.blue) * (blue - target.blue));
    }
};

struct State {
    Paint current_color;
    vector<pair<int, int>> operations; // color_id, amount
    double total_cost;
    
    State() : total_cost(0) {}
    
    double evaluate(const Paint& target, int d) const {
        return current_color.distance(target) * 10000 + total_cost * d;
    }
};

struct Solver {
    int n, k, h, t, d;
    vector<Paint> own_colors;
    vector<Paint> targets;
    
    void read_input() {
        cin >> n >> k >> h >> t >> d;
        own_colors.resize(k);
        targets.resize(h);
        
        for (int i = 0; i < k; i++) {
            cin >> own_colors[i].red >> own_colors[i].green >> own_colors[i].blue;
        }
        for (int i = 0; i < h; i++) {
            cin >> targets[i].red >> targets[i].green >> targets[i].blue;
        }
    }
    
    // 改善された探索: より広い範囲とビームサーチ
    State find_best_mix(const Paint& current, const Paint& target, int budget) {
        vector<State> beam;
        beam.push_back(State());
        beam[0].current_color = current;
        
        const int beam_width = 50;
        const int max_depth = min(budget / 3, 10);
        
        for (int depth = 0; depth < max_depth; depth++) {
            vector<State> next_beam;
            
            for (const auto& state : beam) {
                // 3色の組み合わせを試す（範囲を拡大）
                for (int c1 = 0; c1 < k; c1++) {
                    for (int c2 = c1; c2 < k; c2++) {
                        for (int c3 = c2; c3 < k; c3++) {
                            // より細かい比率で探索
                            for (int a1 = 0; a1 <= 10; a1++) {
                                for (int a2 = 0; a2 <= 10; a2++) {
                                    for (int a3 = 0; a3 <= 10; a3++) {
                                        if (a1 + a2 + a3 == 0) continue;
                                        if (a1 + a2 + a3 > budget - state.total_cost) continue;
                                        
                                        State new_state = state;
                                        Paint mix = state.current_color;
                                        
                                        if (a1 > 0) {
                                            Paint add(own_colors[c1].red, own_colors[c1].green, 
                                                     own_colors[c1].blue, a1);
                                            mix = mix + add;
                                            new_state.total_cost += a1;
                                        }
                                        if (a2 > 0) {
                                            Paint add(own_colors[c2].red, own_colors[c2].green, 
                                                     own_colors[c2].blue, a2);
                                            mix = mix + add;
                                            new_state.total_cost += a2;
                                        }
                                        if (a3 > 0) {
                                            Paint add(own_colors[c3].red, own_colors[c3].green, 
                                                     own_colors[c3].blue, a3);
                                            mix = mix + add;
                                            new_state.total_cost += a3;
                                        }
                                        
                                        new_state.current_color = mix;
                                        next_beam.push_back(new_state);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            // 評価して上位beam_width個を選択
            sort(next_beam.begin(), next_beam.end(), 
                 [&](const State& a, const State& b) {
                     return a.evaluate(target, d) < b.evaluate(target, d);
                 });
            
            if (next_beam.size() > beam_width) {
                next_beam.resize(beam_width);
            }
            
            beam = next_beam;
            if (beam.empty()) break;
        }
        
        return beam.empty() ? State() : beam[0];
    }
    
    void output_palette_setup() {
        // シンプルなパレット設定
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n - 1; j++) {
                cout << (j == 0 ? 1 : 0) << " ";
            }
            cout << "\n";
        }
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n; j++) {
                cout << (i == 0 ? 1 : 0) << " ";
            }
            cout << "\n";
        }
    }
    
    void solve() {
        read_input();
        output_palette_setup();
        
        Paint current_color(0, 0, 0, 0);
        int budget_per_color = t / h;
        
        for (int i = 0; i < h; i++) {
            State best = find_best_mix(current_color, targets[i], budget_per_color);
            
            // 操作を出力（簡略化版）
            for (auto& op : best.operations) {
                cout << "1 0 0 " << op.first << "\n";
            }
            cout << "2 0 0\n";
            
            // 残り絵具を計算（簡略化）
            current_color = best.current_color;
            current_color.count = max(0.0, current_color.count - 1.0);
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << fixed << setprecision(15);
    
    Solver solver;
    solver.solve();
    
    return 0;
} 