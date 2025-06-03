#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const double EPS = 1e-9;

struct Paint {
    double red, green, blue, count;
    Paint(double r = 0, double g = 0, double b = 0, double c = 0) 
        : red(r), green(g), blue(b), count(c) {}
    
    Paint mix_with(const Paint& other, double ratio) const {
        if (count < EPS && other.count < EPS) return Paint();
        double total = count + other.count * ratio;
        return Paint(
            (red * count + other.red * other.count * ratio) / total,
            (green * count + other.green * other.count * ratio) / total,
            (blue * count + other.blue * other.count * ratio) / total,
            total
        );
    }
    
    double distance(const Paint& target) const {
        return sqrt((red - target.red) * (red - target.red) + 
                   (green - target.green) * (green - target.green) + 
                   (blue - target.blue) * (blue - target.blue));
    }
};

struct ColorAnalysis {
    vector<vector<int>> similar_groups;
    vector<Paint> group_centers;
    map<int, int> color_to_group;
    
    void analyze(const vector<Paint>& targets, double threshold = 0.1) {
        int n = targets.size();
        vector<bool> used(n, false);
        
        for (int i = 0; i < n; i++) {
            if (used[i]) continue;
            
            vector<int> group = {i};
            used[i] = true;
            
            for (int j = i + 1; j < n; j++) {
                if (!used[j] && targets[i].distance(targets[j]) < threshold) {
                    group.push_back(j);
                    used[j] = true;
                }
            }
            
            similar_groups.push_back(group);
            
            // グループの中心色を計算
            Paint center(0, 0, 0, 0);
            for (int idx : group) {
                center.red += targets[idx].red;
                center.green += targets[idx].green; 
                center.blue += targets[idx].blue;
                color_to_group[idx] = similar_groups.size() - 1;
            }
            center.red /= group.size();
            center.green /= group.size();
            center.blue /= group.size();
            center.count = 1.0;
            
            group_centers.push_back(center);
        }
    }
};

struct AdvancedSolver {
    int n, k, h, t, d;
    vector<Paint> own_colors;
    vector<Paint> targets;
    ColorAnalysis analysis;
    
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
    
    // 線形計画法で最適な混合比を求める（簡易版）
    vector<double> find_optimal_mix(const Paint& target) {
        vector<double> best_mix(k, 0.0);
        double best_error = 1e9;
        
        // より細かい探索
        function<void(int, vector<double>&, double)> dfs = [&](int pos, vector<double>& current, double sum) {
            if (pos == k) {
                if (sum < EPS) return;
                
                Paint result(0, 0, 0, 0);
                for (int i = 0; i < k; i++) {
                    if (current[i] > EPS) {
                        result = result.mix_with(own_colors[i], current[i] / sum);
                    }
                }
                
                double error = result.distance(target) + sum * 0.001; // コスト考慮
                if (error < best_error) {
                    best_error = error;
                    best_mix = current;
                }
                return;
            }
            
            for (int amount = 0; amount <= 20 && sum + amount <= 30; amount++) {
                current[pos] = amount;
                dfs(pos + 1, current, sum + amount);
            }
        };
        
        vector<double> current(k, 0.0);
        dfs(0, current, 0.0);
        
        return best_mix;
    }
    
    void output_palette_setup() {
        // より効率的なパレット設定
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n - 1; j++) {
                cout << ((j < 3) ? 1 : 0) << " ";
            }
            cout << "\n";
        }
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n; j++) {
                cout << ((i < 3) ? 1 : 0) << " ";
            }
            cout << "\n";
        }
    }
    
    void solve() {
        read_input();
        
        // 色の分析
        analysis.analyze(targets);
        
        output_palette_setup();
        
        Paint current_color(0, 0, 0, 0);
        map<int, Paint> group_bases; // グループごとの基準色
        
        for (int i = 0; i < h; i++) {
            int group_id = analysis.color_to_group[i];
            
            // グループ基準色の作成または調整
            if (group_bases.find(group_id) == group_bases.end()) {
                // 新しいグループ - 基準色を作成
                vector<double> mix = find_optimal_mix(analysis.group_centers[group_id]);
                
                // 現在の絵具を廃棄
                if (current_color.count > EPS) {
                    cout << "3 0 0\n";
                    current_color = Paint(0, 0, 0, 0);
                }
                
                // 基準色を作成
                Paint new_base(0, 0, 0, 0);
                for (int j = 0; j < k; j++) {
                    for (int cnt = 0; cnt < (int)mix[j]; cnt++) {
                        cout << "1 0 0 " << j << "\n";
                        new_base = new_base.mix_with(own_colors[j], 1.0);
                    }
                }
                
                group_bases[group_id] = new_base;
                current_color = new_base;
            }
            
            // 目標色に向けて微調整
            Paint target = targets[i];
            vector<double> adjustment = find_optimal_mix(target);
            
            // 調整用の絵具を追加
            for (int j = 0; j < k; j++) {
                int add_amount = min(3, (int)adjustment[j]); // 少量調整
                for (int cnt = 0; cnt < add_amount; cnt++) {
                    cout << "1 1 0 " << j << "\n";
                    current_color = current_color.mix_with(own_colors[j], 1.0);
                }
            }
            
            // 色を取り出し
            cout << "2 0 0\n";
            current_color.count = max(0.0, current_color.count - 1.0);
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << fixed << setprecision(15);
    
    AdvancedSolver solver;
    solver.solve();
    
    return 0;
} 