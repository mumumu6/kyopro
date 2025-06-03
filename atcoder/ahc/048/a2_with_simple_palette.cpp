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
#define ft first
#define sd second
#define spa " "
#define el '\n'

struct Paint {
    double red, green, blue;
    ll count = 1;
    Paint(double r = 0, double g = 0, double b = 0, ll c = 0) : red(r), green(g), blue(b), count(c) {}
};

int main() {
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

    // **改善されたパレット設計: 3つのエリアに分割**
    // エリア1 (0,0)-(n/2-1, n-1): メイン混色エリア
    // エリア2 (n/2, 0)-(n-1, n/2-1): サブ混色エリア  
    // エリア3 (n/2, n/2)-(n-1, n-1): 一時保管エリア
    
    rep(ii, n) {
        rep(i, n - 1) {
            if (i == n/2 - 1) {
                cout << 1 << " ";  // 縦方向の分割
            } else {
                cout << 0 << " ";
            }
        }
        cout << el;
    }
    
    rep(ii, n - 1) {
        rep(i, n) {
            if (ii == n/2 - 1 && i >= n/2) {
                cout << 1 << " ";  // 横方向の分割（右下エリアのみ）
            } else {
                cout << 0 << " ";
            }
        }
        cout << el;
    }

    ll c = (t / h) / 2;  // 元の予算設定を維持
    Paint main_color(0, 0, 0, 0);    // メインエリアの色
    Paint sub_color(0, 0, 0, 0);     // サブエリアの色

    rep(hi, h) {
        Paint &tr = target_color[hi];
        double error = 101010;

        ll usecount_a = 0, usecount_b = 0, usecount_c = 0;
        ll use_id1 = 0, use_id2 = 1, use_id3 = 1, dis_count = 0;
        Paint new_color = Paint(0, 0, 0, 0);
        bool use_sub_area = false;

        // メインエリアでの最適解を探索
        ll qq = main_color.count / 4;
        ll rr = main_color.count % 4;
        
        rep(c1, k) reps(c2, c1 + 1, k) reps(c3, c2 + 1, k) {
            Paint &color1 = own_color[c1];
            Paint &color2 = own_color[c2];
            Paint &color3 = own_color[c3];

            ll use_now_color = -qq;
            rep(dis, 4) {
                use_now_color += (dis < rr ? qq + 1 : qq);

                rep(ca, 7) rep(cb, 5) rep(cc, 3) {
                    if (ca + cb + cc > c) continue;
                    if (ca + cb + cc + use_now_color < 1) continue;

                    ll sum = ca + cb + cc + use_now_color;

                    double r = (color1.red * ca + color2.red * cb + color3.red * cc +
                                main_color.red * use_now_color) / sum;
                    double g = (color1.green * ca + color2.green * cb + color3.green * cc +
                                main_color.green * use_now_color) / sum;
                    double b = (color1.blue * ca + color2.blue * cb + color3.blue * cc +
                                main_color.blue * use_now_color) / sum;

                    double n_error = sqrt(squ(r - tr.red) + squ(g - tr.green) + squ(b - tr.blue)) * 10000 + 
                                    (ca + cb + cc) * d;

                    if (chmin(error, n_error)) {
                        usecount_a = ca; usecount_b = cb; usecount_c = cc;
                        use_id1 = c1; use_id2 = c2; use_id3 = c3;
                        new_color.red = r; new_color.green = g; new_color.blue = b;
                        new_color.count = sum; dis_count = main_color.count - use_now_color;
                        use_sub_area = false;
                    }
                }
            }
        }

        // サブエリアも使えるなら、そちらでも試してみる
        if (sub_color.count > 0) {
            ll qq_sub = sub_color.count / 4;
            ll rr_sub = sub_color.count % 4;
            
            rep(c1, k) reps(c2, c1 + 1, k) reps(c3, c2 + 1, k) {
                Paint &color1 = own_color[c1];
                Paint &color2 = own_color[c2];
                Paint &color3 = own_color[c3];

                ll use_sub_color = -qq_sub;
                rep(dis, 4) {
                    use_sub_color += (dis < rr_sub ? qq_sub + 1 : qq_sub);

                    rep(ca, 5) rep(cb, 3) rep(cc, 2) {  // サブエリアは控えめに
                        if (ca + cb + cc > c/2) continue;
                        if (ca + cb + cc + use_sub_color < 1) continue;

                        ll sum = ca + cb + cc + use_sub_color;

                        double r = (color1.red * ca + color2.red * cb + color3.red * cc +
                                    sub_color.red * use_sub_color) / sum;
                        double g = (color1.green * ca + color2.green * cb + color3.green * cc +
                                    sub_color.green * use_sub_color) / sum;
                        double b = (color1.blue * ca + color2.blue * cb + color3.blue * cc +
                                    sub_color.blue * use_sub_color) / sum;

                        double n_error = sqrt(squ(r - tr.red) + squ(g - tr.green) + squ(b - tr.blue)) * 10000 + 
                                        (ca + cb + cc) * d;

                        if (chmin(error, n_error)) {
                            usecount_a = ca; usecount_b = cb; usecount_c = cc;
                            use_id1 = c1; use_id2 = c2; use_id3 = c3;
                            new_color.red = r; new_color.green = g; new_color.blue = b;
                            new_color.count = sum; dis_count = sub_color.count - use_sub_color;
                            use_sub_area = true;
                        }
                    }
                }
            }
        }
        
        // 色の更新
        if (use_sub_area) {
            sub_color = new_color;
        } else {
            main_color = new_color;
        }

        // 出力（エリア指定付き）
        if (use_sub_area) {
            // サブエリア(1,0)を使用
            rep(i, dis_count) { cout << 3 << spa << 1 << spa << 0 << el; }
            rep(i, usecount_a) cout << 1 << spa << 1 << spa << 0 << spa << use_id1 << el;
            rep(i, usecount_b) cout << 1 << spa << 1 << spa << 0 << spa << use_id2 << el;
            rep(i, usecount_c) cout << 1 << spa << 1 << spa << 0 << spa << use_id3 << el;
            cout << 2 << spa << 1 << spa << 0 << el;
            sub_color.count--;
        } else {
            // メインエリア(0,0)を使用
            rep(i, dis_count) { cout << 3 << spa << 0 << spa << 0 << el; }
            rep(i, usecount_a) cout << 1 << spa << 0 << spa << 0 << spa << use_id1 << el;
            rep(i, usecount_b) cout << 1 << spa << 0 << spa << 0 << spa << use_id2 << el;
            rep(i, usecount_c) cout << 1 << spa << 0 << spa << 0 << spa << use_id3 << el;
            cout << 2 << spa << 0 << spa << 0 << el;
            main_color.count--;
        }

        // **エリア間の色移動（10回に1回）**
        if (hi % 10 == 0 && main_color.count > 5 && sub_color.count < 3) {
            // メインからサブに少し移動
            cout << 4 << spa << 2 << spa << 0 << spa << 1 << spa << 0 << el;  // メインから2単位移動
            cout << 4 << spa << 0 << spa << 0 << spa << 0 << spa << 1 << el;  // サブエリアに合流
            
            // 色の更新
            double total = main_color.count + sub_color.count;
            sub_color.red = (main_color.red * 2 + sub_color.red * sub_color.count) / (2 + sub_color.count);
            sub_color.green = (main_color.green * 2 + sub_color.green * sub_color.count) / (2 + sub_color.count);
            sub_color.blue = (main_color.blue * 2 + sub_color.blue * sub_color.count) / (2 + sub_color.count);
            sub_color.count += 2;
            main_color.count -= 2;
        }
    }
} 