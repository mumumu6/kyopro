/**
 * @file 01_nearest_neighbor.cpp
 * @brief
 * オフィスから距離400以下の注文だけを候補にした上で、今いる点から最も近いレストランに行くことを50回繰り返し、
 * その後今いる点から最も近い目的地に行くことを50回繰り返す解法プログラム
 */
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

/// @brief 2次元座標上の点を表す構造体
struct Point {
    int x;
    int y;

    Point(int x, int y) : x(x), y(y) {}

    /// @brief 2点間のマンハッタン距離を計算する
    /// @param p 距離を計算する点
    /// @return 2点間のマンハッタン距離
    int dist(const Point& p) const { return std::abs(x - p.x) + std::abs(y - p.y); } 
};

struct Route_index{
    Point point;
    int index;
    bool is_restaurant;
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
    std::vector<Point> restaurants;
    /// @brief 目的地の座標の配列
    std::vector<Point> destinations;

    /// @brief 入力データを読み込む
    /// @return 読み込んだ入力データ
    static Input read() {
        int order_count = 1000;
        int pickup_count = 50;
        Point office(400, 400);
        std::vector<Point> restaurants;
        std::vector<Point> destinations;

        for (int i = 0; i < order_count; ++i) {
            int a, b, c, d;
            std::cin >> a >> b >> c >> d;
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
    std::vector<int> orders;

    /// @brief 配達ルート
    std::vector<Point> route;

    /// @brief 配達ルートのインデックス情報
    std::vector<Route_index> route_indices;

    /// @brief 出力データを構築する
    /// @param orders 選択した注文のリスト
    /// @param route 配達ルート
    Output(std::vector<int> orders, std::vector<Point> route) : orders(orders), route(route) {
        // 移動距離の合計を計算する
        dist_sum = 0;

        for (int i = 0; i < route.size() - 1; ++i) {
            dist_sum += route[i].dist(route[i + 1]);
        }
    }

    /// @brief 出力データを構築する（インデックス情報付き）
    /// @param orders 選択した注文のリスト
    /// @param route 配達ルート
    /// @param route_indices 配達ルートのインデックス情報
    Output(std::vector<int> orders, std::vector<Point> route, std::vector<Route_index> route_indices) 
        : orders(orders), route(route), route_indices(route_indices) {
        // 移動距離の合計を計算する
        dist_sum = 0;

        for (int i = 0; i < route.size() - 1; ++i) {
            dist_sum += route[i].dist(route[i + 1]);
        }
    }

    /// @brief 解を出力する
    void print() {
        // 選択した注文の集合を出力する
        std::cout << orders.size();

        for (int i = 0; i < orders.size(); ++i) {
            // 0-indexed -> 1-indexedに変更
            std::cout << " " << orders[i] + 1;
        }

        std::cout << std::endl;

        // 配達ルートを出力する
        std::cout << route.size();

        for (int i = 0; i < route.size(); ++i) {
            std::cout << " " << route[i].x << " " << route[i].y;
        }

        std::cout << std::endl;

        インデックス情報が存在する場合は、デバッグ情報として出力
        if (!route_indices.empty()) {
            std::cerr << "Route indices information:" << std::endl;
            for (int i = 0; i < route_indices.size(); ++i) {
                std::cerr << "Point(" << route_indices[i].point.x << ", " << route_indices[i].point.y << ")";
                std::cerr << " - Index: " << route_indices[i].index;
                std::cerr << " - Is restaurant: " << (route_indices[i].is_restaurant ? "Yes" : "No") << std::endl;
            }
        }
    }
};

/// @brief 経路の距離を計算する
/// @param route 経路
/// @return 経路の距離
int get_distance(const std::vector<Point>& route) {
    int dist = 0;
    for (int i = 0; i < route.size() - 1; ++i) {
        dist += route[i].dist(route[i + 1]);
    }
    return dist;
}

/// @brief 問題を解く関数（この関数を実装していきます）
/// @param input 入力データ
/// @return 出力データ
Output solve(const Input& input) {
    // 貪欲その2
    // 以下を順に実行するプログラム
    // 1.高橋君は最初オフィスから出発する
    // 2.訪問したレストランが50軒に達するまで、今いる場所から一番近いレストランに移動することを繰り返す
    // 3.受けた注文を捌ききるまで、今いる場所から一番近い配達先に移動することを繰り返す
    // 4.オフィスに帰る
    
    std::vector<int> candidates; 
    std::vector<int> orders;   // 注文の集合
    std::vector<Point> route;  // 配達ルート
    std::vector<Route_index> route_indices; // 配達ルートのインデックス情報

    // 1. オフィスから距離400以下の注文だけを候補にする
    for (int i = 0; i < input.order_count; i++) {
        if (input.office.dist(input.restaurants[i]) <= 490 && input.office.dist(input.destinations[i]) <= 490) {
            candidates.push_back(i);
        }
    }

    // 1.オフィスからスタート
    route.push_back(input.office);
    // オフィスのインデックス情報を追加
    route_indices.push_back({input.office, -1, false}); // オフィスなので特別な値 -1

    Point current_position = input.office;  // 現在地
    int total_dist = 0;                     // 総移動距離

    // 2.訪問したレストランが50軒に達するまで、今いる場所から一番近いレストランに移動することを繰り返す

    // 同じレストランを2回訪れてはいけないので、訪問済みのレストランを記録する
    std::vector<bool> visited_restaurant(input.order_count, false);

    // pickup_count(=50)回ループ
    for (int i = 0; i < input.pickup_count + 50; ++i) {
        // レストランを全探索して、最も近いレストランを探す
        int nearest_restaurant = 0;  // レストランの番号
        int min_dist = 1000000;      // 最も近いレストランの距離

        for (int j : candidates) {
            // 既に訪れていたらスキップ
            if (visited_restaurant[j]) {
                continue;
            }

            // 最短距離が更新されたら記録
            int distance = current_position.dist(input.restaurants[j]);

            if (distance < min_dist) {
                min_dist = distance;
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
        std::cerr << i << "番目のレストラン: p_" << nearest_restaurant << " = (" << restaurant_pos.x << ", "
                  << restaurant_pos.y << ")" << std::endl;
    }

    // 【ヒント】ここまで穴埋めできたら、正しく動くか一度実行してみましょう！

    // 3.受けた注文を捌ききるまで、今いる場所から一番近い配達先に移動することを繰り返す

    // 行かなければいけない配達先のリスト
    // ordersは最終的に出力しなければならないので、ここでコピーを作成しておく
    // 配達先を訪問したらこのリストから1つずつ削除していく
    std::vector<int> destinations(orders);

    // pickup_count(=50)回ループ
    for (int i = 0; i < input.pickup_count; ++i) {
        // 配達先を全探索して、最も近い配達先を探す
        int nearest_index = 0;                                  // 配達先リストのインデックス
        int nearest_destination = destinations[nearest_index];  // 配達先の番号
        int min_dist = 1000000;                                 // 最も近い配達先の距離

        // 0～999まで全探索するのではなく、50個のレストランに対応した配達先を全探索することに注意
        for (int j = 0; j < destinations.size(); ++j) {
            // 【穴埋め】最短距離が更新されたら記録
            if(min_dist > current_position.dist(input.destinations[destinations[j]])){
                min_dist = current_position.dist(input.destinations[destinations[j]]);
                nearest_index = j;
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
        std::cerr << i << "番目の配達先: q_" << nearest_destination << " = (" << destination_pos.x << ", "
                  << destination_pos.y << ")" << std::endl;
    }

    // 削除対象のインデックスを特定
    std::vector<int> indices_to_remove;
    for (int idx : destinations) {
        for (int i = 0; i < orders.size(); ++i) {
            if (orders[i] == idx) {
                indices_to_remove.push_back(i);
                break;
            }
        }
    }
    
    // 大きいインデックスから削除（小さい方から削除するとインデックスがずれる）
    std::sort(indices_to_remove.begin(), indices_to_remove.end(), std::greater<int>());
    
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
    std::cerr << "total distance: " << total_dist << std::endl;

    return Output(orders, route, route_indices);
}

/// @brief 配達先の訪問順序を焼きなまし法で改善する関数
/// @param input 入力データ
/// @param output_greedy 貪欲法で求めた出力データ
/// @return 出力データ
Output solve_simulated_annealing(const Input& input, const Output& output_greedy) {
    // 焼きなまし法
    // 「ある1つの配達先を訪問する順序を、別の場所に入れ替える」操作を繰り返すことで、経路を改善する
    // 山登り法と異なり、一時的に解が悪化する操作も確率的に受け入れることで局所解から抜け出す

    // 貪欲法で求めた解をコピー(これを初期解とする)
    std::vector<int> orders = output_greedy.orders;
    std::vector<Point> route = output_greedy.route;
    std::vector<Route_index> route_indices = output_greedy.route_indices;

    // 現在の経路の距離を計算
    int current_dist = get_distance(route);
    int best_dist = current_dist;
    std::vector<Point> best_route = route;
    std::vector<Route_index> best_route_indices = route_indices;

    // 乱数生成器を用意
    std::mt19937 rand{42};
    std::uniform_real_distribution<double> uniform_dist(0.0, 1.0);

    // 焼きなまし法のパラメータ
    double start_temp = 10000.0;  // 開始温度
    double end_temp = 10.0;      // 終了温度

    // 焼きなまし法の開始時刻を取得
    auto start_time = std::chrono::system_clock::now();

    // 制限時間(1.9秒)
    const int time_limit = 1800;

    // 試行回数
    int iteration = 0;
    int accepted = 0;  // 受理された回数
    int improved = 0;  // 改善された回数

    // 焼きなまし法の本体
    while (true) {
        // 現在時刻を取得
        auto current_time = std::chrono::system_clock::now();
        int elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();

        // 制限時間になったら終了
        if (elapsed_ms >= time_limit) {
            break;
        }

        // 経過時間から現在の温度を計算
        // 時間の経過とともに指数関数的に温度を下げる
        double progress = static_cast<double>(elapsed_ms) / time_limit;
        double temp = start_temp * std::pow(end_temp / start_temp, progress);

        // 近傍操作をランダムに選択
        int t = rand() % 4;
        if (t < 3) {  // 75%の確率で配達先を入れ替え
            // 配達先（51～100番目）の入れ替え
            int i = 51 + rand() % input.pickup_count;
            int j = 51 + rand() % input.pickup_count;

            // i番目の訪問先を一時保存
            Point tmp = route[i];
            Route_index tmp_index = route_indices[i];
            
            // i番目の要素を削除
            route.erase(route.begin() + i);
            route_indices.erase(route_indices.begin() + i);
            
            // j番目に挿入（j>iの場合はj-1番目になることに注意）
            int insert_pos = j;
            if (i < j) insert_pos--;
            
            route.insert(route.begin() + insert_pos, tmp);
            route_indices.insert(route_indices.begin() + insert_pos, tmp_index);

            // 操作後の経路の距離を計算
            int new_dist = get_distance(route);
            int delta = new_dist - current_dist;

            // 確率的に受理するかどうかを決定
            bool accept = false;
            if (delta <= 0) {
                // 改善する場合は常に受理
                accept = true;
                improved++;
            } else {
                // 悪化する場合は温度に応じた確率で受理
                double probability = std::exp(-delta / temp);
                if (uniform_dist(rand) < probability) {
                    accept = true;
                }
            }

            if (accept) {
                // 解を受理
                current_dist = new_dist;
                accepted++;
                
                // 最良解の更新
                if (new_dist < best_dist) {
                    best_dist = new_dist;
                    best_route = route;
                    best_route_indices = route_indices;
                    std::cerr << "iteration: " << iteration << ", temp: " << temp 
                              << ", new best distance: " << best_dist << std::endl;
                }
            } else {
                // 解を棄却して元に戻す
                route.erase(route.begin() + insert_pos);
                route_indices.erase(route_indices.begin() + insert_pos);
                route.insert(route.begin() + i, tmp);
                route_indices.insert(route_indices.begin() + i, tmp_index);
            }
        } else {  // 25%の確率でレストランを入れ替え
            // レストラン（1～50番目）の入れ替え
            int i = 1 + rand() % input.pickup_count;
            int j = 1 + rand() % input.pickup_count;

            // i番目の訪問先を一時保存
            Point tmp = route[i];
            Route_index tmp_index = route_indices[i];
            
            // i番目の要素を削除
            route.erase(route.begin() + i);
            route_indices.erase(route_indices.begin() + i);
            
            // j番目に挿入（j>iの場合はj-1番目になることに注意）
            int insert_pos = j;
            if (i < j) insert_pos--;
            
            route.insert(route.begin() + insert_pos, tmp);
            route_indices.insert(route_indices.begin() + insert_pos, tmp_index);

            // 操作後の経路の距離を計算
            int new_dist = get_distance(route);
            int delta = new_dist - current_dist;

            // 確率的に受理するかどうかを決定
            bool accept = false;
            if (delta <= 0) {
                // 改善する場合は常に受理
                accept = true;
                improved++;
            } else {
                // 悪化する場合は温度に応じた確率で受理
                double probability = std::exp(-delta / temp);
                if (uniform_dist(rand) < probability) {
                    accept = true;
                }
            }

            if (accept) {
                // 解を受理
                current_dist = new_dist;
                accepted++;
                
                // 最良解の更新
                if (new_dist < best_dist) {
                    best_dist = new_dist;
                    best_route = route;
                    best_route_indices = route_indices;
                    std::cerr << "iteration: " << iteration << ", temp: " << temp 
                              << ", new best distance: " << best_dist << std::endl;
                }
            } else {
                // 解を棄却して元に戻す
                route.erase(route.begin() + insert_pos);
                route_indices.erase(route_indices.begin() + insert_pos);
                route.insert(route.begin() + i, tmp);
                route_indices.insert(route_indices.begin() + i, tmp_index);
            }
        }

        // 試行回数のカウントを増やす
        iteration++;
    }

    // 試行回数と合計距離を標準エラー出力に出力
    std::cerr << "--- Result ---" << std::endl;
    std::cerr << "iteration     : " << iteration << std::endl;
    std::cerr << "accepted      : " << accepted << std::endl;
    std::cerr << "improved      : " << improved << std::endl;
    std::cerr << "best distance : " << best_dist << std::endl;

    return Output(orders, best_route, best_route_indices);
}

Output solve_mix_destination_and_restaurant(const Input& input,const Output& output_annealing){
    // レストランを訪問した後、その近くに目的地があれば立ち寄る戦略
    
    std::vector<int> orders;          // 注文の集合
    std::vector<Point> route;         // 配達ルート
    std::vector<Route_index> route_indices; // 配達ルートのインデックス情報
    std::vector<bool> visited_restaurant(input.order_count, false);
    std::vector<bool> visited_destination(input.order_count, false);

    // コピー
    orders = output_annealing.orders;
    route = output_annealing.route;
    route_indices = output_annealing.route_indices;

    // このあとの実装はここで行います
    // routeの50番目までをコピー
    std::vector<Point> restaurant_route(route.begin(), route.begin() + 50);
    
    // ここではまだ具体的な実装はありませんが、
    // 最終的にはroute, route_indices, ordersを適切に更新します
    
    // 修正されたルートでOutputを作成
    return Output(orders, route, route_indices);
}

int main() {
    // 入力データを読み込む
    Input input = Input::read();

    // 問題を解く
    Output output = solve(input);
    Output output_annealing = solve_simulated_annealing(input, output);

    Output output_mixed = solve_mix_destination_and_restaurant(input, output_annealing);

    // 出力する
    output_mixed.print();
    return 0;
}
