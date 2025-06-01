#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using ll = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
#define spa " "
#define el '\n'

struct Paint {
    float red, green, blue;
    ll count = 1;
    Paint(float r = 0, float g = 0, float b = 0, ll c = 0) : red(r), green(g), blue(b), count(c) {}
};

// 高速化のための事前計算テーブル
struct FastColorMixer {
    vector<Paint> own_colors;
    vector<Paint> targets;
    ll k, h, c, d;

    // 色差計算の高速化用LUT
    vector<vector<float>> color_dist_cache;

    FastColorMixer(const vector<Paint> &own, const vector<Paint> &tgt, ll _c, ll _d)
        : own_colors(own), targets(tgt), k(own.size()), h(tgt.size()), c(_c), d(_d) {

        // 色距離キャッシュの事前計算
        color_dist_cache.assign(k, vector<float>(h));
        rep(i, k) rep(j, h) {
            float dr               = own_colors[i].red - targets[j].red;
            float dg               = own_colors[i].green - targets[j].green;
            float db               = own_colors[i].blue - targets[j].blue;
            color_dist_cache[i][j] = dr * dr + dg * dg + db * db;
        }
    }

    // 高速な色混合計算
    inline float calc_mixed_error(ll c1, ll c2, ll c3, ll ca, ll cb, ll cc, const Paint &now_color,
                                  ll use_now, ll target_idx) const {
        ll sum = ca + cb + cc + use_now;
        if (sum == 0) return 1e9f;

        float inv_sum = 1.0f / sum;
        float r       = (own_colors[c1].red * ca + own_colors[c2].red * cb + own_colors[c3].red * cc +
                   now_color.red * use_now) *
                  inv_sum;
        float g = (own_colors[c1].green * ca + own_colors[c2].green * cb + own_colors[c3].green * cc +
                   now_color.green * use_now) *
                  inv_sum;
        float b = (own_colors[c1].blue * ca + own_colors[c2].blue * cb + own_colors[c3].blue * cc +
                   now_color.blue * use_now) *
                  inv_sum;

        const Paint &target = targets[target_idx];
        float dr = r - target.red, dg = g - target.green, db = b - target.blue;
        float color_error  = sqrtf(dr * dr + dg * dg + db * db) * 10000.0f;
        float cost_penalty = (ca + cb + cc + now_color.count - use_now) * d;

        return color_error + cost_penalty;
    }

    // 最適な組み合わせを高速検索
    struct BestMix {
        ll c1, c2, c3, ca, cb, cc, use_now, dis_count;
        Paint new_color;
        float error;
    };

    BestMix find_best_mix(const Paint &now_color, ll target_idx) const {
        BestMix best;
        best.error     = 1e9f;
        best.new_color = Paint(0, 0, 0, 0);

        ll qq = now_color.count / 6;
        ll rr = now_color.count % 6;

        // 色の類似度でソートして効率化
        vector<pair<float, ll>> color_similarity;
        rep(i, k) { color_similarity.emplace_back(color_dist_cache[i][target_idx], i); }
        sort(color_similarity.begin(), color_similarity.end());

        // 上位の色のみを使用（大幅な高速化）
        ll search_limit = min(k, 8LL); // 最大8色まで検索

        rep(i1, search_limit) {
            ll c1 = color_similarity[i1].second;

            reps(i2, i1, search_limit) {
                ll c2 = color_similarity[i2].second;

                reps(i3, i2, search_limit) {
                    ll c3 = color_similarity[i3].second;

                    // 現在の色の使用パターンを最適化
                    rep(dis, min(6LL, now_color.count + 1)) {
                        ll use_now_color = (dis < rr ? qq + 1 : qq);

                        // ループの範囲を制限して高速化
                        rep(ca, min(4LL, c + 1)) {
                            ll cb_max = min(4LL, c + 1 - ca);
                            if (cb_max < 0) break;

                            rep(cb, cb_max) {
                                ll cc_max = min(3LL, c - ca - cb);
                                if (cc_max < 0) break;

                                rep(cc, cc_max + 1) {
                                    if (ca + cb + cc + use_now_color < 1) continue;

                                    float error = calc_mixed_error(c1, c2, c3, ca, cb, cc, now_color,
                                                                   use_now_color, target_idx);

                                    // 早期終了で高速化
                                    if (error >= best.error * 1.05f) continue;

                                    if (error < best.error) {
                                        best.error     = error;
                                        best.c1        = c1;
                                        best.c2        = c2;
                                        best.c3        = c3;
                                        best.ca        = ca;
                                        best.cb        = cb;
                                        best.cc        = cc;
                                        best.use_now   = use_now_color;
                                        best.dis_count = now_color.count - use_now_color;

                                        ll sum         = ca + cb + cc + use_now_color;
                                        float inv_sum  = 1.0f / sum;
                                        best.new_color = Paint(
                                            (own_colors[c1].red * ca + own_colors[c2].red * cb +
                                             own_colors[c3].red * cc + now_color.red * use_now_color) *
                                                inv_sum,
                                            (own_colors[c1].green * ca + own_colors[c2].green * cb +
                                             own_colors[c3].green * cc +
                                             now_color.green * use_now_color) *
                                                inv_sum,
                                            (own_colors[c1].blue * ca + own_colors[c2].blue * cb +
                                             own_colors[c3].blue * cc + now_color.blue * use_now_color) *
                                                inv_sum,
                                            sum);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return best;
    }
};

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    ll n, k, h, t, d;
    cin >> n >> k >> h >> t >> d;

    vector<Paint> own_color(k);
    vector<Paint> target_color(h);

    rep(i, k) {
        double r, g, b;
        cin >> r >> g >> b;
        own_color[i] = Paint(r, g, b);
    }
    rep(i, h) {
        double r, g, b;
        cin >> r >> g >> b;
        target_color[i] = Paint(r, g, b);
    }

    // 初期出力
    rep(ii, n) {
        rep(i, n - 1) cout << 0 << " ";
        cout << el;
    }
    rep(ii, n - 1) {
        rep(i, n) cout << 0 << " ";
        cout << el;
    }

    ll c = (t / h) / 2;

    // 高速化エンジンを初期化
    FastColorMixer mixer(own_color, target_color, c, d);

    Paint now_color(0, 0, 0, 0);

    rep(hi, h) {
        auto best = mixer.find_best_mix(now_color, hi);

        now_color = best.new_color;

        // 出力
        rep(i, best.dis_count) { cout << 3 << spa << 0 << spa << 0 << el; }
        rep(i, best.ca) cout << 1 << spa << 0 << spa << 0 << spa << best.c1 << el;
        rep(i, best.cb) cout << 1 << spa << 0 << spa << 0 << spa << best.c2 << el;
        rep(i, best.cc) cout << 1 << spa << 0 << spa << 0 << spa << best.c3 << el;

        cout << 2 << spa << 0 << spa << 0 << el;
        now_color.count--;
    }

    return 0;
}