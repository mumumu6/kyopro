#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Paint は精度を float にしてキャッシュ効率を狙う例
struct Paint {
    float red, green, blue;
    ll    count;
    Paint(float r=0, float g=0, float b=0, ll c=0)
        : red(r), green(g), blue(b), count(c) {}
};

inline bool chmin(float &a, float b) {
    if (a > b) {
        a = b;
        return true;
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, k, h, t, d;
    cin >> n >> k >> h >> t >> d;

    vector<Paint> own_color(k);
    vector<Paint> target_color(h);
    for (int i = 0; i < k; i++) {
        double r, g, b;
        cin >> r >> g >> b;
        own_color[i] = Paint((float)r, (float)g, (float)b, 0);
    }
    for (int i = 0; i < h; i++) {
        double r, g, b;
        cin >> r >> g >> b;
        target_color[i] = Paint((float)r, (float)g, (float)b, 0);
    }

    // 「最初の n×(n-1) 行を 0 で埋める」出力（必要があればそのまま）
    for (int ii = 0; ii < n; ii++) {
        for (int jj = 0; jj < n - 1; jj++) {
            cout << 0 << " ";
        }
        cout << "\n";
    }
    for (int ii = 0; ii < n; ii++) {
        for (int jj = 0; jj < n - 1; jj++) {
            cout << 0 << " ";
        }
        cout << "\n";
    }

    // 1 ステップあたりに使える色の上限を clamp する例
    ll c = (t / h) / 2;
    c = min<ll>(c, 4);  // たとえば「最大でも 4 本くらいまでしか混ぜない」と制限をかけてみる

    Paint now_color(0, 0, 0, 0);

    // ビームサーチ用のビーム幅
    const int BEAM = 20;

    // tmp_buf：(score, c1, c2, c3, dis, ca, cb, cc) の候補を入れるバッファ
    struct Candidate {
        float score;
        int c1, c2, c3;
        int dis, ca, cb, cc;
        // 小さいスコアが良いと仮定
        bool operator<(const Candidate &other) const {
            return score < other.score;
        }
    };
    vector<Candidate> tmp_buf;
    tmp_buf.reserve(100000);

    for (int hi = 0; hi < h; hi++) {
        // 1) まず target_color[hi] に近い色を上位 M 個だけサンプリングする
        Paint &tr = target_color[hi];
        vector<pair<float,int>> dist_idx(k);
        for (int i = 0; i < k; i++) {
            float dr = own_color[i].red   - tr.red;
            float dg = own_color[i].green - tr.green;
            float db = own_color[i].blue  - tr.blue;
            float dist2 = dr*dr + dg*dg + db*db;
            dist_idx[i] = {dist2, i};
        }
        sort(dist_idx.begin(), dist_idx.end());
        int M = min<ll>(k, 12);  // 上位 12 色だけ
        vector<int> best_idxs(M);
        for (int i = 0; i < M; i++) best_idxs[i] = dist_idx[i].second;

        // 2) now_color.count, qq, rr をあらかじめ求めておく
        ll qq = now_color.count / 6;
        ll rr = now_color.count % 6;

        // 3) ビームサーチ的に「まず一色だけ→次に二色ペア→最後三色トリオ」
        //    の順で候補を作り、都度「上位 BEAM 個」だけ残すアプローチ
        tmp_buf.clear();

        // --- (A) 単色だけを試す ---
        for (int idx = 0; idx < M; idx++) {
            int c1 = best_idxs[idx];
            // dis を絞る例：now_color.count が小さいときは dis=0～ min(rr,2) くらいに絞る
            int max_dis = min<ll>(5, now_color.count + 0);
            for (int dis = 0; dis <= max_dis; dis++) {
                ll use_now_color = (dis < rr ? qq + 1 : qq);

                int ca = 1, cb = 0, cc = 0; // 単色なので ca=1, cb=cc=0
                if (ca + cb + cc + use_now_color < 1) continue;
                if (ca + cb + cc > c) continue;

                ll sum = ca + cb + cc + use_now_color;
                float r = (
                      own_color[c1].red   * ca
                    + now_color.red       * use_now_color
                ) / sum;
                float g = (
                      own_color[c1].green * ca
                    + now_color.green     * use_now_color
                ) / sum;
                float b = (
                      own_color[c1].blue  * ca
                    + now_color.blue      * use_now_color
                ) / sum;

                // ２乗距離で比較
                float dist2 = (r - tr.red)*(r - tr.red)
                            + (g - tr.green)*(g - tr.green)
                            + (b - tr.blue)*(b - tr.blue);
                float score = dist2 * (10000.0f * 10000.0f)
                              + float(ca + cb + cc + now_color.count - use_now_color) * float(d);

                tmp_buf.push_back({score, c1, -1, -1, dis, ca, cb, cc});
            }
        }
        // スコア順に小さいほうから BEAM 個だけプル（大きいものは切り捨て）
        sort(tmp_buf.begin(), tmp_buf.end());
        if ((int)tmp_buf.size() > BEAM) tmp_buf.resize(BEAM);

        // --- (B) 二色ペアだけを試す ---
        vector<Candidate> next_buf;
        next_buf.reserve(BEAM * M);
        for (auto &cand : tmp_buf) {
            // すでに単色 (c1 だけ) で cand を作っている状態をベースに
            // 「今度はもう一色足して評価したい」 → c2 を追加
            int base_c1 = cand.c1;
            // best_idxs からもう一色 c2 を選ぶ例
            for (int x = 0; x < M; x++) {
                int c2 = best_idxs[x];
                if (c2 == base_c1) continue;
                // 色の組み合わせとしては (base_c1, c2)
                // dis, ca, cb, cc を再度小さい範囲で試す
                int max_dis2 = min<ll>(5, now_color.count + 0);
                for (int dis = 0; dis <= max_dis2; dis++) {
                    ll use_now_color = (dis < rr ? qq + 1 : qq);

                    for (int ca = 1; ca <= 2; ca++) { // ca=1～2 くらいに絞る例
                        for (int cb = 1; cb <= 2; cb++) {
                            int cc = 0;
                            if (ca + cb + cc > c) continue;
                            if (ca + cb + cc + use_now_color < 1) continue;

                            ll sum = ca + cb + cc + use_now_color;
                            float r = (
                                  own_color[base_c1].red   * ca
                                + own_color[c2].red        * cb
                                + now_color.red             * use_now_color
                            ) / sum;
                            float g = (
                                  own_color[base_c1].green * ca
                                + own_color[c2].green      * cb
                                + now_color.green           * use_now_color
                            ) / sum;
                            float b = (
                                  own_color[base_c1].blue  * ca
                                + own_color[c2].blue       * cb
                                + now_color.blue            * use_now_color
                            ) / sum;

                            float dist2 = (r - tr.red)*(r - tr.red)
                                        + (g - tr.green)*(g - tr.green)
                                        + (b - tr.blue)*(b - tr.blue);
                            float score = dist2 * (10000.0f * 10000.0f)
                                           + float(ca + cb + cc + now_color.count - use_now_color) * float(d);

                            next_buf.push_back({score, base_c1, c2, -1, dis, ca, cb, cc});
                        }
                    }
                }
            }
        }
        sort(next_buf.begin(), next_buf.end());
        if ((int)next_buf.size() > BEAM) next_buf.resize(BEAM);

        // --- (C) 三色トリオを試す（最後に必要に応じて、ただし候補数は BEAM 個までに留める）---
        vector<Candidate> final_buf;
        final_buf.reserve(BEAM * (M*M));
        for (auto &cand : next_buf) {
            int base_c1 = cand.c1;
            int base_c2 = cand.c2;
            // best_idxs から c3 を選ぶ
            for (int idx = 0; idx < M; idx++) {
                int c3 = best_idxs[idx];
                if (c3 == base_c1 || c3 == base_c2) continue;

                int max_dis3 = min<ll>(5, now_color.count + 0);
                for (int dis = 0; dis <= max_dis3; dis++) {
                    ll use_now_color = (dis < rr ? qq + 1 : qq);

                    // ca,cb,cc を「さらに 2 本以内」などで絞る
                    for (int ca = 1; ca <= 2; ca++) {
                        for (int cb = 1; cb <= 2; cb++) {
                            for (int cc = 1; cc <= 1; cc++) {
                                if (ca + cb + cc > c) continue;
                                if (ca + cb + cc + use_now_color < 1) continue;

                                ll sum = ca + cb + cc + use_now_color;
                                float r = (
                                      own_color[base_c1].red * ca
                                    + own_color[base_c2].red * cb
                                    + own_color[c3].red       * cc
                                    + now_color.red           * use_now_color
                                ) / sum;
                                float g = (
                                      own_color[base_c1].green * ca
                                    + own_color[base_c2].green * cb
                                    + own_color[c3].green       * cc
                                    + now_color.green           * use_now_color
                                ) / sum;
                                float b = (
                                      own_color[base_c1].blue * ca
                                    + own_color[base_c2].blue * cb
                                    + own_color[c3].blue      * cc
                                    + now_color.blue           * use_now_color
                                ) / sum;

                                float dist2 = (r - tr.red)*(r - tr.red)
                                            + (g - tr.green)*(g - tr.green)
                                            + (b - tr.blue)*(b - tr.blue);
                                float score = dist2 * (10000.0f * 10000.0f)
                                               + float(ca + cb + cc + now_color.count - use_now_color) * float(d);

                                final_buf.push_back({score, base_c1, base_c2, c3, dis, ca, cb, cc});
                            }
                        }
                    }
                }
            }
        }
        sort(final_buf.begin(), final_buf.end());
        if ((int)final_buf.size() > BEAM) final_buf.resize(BEAM);

        // これで final_buf[0] が最善候補
        Candidate best = final_buf[0];

        // 出力（dis 本数分 → 3 0 0 を出す、ca→ (1 0 0 c1)、… の順）
        ll qq2 = now_color.count / 6;
        ll rr2 = now_color.count % 6;
        ll use_now_color = (best.dis < rr2 ? qq2 + 1 : qq2);

        ll sum = best.ca + best.cb + best.cc + use_now_color;
        ll dis_count = now_color.count - use_now_color;
        if (dis_count < 0) dis_count = 0;

        for (int i = 0; i < dis_count; i++) {
            cout << 3 << " " << 0 << " " << 0 << "\n";
        }
        for (int i = 0; i < best.ca; i++) {
            cout << 1 << " " << 0 << " " << 0 << " " << best.c1 << "\n";
        }
        if (best.cb > 0) {
            for (int i = 0; i < best.cb; i++) {
                cout << 1 << " " << 0 << " " << 0 << " " << best.c2 << "\n";
            }
        }
        if (best.cc > 0) {
            for (int i = 0; i < best.cc; i++) {
                cout << 1 << " " << 0 << " " << 0 << " " << best.c3 << "\n";
            }
        }
        cout << 2 << " " << 0 << " " << 0 << "\n";

        // now_color の更新（float → double に戻したいなら適宜キャスト）
        now_color.red   = ( (own_color[best.c1].red   * best.ca)
                          + (best.cb>0 ? own_color[best.c2].red   * best.cb   : 0.0f)
                          + (best.cc>0 ? own_color[best.c3].red    * best.cc   : 0.0f)
                          + (now_color.red * use_now_color)
                        ) / sum;
        now_color.green = ( (own_color[best.c1].green * best.ca)
                          + (best.cb>0 ? own_color[best.c2].green * best.cb   : 0.0f)
                          + (best.cc>0 ? own_color[best.c3].green  * best.cc   : 0.0f)
                          + (now_color.green * use_now_color)
                        ) / sum;
        now_color.blue  = ( (own_color[best.c1].blue  * best.ca)
                          + (best.cb>0 ? own_color[best.c2].blue  * best.cb   : 0.0f)
                          + (best.cc>0 ? own_color[best.c3].blue   * best.cc   : 0.0f)
                          + (now_color.blue * use_now_color)
                        ) / sum;
        now_color.count = sum - 1;
    }

    return 0;
}
