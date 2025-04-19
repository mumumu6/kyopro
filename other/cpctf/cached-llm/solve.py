import hashlib
import random
import string

target = "41633f"

while True:
    msg = ''.join(random.choices(string.printable, k=10))  # 長さは適当でOK
    h = hashlib.sha256(msg.encode()).hexdigest()[:6]
    if h == target:
        print("🎯 HIT:", repr(msg))
        break
