#!/usr/bin/env python3
"""
🎯 実際のソルバー使用機械学習
a2.cppを改造して、実際のスコアで学習
"""

import subprocess
import json
import numpy as np
import time
import os
from concurrent.futures import ThreadPoolExecutor

# 設定
MAX_CASES = 200  # 時間考慮で現実的な数
TIMEOUT_SEC = 15

def create_parameterized_solver():
    """パラメータ化されたソルバーを作成"""
    solver_code = '''
#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint   = modint998244353;
using ll     = long long;
const ll INF = 4e18;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
bool chmin(auto &a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto &a, auto b) { return a < b ? a = b, 1 : 0; }

// グローバルパラメータ（コマンドライン引数で設定）
int MAX_CA = 7, MAX_CB = 5, MAX_CC = 3;

struct Paint {
    double red, green, blue;
    ll count = 1;
    Paint(double r = 0, double g = 0, double b = 0, ll c = 0) : red(r), green(g), blue(b), count(c) {}
};

int main(int argc, char* argv[]) {
    // パラメータ設定
    if (argc >= 4) {
        MAX_CA = atoi(argv[1]);
        MAX_CB = atoi(argv[2]);
        MAX_CC = atoi(argv[3]);
    }
    
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, k, h, t, d;
    cin >> n >> k >> h >> t >> d;

    vector<Paint> own_color(k, Paint());
    vector<Paint> target_color(h, Paint());

    rep(i, k) cin >> own_color[i].red >> own_color[i].green >> own_color[i].blue;
    rep(i, h) cin >> target_color[i].red >> target_color[i].green >> target_color[i].blue;

    auto squ = [](double x) { return x * x; };

    rep(ii, n) {
        rep(i, n - 1) cout << 0 << " ";
        cout << endl;
    }
    rep(ii, n - 1) {
        rep(i, n) cout << 0 << " ";
        cout << endl;
    }

    ll c = (t / h) / 2;
    Paint now_color(0, 0, 0, 0);
    
    double total_error = 0;

    rep(hi, h) {
        Paint &tr = target_color[hi];
        double error = 101010;

        ll usecount_a = 0, usecount_b = 0, usecount_c = 0;
        ll use_id1 = 0, use_id2 = 1, use_id3 = 1, dis_count = 0;
        Paint new_color = Paint(0, 0, 0, 0);

        ll qq = now_color.count / 4;
        ll rr = now_color.count % 4;
        
        rep(c1, k) reps(c2, c1 + 1, k) reps(c3, c2 + 1, k) {
            Paint &color1 = own_color[c1];
            Paint &color2 = own_color[c2];
            Paint &color3 = own_color[c3];

            ll use_now_color = -qq;
            rep(dis, 4) {
                use_now_color += (dis < rr ? qq + 1 : qq);

                rep(ca, MAX_CA) rep(cb, MAX_CB) rep(cc, MAX_CC) {
                    if (ca + cb + cc > c) continue;
                    if (ca + cb + cc + use_now_color < 1) continue;

                    ll sum = ca + cb + cc + use_now_color;

                    double r = (color1.red * ca + color2.red * cb + color3.red * cc +
                                now_color.red * use_now_color) / sum;
                    double g = (color1.green * ca + color2.green * cb + color3.green * cc +
                                now_color.green * use_now_color) / sum;
                    double b = (color1.blue * ca + color2.blue * cb + color3.blue * cc +
                                now_color.blue * use_now_color) / sum;

                    double n_error =
                        sqrt(squ(r - tr.red) + squ(g - tr.green) + squ(b - tr.blue)) * 10000 +
                        (ca + cb + cc) * d;

                    if (chmin(error, n_error)) {
                        usecount_a = ca; usecount_b = cb; usecount_c = cc;
                        use_id1 = c1; use_id2 = c2; use_id3 = c3;
                        new_color.red = r; new_color.green = g; new_color.blue = b;
                        new_color.count = sum; dis_count = now_color.count - use_now_color;
                    }
                }
            }
        }
        
        total_error += error;
        now_color = new_color;

        rep(i, dis_count) { cout << 3 << " " << 0 << " " << 0 << endl; }
        rep(i, usecount_a) cout << 1 << " " << 0 << " " << 0 << " " << use_id1 << endl;
        rep(i, usecount_b) cout << 1 << " " << 0 << " " << 0 << " " << use_id2 << endl;
        rep(i, usecount_c) cout << 1 << " " << 0 << " " << 0 << " " << use_id3 << endl;

        cout << 2 << " " << 0 << " " << 0 << endl;
        now_color.count--;
    }
    
    // 最後にスコア出力（stderr）
    cerr << "SCORE: " << (100000.0 / (1.0 + total_error)) << endl;
    return 0;
}
'''
    
    with open("solver_parameterized.cpp", "w") as f:
        f.write(solver_code)
    
    compile_cmd = "g++ -O2 -std=c++20 -o solver_parameterized solver_parameterized.cpp"
    result = subprocess.run(compile_cmd, shell=True)
    return result.returncode == 0

