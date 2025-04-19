import sys

try:
    # 標準入力からの入力を試みる
    plaintext = input()
except EOFError:
    # 入力がない場合はデフォルト値を使用
    plaintext = "default_text"
    print(f"入力がありませんでした。デフォルト値 '{plaintext}' を使用します。")

a = [ord(i) for i in plaintext]
cipher = 0
for i,chr in enumerate(a,1000):
    cipher += chr
    cipher *= i

print(f"暗号化結果: {cipher}")

try:
    f = open('cipher.txt', 'w')
    f.write(str(cipher))
    f.close()
    print("cipher.txtに書き込みました。")
except Exception as e:
    print(f"エラーが発生しました: {e}")