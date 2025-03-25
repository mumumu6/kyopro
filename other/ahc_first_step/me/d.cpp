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
#define all(x) std::begin(x), std::end(x)
#define mp(a, b) make_pair(a, b)
#define pii pair<int, int>
#define pll pair<ll, ll>
#define pb(x) push_back(x)
#define so(z) sort(z.begin(), z.end())
#define sor(z) sort(z.rbegin(), z.rend())
#define vec vector<ll>
#define vecc vector<vector<ll>>

ofstream outputfile("/home/mumumu/kyopro/other/ahc_first_step/cpp/tools/out/0097.txt");
ifstream inputfile("/home/mumumu/kyopro/other/ahc_first_step/cpp/tools/in/0097.txt"); // 入力ファイル名

struct Point {
    int x;
    int y;

    Point(int x, int y) : x(x), y(y) {}

    /// @brief 2点間のマンハッタン距離を計算する
    /// @param p 距離を計算する点
    /// @return 2点間のマンハッタン距離
    int dist(const Point &p) const { return std::abs(x - p.x) + std::abs(y - p.y); }

    // Add comparison operators for use in containers like multiset
    bool operator<(const Point &other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    bool operator==(const Point &other) const { return x == other.x && y == other.y; }
};

struct Route_index {
    Point point;
    int index;
    bool is_restaurant;
    int route_number; // 目的地ならレストラン、レストランなら目的地のルートの番目
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
            inputfile >> a >> b >> c >> d;
            restaurants.emplace_back(a, b);
            destinations.emplace_back(c, d);
        }

        return Input{order_count, pickup_count, office, restaurants, destinations};
    }
};

/// @brief 出力データを表す構造体
struct Output {
    /// @brief 移動距離の合計
    int dist_sum;

    /// @brief 選択した注文のリスト
    vector<int> orders;

    /// @brief 配達ルート
    vector<Point> route;

    /// @brief 配達ルートのインデックス情報
    vector<Route_index> route_indices;

    map<int, int> visit_restaurant_set;  // indexから何番目に訪れたかを分かるようにする
    map<int, int> visit_destination_set; // indexから何番目に訪れたかを分かるようにする

    /// @brief 出力データを構築する
    /// @param orders 選択した注文のリスト
    /// @param route 配達ルート
    Output(vector<int> orders, vector<Point> route) : orders(orders), route(route) {
        // 移動距離の合計を計算する
        dist_sum = 0;

        for (int i = 0; i < route.size() - 1; ++i) { dist_sum += route[i].dist(route[i + 1]); }
    }

    /// @brief 出力データを構築する（インデックス情報付き）
    /// @param orders 選択した注文のリスト
    /// @param route 配達ルート
    /// @param route_indices 配達ルートのインデックス情報
    Output(vector<int> orders, vector<Point> route, vector<Route_index> route_indices)
        : orders(orders), route(route), route_indices(route_indices) {
        // 移動距離の合計を計算する
        dist_sum = 0;

        for (int i = 0; i < route.size() - 1; ++i) { dist_sum += route[i].dist(route[i + 1]); }

        // indexと訪問順を記録
        for (int i = 0; i < route_indices.size(); ++i) {
            if (route_indices[i].index != -1) { // オフィスでない場合
                if (route_indices[i].is_restaurant) {
                    visit_restaurant_set[route_indices[i].index] = i;
                } else {
                    visit_destination_set[route_indices[i].index] = i;
                }
            }
        }
    }

    /// @brief 解を出力する
    void print() {
        // 選択した注文の集合を出力する
        outputfile << orders.size();

        for (int i = 0; i < orders.size(); ++i) {
            // 0-indexed -> 1-indexedに変更
            outputfile << " " << orders[i] + 1;
        }

        outputfile << endl;

        // 配達ルートを出力する
        outputfile << route.size();

        for (int i = 0; i < route.size(); ++i) { outputfile << " " << route[i].x << " " << route[i].y; }

        outputfile << endl;

        // // インデックス情報が存在する場合は、デバッグ情報として出力
        // if (!route_indices.empty()) {
        //     cerr << "Route indices information:" << endl;
        //     for (int i = 0; i < route_indices.size(); ++i) {
        //         cerr << "Point(" << route_indices[i].point.x << ", " << route_indices[i].point.y <<
        //         ")"; cerr << " - Index: " << route_indices[i].index; cerr << " - Is restaurant: " <<
        //         (route_indices[i].is_restaurant ? "Yes" : "No") << endl;
        //     }
        // }
    }
};

int get_distance(const std::vector<Point> &route) {
    int dist = 0;
    for (int i = 0; i < route.size() - 1; ++i) { dist += route[i].dist(route[i + 1]); }
    return dist;
}

