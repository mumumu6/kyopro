#include <bits/stdc++.h>
#include <cxxabi.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using ll = long long;
const ll INF = 4e18;
using ld = long double;
const ld INFD = INFINITY;
const ld EPS = LDBL_EPSILON;
template<class T, class Comp = less<T>> using p_que = priority_queue<T, vector<T>, Comp>;
array sin90 = { 0, 1, 0, -1 }, cos90 = { 1, 0, -1, 0 };
array sin45 = { 0, 1, 1, 1, 0, -1, -1, -1 }, cos45 = { 1, 1, 0, -1, -1, -1, 0, 1 };
#define p1 first
#define p2 second
#define reps(i, l, r) for (decay_t<decltype(r)> i##_right = (r), i = (l); i < i##_right; i++)
#define rep(i, n) reps(i, 0, n)
#define rreps(i, l, r) for (decay_t<decltype(r)> i##_left = (l), i = (r) - 1; i >= i##_left; i--)
#define rrep(i, n) rreps(i, 0, n)
#define all(a) begin(a), end(a)
#define sz(a) ssize(a)
template <typename T> inline constexpr bool is_pair_v = false;
template <typename T, typename U> inline constexpr bool is_pair_v<pair<T, U>> = true;
template <typename T> inline string get_typename(size_t length_limit = string::npos) {
  string name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
  if (name.length() > length_limit) name = name.substr(0, length_limit - 3) + "...";
  return name;
}
bool chmin(auto& a, auto b) { return a > b ? a = b, 1 : 0; }
bool chmax(auto& a, auto b) { return a < b ? a = b, 1 : 0; }
string int128_to_str(__int128_t target) {
  string target_str;
  __uint128_t target_tmp = target < 0 ? -target : target;
  do { target_str += '0' + target_tmp % 10; target_tmp /= 10; } while (target_tmp != 0);
  if (target < 0) target_str += '-';
  reverse(target_str.begin(), target_str.end());
  return target_str;
}
template <typename T> string to_pretty_str(T target) {
  string str;
  if constexpr (is_void_v<T>) str += "void"s;
  else if constexpr (is_null_pointer_v<T>) str += "null"s;
  else if constexpr (is_same_v<T, bool>) str += target ? "true"s : "false"s;
  else if constexpr (is_same_v<T, char> || is_same_v<T, char16_t> || is_same_v<T, char32_t> || is_same_v<T, wchar_t>) str += "'"s + target + "'"s;
  else if constexpr (atcoder::internal::is_modint<T>::value) str += to_string(target.val()) + "(mod)"s;
  else if constexpr (is_arithmetic_v<T>) {
    if constexpr (is_same_v<T, __int128_t>) str += int128_to_str(target);
    else str += to_string(target);
    if constexpr (is_unsigned_v<T>) str += "u"s;
    if constexpr (is_same_v<remove_cv_t<T>, long>) str += "L"s;
    else if constexpr (is_same_v<remove_cv_t<T>, long long>) str += "LL"s;
    else if constexpr (is_same_v<T, __int128_t>) str += "LLL"s;
  } else if constexpr (is_pair_v<T>) str += "("s + to_pretty_str(target.first) + ", "s + to_pretty_str(target.second) + ")"s;
  else if constexpr (is_convertible_v<T, string>) str += """s + target + """s;
  else if constexpr (is_array_v<T>) {
    str += "["s;
    bool separate = false;
    for (const auto &target_i : target) { if (separate) str += ", "s; str += to_pretty_str(target_i); separate = true; }
    str += "]"s;
  } else if constexpr (is_same_v<decltype(begin(declval<T>())), decltype(end(declval<T>()))>) {
    str += "("s + get_typename<T>(20) + "){"s;
    bool separate = false;
    for (const auto &target_i : target) { if (separate) str += ","s; str += " "s + to_pretty_str(target_i); separate = true; }
    if (separate) str += " "s;
    str += "}"s;
  } else str += "<"s + get_typename<T>(20) + " ("s + to_string(sizeof(target)) + " byte)>"s;
  return str;
}
#ifdef DEBUG
template <typename F> void debug_txt_f(F callback, int line = -1, string file = (__FILE__)) {
  string dump_str = "[DEBUG] ";
  if (line >= 0) {
    dump_str += "(";
    if (file != (__FILE__)) dump_str += file + ":";
    dump_str += "L" + to_string(line) + ") ";
  }
  dump_str += callback();
  cerr << dump_str << endl;
}
#  define debug_txt(txt) (debug_txt_f([]() { return to_pretty_str(txt); }, (__LINE__), (__FILE__)), true)
template <typename... Types>
void dump_f(string labels, tuple<Types...> targets_tupl, int line = -1, string file = (__FILE__)) {
  debug_txt_f([=]() {
    string txt;
    apply([labels, &txt](auto... targets) {
      int i = 0, label_left = 0;
      (([&](auto target) {
        const auto label_len = labels.find(',', label_left) - label_left;
        if (i >= 1) txt += ", ";
        txt += regex_replace(labels.substr(label_left, label_len), regex("^\s+|\s+$"), "") + ": " + to_pretty_str(target);
        label_left += label_len + 1;
        i++;
      })(targets), ...);
      txt += ";";
    }, targets_tupl);
    return txt;
  }, line, file);
}
#  define dump(...) (dump_f((#__VA_ARGS__), make_tuple(__VA_ARGS__), (__LINE__), (__FILE__)), true)
#else
#  define debug_txt(...) (false)
#  define dump(...) (false)
#endif

int main() {
  cin.tie(nullptr);
  ios_base::sync_with_stdio(false);
  cout << fixed << setprecision(8);

  
  
}
