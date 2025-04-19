cipher = 103200264548574214569124695908951019136986646123214535931636006688814109904122192900997137101

for i in range(5, 40):
    a = cipher
    s = []

    for j in range(i + 1):
        k = 1000 + i - j
        if ((a % k) != 0):
            a = a
        else:
            s.append(chr(int((a // k) % (k - 1))))
            a = a // k
            a = a - (a % (k - 1))

    if(s != []):
        s.reverse()
        print("".join(s))
        print(f"length = {i}")
        break