Output solve(const Input &input) {
    // 貪欲その2
    // 以下を順に実行するプログラム
    // 1.高橋君は最初オフィスから出発する
    // 2.訪問したレストランが50軒に達するまで、今いる場所から一番近いレストランに移動することを繰り返す
    // 3.受けた注文を捌ききるまで、今いる場所から一番近い配達先に移動することを繰り返す
    // 4.オフィスに帰る

    vector<int> candidates;
    vector<int> orders;                // 注文の集合
    vector<Point> route;               // 配達ルート
    vector<Route_index> route_indices; // 配達ルートのインデックス情報

    // 1. オフィスから距離400以下の注文だけを候補にする
    for (int i = 0; i < input.order_count; i++) {
        if (input.office.dist(input.restaurants[i]) <= 300 &&
            input.office.dist(input.destinations[i]) <= 380) {
            candidates.push_back(i);
        }
    }

    // 1.オフィスからスタート
    route.push_back(input.office);
    // オフィスのインデックス情報を追加
    route_indices.push_back({input.office, -1, false}); // オフィスなので特別な値 -1

    Point current_position = input.office; // 現在地
    int total_dist         = 0;            // 総移動距離

    // 2.訪問したレストランが50軒に達するまで、今いる場所から一番近いレストランに移動することを繰り返す

    // 同じレストランを2回訪れてはいけないので、訪問済みのレストランを記録する
    vector<bool> visited_restaurant(input.order_count, false);

    // pickup_count(=50)回ループ
    for (int i = 0; i < input.pickup_count + 50; ++i) {
        // レストランを全探索して、最も近いレストランを探す
        int nearest_restaurant = 0;       // レストランの番号
        int min_dist           = 1000000; // 最も近いレストランの距離

        for (int j : candidates) {
            // 既に訪れていたらスキップ
            if (visited_restaurant[j]) { continue; }

            // 最短距離が更新されたら記録
            int distance = current_position.dist(input.restaurants[j]);

            if (distance < min_dist) {
                min_dist           = distance;
                nearest_restaurant = j;
            }
        }

        // 最も近いレストラン(nearest_restaurant)に移動する
        // 【穴埋め】現在位置を最も近いレストランの位置に更新
        current_position = input.restaurants[nearest_restaurant];

        // 【穴埋め】注文の集合に選んだレストランを追加
        orders.push_back(nearest_restaurant);

        // 【穴埋め】配達ルートに現在の位置を追加
        route.push_back(current_position);

        // レストランのインデックス情報を追加
        route_indices.push_back({current_position, nearest_restaurant, true});

        // 【穴埋め】訪問済みレストランの配列にtrueをセット
        visited_restaurant[nearest_restaurant] = true;

        // 総移動距離の更新
        total_dist += min_dist;

        // デバッグしやすいよう、標準エラー出力にレストランを出力
        // 標準エラー出力はデバッグに有効なので、AHCでは積極的に活用していきましょう
        Point restaurant_pos = input.restaurants[nearest_restaurant];
        cerr << i << "番目のレストラン: p_" << nearest_restaurant << " = (" << restaurant_pos.x << ", "
             << restaurant_pos.y << ")" << endl;
    }

    // 【ヒント】ここまで穴埋めできたら、正しく動くか一度実行してみましょう！

    // 3.受けた注文を捌ききるまで、今いる場所から一番近い配達先に移動することを繰り返す

    // 行かなければいけない配達先のリスト
    // ordersは最終的に出力しなければならないので、ここでコピーを作成しておく
    // 配達先を訪問したらこのリストから1つずつ削除していく
    vector<int> destinations(orders);

    // pickup_count(=50)回ループ
    for (int i = 0; i < input.pickup_count; ++i) {
        // 配達先を全探索して、最も近い配達先を探す
        int nearest_index       = 0;                           // 配達先リストのインデックス
        int nearest_destination = destinations[nearest_index]; // 配達先の番号
        int min_dist            = 1000000;                     // 最も近い配達先の距離

        // 0～999まで全探索するのではなく、50個のレストランに対応した配達先を全探索することに注意
        for (int j = 0; j < destinations.size(); ++j) {
            // 【穴埋め】最短距離が更新されたら記録
            if (min_dist > current_position.dist(input.destinations[destinations[j]])) {
                min_dist            = current_position.dist(input.destinations[destinations[j]]);
                nearest_index       = j;
                nearest_destination = destinations[nearest_index];
            }
        }

        // 最も近い配達先(nearest_destination)に移動する
        // 【穴埋め】現在位置を最も近い配達先の位置に更新
        current_position = input.destinations[nearest_destination];

        // 【穴埋め】配達ルートに現在の位置を追加
        route.push_back(current_position);

        // 配達先のインデックス情報を追加
        route_indices.push_back({current_position, nearest_destination, false});

        // 【穴埋め】配達先のリストから削除
        destinations.erase(destinations.begin() + nearest_index);

        // 総移動距離の更新
        total_dist += min_dist;

        // デバッグしやすいよう、標準エラー出力に配達先を出力
        Point destination_pos = input.destinations[nearest_destination];
        cerr << i << "番目の配達先: q_" << nearest_destination << " = (" << destination_pos.x << ", "
             << destination_pos.y << ")" << endl;
    }

    // 削除対象のインデックスを特定
    vector<int> indices_to_remove;
    for (int idx : destinations) {
        for (int i = 0; i < orders.size(); ++i) {
            if (orders[i] == idx) {
                indices_to_remove.push_back(i);
                break;
            }
        }
    }

    // 大きいインデックスから削除（小さい方から削除するとインデックスがずれる）
    sort(indices_to_remove.begin(), indices_to_remove.end(), greater<int>());

    for (int idx : indices_to_remove) {
        // 対応するrouteを削除（route[0]はオフィスなのでインデックスを+1する）
        route.erase(route.begin() + idx + 1);
        // 対応するroute_indicesを削除
        route_indices.erase(route_indices.begin() + idx + 1);
        // orderを削除
        orders.erase(orders.begin() + idx);
    }

    // 4.オフィスに戻る
    route.push_back(input.office);
    // オフィスのインデックス情報を追加
    route_indices.push_back({input.office, -1, false}); // オフィスなので特別な値 -1

    total_dist += current_position.dist(input.office);

    // 合計距離を標準エラー出力に出力
    cerr << "total distance: " << total_dist << endl;

    return Output(orders, route, route_indices);
}

