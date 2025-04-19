# cipher.txtの値を使用
cipher = 109414994672658160164900449719144933039623668542660

def try_decode(cipher, length):
    result = []
    current = cipher
    
    # 最後の文字から逆算
    for i in range(length-1, -1, -1):
        idx = 1000 + i
        
        # 現在の数値がidxで割り切れるか確認
        if current % idx != 0:
            return None  # 割り切れない場合は失敗
        
        # 掛け算の逆操作
        current = current // idx
        
        # 文字コードを取得（最後に加えられた値）
        char_code = current % 256  # ASCIIコードの範囲内に制限
        
        # 有効なASCIIコードか確認
        if 32 <= char_code <= 126:  # 印字可能なASCII文字の範囲
            result.insert(0, chr(int(char_code)))
            # 加算の逆操作
            current -= char_code
        else:
            return None  # 無効な文字コードの場合は失敗
    
    # 全ての処理が終わった後、currentが0になっているか確認
    if current == 0:
        return ''.join(result)
    return None

# 様々な長さを試す
for length in range(1, 20):
    result = try_decode(cipher, length)
    if result:
        print(f"長さ {length} の場合: {result}")

# ans.txtの値も試してみる
cipher2 = 103200264548574214569124695908951019136986646123214535931636006688814109904122192900997137101
print("\nans.txtの値を使用:")
for length in range(1, 20):
    result = try_decode(cipher2, length)
    if result:
        print(f"長さ {length} の場合: {result}")ｙｙｙｙ