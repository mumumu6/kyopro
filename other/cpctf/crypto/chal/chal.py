from Crypto.Util.number import getPrime, bytes_to_long

flag = "CPCTF{fake_flag}"

p = getPrime(512)
q = getPrime(512)  # qも素数に変更
e = 0x10001  # 一般的なRSA公開指数
n = p * q
c = pow(bytes_to_long(flag.encode()), e, n)

with open("output.py", "w") as f:
    f.write(f"e = {e}\n")
    f.write(f"n = {n}\n")
    f.write(f"c = {c}\n")