/// @brief 配達先の訪問順序を焼きなまし法で改善する関数
/// @param input 入力データ
/// @param output_greedy 貪欲法で求めた出力データ
/// @return 出力データ
Output solve_simulated_annealing(const Input &input, const Output &output_greedy) {
    // 焼きなまし法
    // 「ある1つの配達先を訪問する順序を、別の場所に入れ替える」操作を繰り返すことで、経路を改善する
    // 山登り法と異なり、一時的に解が悪化する操作も確率的に受け入れることで局所解から抜け出す

    // 貪欲法で求めた解をコピー(これを初期解とする)
    vector<int> orders                = output_greedy.orders;
    vector<Point> route               = output_greedy.route;
    vector<Route_index> route_indices = output_greedy.route_indices;

    // 現在の経路の距離を計算
    int current_dist                       = get_distance(route);
    int best_dist                          = current_dist;
    vector<Point> best_route               = route;
    vector<Route_index> best_route_indices = route_indices;

    // 乱数生成器を用意
    mt19937 rand{42};
    uniform_real_distribution<double> uniform_dist(0.0, 1.0);

    // 焼きなまし法のパラメータ
    double start_temp = 1000.0; // 開始温度
    double end_temp   = 10.0;   // 終了温度

    // 焼きなまし法の開始時刻を取得
    auto start_time = chrono::system_clock::now();

    // 制限時間(1.9秒)
    const int time_limit = 1800;

    // 試行回数
    int iteration = 0;
    int accepted  = 0; // 受理された回数
    int improved  = 0; // 改善された回数

    // 焼きなまし法の本体
    while (true) {
        // 現在時刻を取得
        auto current_time = chrono::system_clock::now();
        int elapsed_ms = chrono::duration_cast<chrono::milliseconds>(current_time - start_time).count();

        // 制限時間になったら終了
        if (elapsed_ms >= time_limit) { break; }

        // 経過時間から現在の温度を計算
        // 時間の経過とともに指数関数的に温度を下げる
        double progress = static_cast<double>(elapsed_ms) / time_limit;
        double temp     = start_temp * pow(end_temp / start_temp, progress);

        // 近傍操作をランダムに選択
        int t = rand() % 4;
        if (t < 3) { // 75%の確率で配達先を入れ替え
            // 配達先（51～100番目）の入れ替え
            int i = 51 + rand() % input.pickup_count;
            int j = 51 + rand() % input.pickup_count;

            // i番目の要素を削除
            swap(route[i], route[j]);
            swap(route_indices[i], route_indices[j]);

            // 操作後の経路の距離を計算
            int new_dist = get_distance(route);
            int delta    = new_dist - current_dist;

            // 確率的に受理するかどうかを決定
            bool accept = false;
            if (delta <= 0) {
                // 改善する場合は常に受理
                accept = true;
                improved++;
            } else {
                // 悪化する場合は温度に応じた確率で受理
                double probability = exp(-delta / temp);
                if (uniform_dist(rand) < probability) { accept = true; }
            }

            if (accept) {
                // 解を受理
                current_dist = new_dist;
                accepted++;

                // 最良解の更新
                if (new_dist < best_dist) {
                    best_dist          = new_dist;
                    best_route         = route;
                    best_route_indices = route_indices;
                    cerr << "iteration: " << iteration << ", temp: " << temp
                         << ", new best distance: " << best_dist << endl;
                }
            } else {
                // 解を棄却して元に戻す
                swap(route[i], route[j]);
                swap(route_indices[i], route_indices[j]);
            }
        } else { // 25%の確率でレストランを入れ替え
            // レストラン（1～50番目）の入れ替え
            int i = 1 + rand() % input.pickup_count;
            int j = 1 + rand() % input.pickup_count;

            swap(route[i], route[j]);
            swap(route_indices[i], route_indices[j]);

            // 操作後の経路の距離を計算
            int new_dist = get_distance(route);
            int delta    = new_dist - current_dist;

            // 確率的に受理するかどうかを決定
            bool accept = false;
            if (delta <= 0) {
                // 改善する場合は常に受理
                accept = true;
                improved++;
            } else {
                // 悪化する場合は温度に応じた確率で受理
                double probability = exp(-delta / temp);
                if (uniform_dist(rand) < probability) { accept = true; }
            }

            if (accept) {
                // 解を受理
                current_dist = new_dist;
                accepted++;

                // 最良解の更新
                if (new_dist < best_dist) {
                    best_dist          = new_dist;
                    best_route         = route;
                    best_route_indices = route_indices;
                    cerr << "iteration: " << iteration << ", temp: " << temp
                         << ", new best distance: " << best_dist << endl;
                }
            } else {
                swap(route[i], route[j]);
                swap(route_indices[i], route_indices[j]);
            }
        }

        // 試行回数のカウントを増やす
        iteration++;
    }

    // 試行回数と合計距離を標準エラー出力に出力
    cerr << "--- Result ---" << endl;
    cerr << "iteration     : " << iteration << endl;
    cerr << "accepted      : " << accepted << endl;
    cerr << "improved      : " << improved << endl;
    cerr << "best distance : " << best_dist << endl;

    return Output(orders, best_route, best_route_indices);
}

