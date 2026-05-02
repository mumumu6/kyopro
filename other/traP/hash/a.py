import hashlib

x = bytes.fromhex("6d756d756d752d000003edffd4ec4a")
print(hashlib.sha256(x).hexdigest())-