def run_test_case(args):
    """テストケース実行"""
    case_id, params = args
    max_ca, max_cb, max_cc = params
    
    try:
        # テストケース生成
        np.random.seed(case_id + 42)
        n = np.random.randint(20, 30)
        k = np.random.randint(10, 20)
        h = np.random.randint(50, 100)
        t = np.random.randint(5000, 15000)
        d = np.random.randint(10, 50)
        
        input_data = f"{n} {k} {h} {t} {d}\\n"
        
        # 所有色
        for _ in range(k):
            r, g, b = np.random.random(3)
            input_data += f"{r:.6f} {g:.6f} {b:.6f}\\n"
        
        # 目標色
        for _ in range(h):
            r, g, b = np.random.random(3)
            input_data += f"{r:.6f} {g:.6f} {b:.6f}\\n"
        
        # ソルバー実行
        cmd = f"timeout {TIMEOUT_SEC}s ./solver_parameterized {max_ca} {max_cb} {max_cc}"
        result = subprocess.run(cmd, shell=True, input=input_data, 
                              capture_output=True, text=True, timeout=TIMEOUT_SEC + 5)
        
        if result.returncode == 0:
            # スコア抽出
            for line in result.stderr.split('\\n'):
                if line.startswith('SCORE:'):
                    score = float(line.split(':')[1].strip())
                    return (case_id, n, k, h, t, d, max_ca, max_cb, max_cc, score)
        return None
        
    except Exception as e:
        return None

def main():
    print("🎯 実際のソルバーで機械学習開始！")
    
    # パラメータ化ソルバー作成
    if not create_parameterized_solver():
        print("❌ ソルバー作成失敗")
        return
    
    print("✅ パラメータ化ソルバー作成完了")
    
    # パラメータ空間定義
    param_combinations = []
    for ca in [5, 6, 7, 8, 9]:      # MAX_CA
        for cb in [3, 4, 5, 6, 7]:   # MAX_CB
            for cc in [2, 3, 4, 5]:   # MAX_CC
                param_combinations.append((ca, cb, cc))
    
    print(f"🔍 {len(param_combinations)}種類のパラメータで{MAX_CASES}ケース評価")
    
    # 全タスク生成
    all_tasks = []
    for case_id in range(MAX_CASES):
        for params in param_combinations:
            all_tasks.append((case_id * 1000 + len(all_tasks), params))
    
    print(f"💪 {len(all_tasks)}個のタスクを並列実行...")
    
    # 並列実行
    results = []
    start_time = time.time()
    
    with ThreadPoolExecutor(max_workers=4) as executor:
        futures = [executor.submit(run_test_case, task) for task in all_tasks]
        
        for i, future in enumerate(futures):
            try:
                result = future.result(timeout=TIMEOUT_SEC + 10)
                if result is not None:
                    results.append(result)
                
                # 進捗表示
                if (i + 1) % 50 == 0:
                    elapsed = time.time() - start_time
                    success_rate = len(results) / (i + 1) * 100
                    print(f"⏱️  進捗: {i+1}/{len(all_tasks)} ({elapsed:.1f}s, 成功率: {success_rate:.1f}%)")
                    
            except Exception as e:
                continue
    
    print(f"✅ {len(results)}個の有効データ収集完了！")
    
    if len(results) < 50:
        print("❌ データ不足、元版を使用")
        return
    
    # データ分析
    data = np.array([[r[1], r[2], r[3], r[4], r[5]] for r in results])  # n,k,h,t,d
    params = np.array([[r[6], r[7], r[8]] for r in results])  # max_ca, max_cb, max_cc
    scores = np.array([r[9] for r in results])
    
    # 最適パラメータ抽出（スコア上位の頻度分析）
    top_indices = np.argsort(scores)[-len(scores)//4:]  # 上位25%
    top_params = params[top_indices]
    
    optimal_ca = int(np.median(top_params[:, 0]))
    optimal_cb = int(np.median(top_params[:, 1]))
    optimal_cc = int(np.median(top_params[:, 2]))
    
    # 結果保存
    result_data = {
        'optimal_params': {
            'MAX_CA': optimal_ca,
            'MAX_CB': optimal_cb,
            'MAX_CC': optimal_cc
        },
        'stats': {
            'total_results': len(results),
            'avg_score': float(np.mean(scores)),
            'best_score': float(np.max(scores)),
            'training_time': time.time() - start_time
        }
    }
    
    with open('real_training_results.json', 'w') as f:
        json.dump(result_data, f, indent=2)
    
    print("🎉 実データ機械学習完了！")
    print(f"📈 最適パラメータ: CA={optimal_ca}, CB={optimal_cb}, CC={optimal_cc}")
    print(f"📊 平均スコア: {np.mean(scores):.2f}, 最高スコア: {np.max(scores):.2f}")
    print(f"⏱️  実行時間: {time.time() - start_time:.1f}秒")
    
    # クリーンアップ
    for f in ["solver_parameterized", "solver_parameterized.cpp"]:
        if os.path.exists(f):
            os.remove(f)

if __name__ == "__main__":
    main() 