Output mix(const Input &input, const Output &output_annealing) {
    vector<int> orders                         = output_annealing.orders;
    vector<Point> original_route               = output_annealing.route;
    vector<Route_index> original_route_indices = output_annealing.route_indices;
    map<int, int> visit_restaurant_set         = output_annealing.visit_restaurant_set;
    map<int, int> visit_destination_set        = output_annealing.visit_destination_set;
    vector<bool> visited_restaurant(10003, false);
    vector<bool> visited_destination(1003, false);

    // 新しいルートを構築
    vector<Point> new_route;
    vector<Route_index> new_route_indices; // 初期化を追加

    multiset<pair<Point, int>> next_visits; // 寄り道していい目的地の座標とindex

    Point current_position = input.office;

    rep(i, original_route.size()) { // 目的地に寄り道して距離が縮まないか確認していく
        // routeから一つずつ取り出していくだけにする。既にみたものはskipする方針

        if (!original_route_indices[i].is_restaurant &&
            visited_destination[original_route_indices[i].index]) { // 目的地で既に行っていればスキップ
            cout << "skiped : " << i << endl;
            cout << original_route_indices[i].index << endl;
            continue;
        }

        current_position = original_route[i];
        new_route.push_back(current_position);

        vector<pair<Point, int>> near_destination;

        for (auto now_destination : next_visits) {
            // 距離40以内に目的地があれば問答無用で追加する

            if (now_destination.first.dist(current_position) <= 40) {
                current_position = now_destination.first;

                visited_destination[now_destination.second] = true;
                near_destination.push_back(now_destination);
                new_route.push_back( // ルートに追加
                    now_destination
                        .first); // こことりあえず入れているが候補のものの中できちんと順序を付けた方がいいかもしれない
            }
        }

        for (auto delete_candidate : near_destination) { // 行く候補から削除
            next_visits.erase(next_visits.find(delete_candidate));
        }
    }

    return Output(orders, new_route, original_route_indices);
}

int main() {

    // 入力データを読み込む
    Input input = Input::read();

    // 問題を解く
    Output output           = solve(input);
    Output output_annealing = solve_simulated_annealing(input, output);

    Output output_last = mix(input, output_annealing);

    // 出力する
    output_last.print();
    return 0;
}
