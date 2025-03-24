#include <bits/stdc++.h>
using namespace std;
#include <atcoder/all>
using namespace atcoder;
using mint = modint998244353;
using ll   = long long;
#define rep(i, n) for (ll i = 0; i < (n); i++)
#define reps(i, a, b) for (ll i = (a); i < (b); i++)
bool chmin(auto &a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto &a, auto b) { return a < b ? a = b, 1 : 0; }
vector<ll> sin45 = {0, 1, 1, 1, 0, -1, -1, -1}, cos45 = {1, 1, 0, -1, -1, -1, 0, 1};
vector<ll> dx = {0, 1, 0, -1};
vector<ll> dy = {1, 0, -1, 0};
#define ft first
#define sd second
#define all(x) begin(x), end(x)
#define mp(a, b) make_pair(a, b)
#define pii pair<int, int>
#define pll pair<ll, ll>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>

struct Point {
    int x;
    int y;

    Point(int x, int y) : x(x), y(y) {}

    /// @brief 2点間のマンハッタン距離を計算する
    /// @param p 距離を計算する点
    /// @return 2点間のマンハッタン距離
    int dist(const Point &p) const { return abs(x - p.x) + abs(y - p.y); }
};

/// @brief 入力データを表す構造体
struct Input {
    /// @brief レストランの数 (=1000)
    int order_count;
    /// @brief 選択する必要のある注文の数 (=50)
    int pickup_count;
    /// @brief AtCoderオフィスの座標 (=(400, 400))
    Point office;
    /// @brief レストランの座標の配列
    vector<Point> restaurants;
    /// @brief 目的地の座標の配列
    vector<Point> destinations;

    /// @brief 入力データを読み込む
    /// @return 読み込んだ入力データ
    static Input read() {
        int order_count  = 1000;
        int pickup_count = 50;
        Point office(400, 400);
        vector<Point> restaurants;
        vector<Point> destinations;

        for (int i = 0; i < order_count; ++i) {
            int a, b, c, d;
            cin >> a >> b >> c >> d;
            restaurants.emplace_back(a, b);
            destinations.emplace_back(c, d);
        }

        return Input{order_count, pickup_count, office, restaurants, destinations};
    }
};

struct Output {
    /// @brief 移動距離の合計
    int dist_sum;

    /// @brief 選択した注文のリスト
    vector<int> orders;

    /// @brief 配達ルート
    vector<Point> route;

    /// @brief 出力データを構築する
    /// @param orders 選択した注文のリスト
    /// @param route 配達ルート
    Output(vector<int> orders, vector<Point> route) : orders(orders), route(route) {
        // 移動距離の合計を計算する
        dist_sum = 0;

        for (int i = 0; i < route.size() - 1; ++i) { dist_sum += route[i].dist(route[i + 1]); }
    }

    /// @brief 解を出力する
    void print() {
        // 選択した注文の集合を出力する
        cout << orders.size();

        for (int i = 0; i < orders.size(); ++i) {
            // 0-indexed -> 1-indexedに変更
            cout << " " << orders[i] + 1;
        }

        cout << endl;

        // 配達ルートを出力する
        cout << route.size();

        for (int i = 0; i < route.size(); ++i) { cout << " " << route[i].x << " " << route[i].y; }

        cout << endl;
    }
};

int get_distance(const vector<Point> &route) {
    int dist = 0;
    for (int i = 0; i < route.size() - 1; ++i) { dist += route[i].dist(route[i + 1]); }
    return dist;
}

Output solve(const Input &input) {

    vector<pair<int, bool>> candidates; // (index, レストランかどうか)
    vector<int> orders;
    vector<Point> route;

    for (int i = 0; i < input.order_count; i++) {
        if (input.office.dist(input.restaurants[i]) <= 400 &&
            input.office.dist(input.destinations[i]) <= 400) {
            candidates.push_back(make_pair(i, true));
        }
    }

    // 2.オフィスからスタート
    route.push_back(input.office);
    Point current_position = input.office; // 現在地
    int total_dist         = 0;            // 総移動距離

    vector<vector<bool>> visited = vector<vector<bool>>(input.order_count, vector<bool>(2, false));

    int orders_count = 0;

    // 最大50注文まで、または候補がなくなるまで
    while (!candidates.empty()) {

        pair<int, bool> nearest = make_pair(0, false); // (index, レストランかどうか)
        int min_dist            = 1000000;             // 最も近いレストランの距離
        int candidate_index     = -1;

        for (int idx = 0; idx < candidates.size(); idx++) {
            auto candidate = candidates[idx];

            // 最短距離が更新されたら記録
            int distance = candidate.second ? current_position.dist(input.restaurants[candidate.first])
                                            : current_position.dist(input.destinations[candidate.first]);

            if (distance < min_dist) {
                min_dist        = distance;
                nearest         = candidate;
                candidate_index = idx;
            }
        }

        current_position =
            nearest.second ? input.restaurants[nearest.first] : input.destinations[nearest.first];

        // 注文の集合に選んだレストランを追加
        if (nearest.second) {
            orders.push_back(nearest.first);
            candidates[candidate_index] = make_pair(nearest.first, false);
            orders_count++;
        } else {
            candidates.erase(candidates.begin() + candidate_index);
        }

        // 配達ルートに現在の位置を追加
        route.push_back(current_position);

        if (orders_count >= input.pickup_count) {
            cerr << "注文数が上限に達しました: " << candidates.size() << " 残り候補, " << orders_count << " 注文" << endl;

            // レストラン型の候補を安全に削除するため、後ろから削除する
           // レストラン型の候補を安全に削除するため、後ろから削除する
            vector<pair<int, bool>> delivery_candidates;
            for (int i = candidates.size() - 1; i >= 0; --i) {
                if (candidates[i].second) {
                    candidates.erase(candidates.begin() + i);
                } else {
                    delivery_candidates.push_back(candidates[i]);
                }
            }
            cerr << "フィルタリング後: " << candidates.size() << " 残り候補" << endl;

        }
    }

    // 最後にオフィスに戻る
    route.push_back(input.office);

    // 合計距離を標準エラー出力に出力
    cerr << "total distance: " << get_distance(route) << endl;

    return Output(orders, route);
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(20);

    Input input = Input::read();

    Output output = solve(input);

    // 出力する
    output.print();

    return 0;
}