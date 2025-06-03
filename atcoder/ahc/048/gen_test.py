import random

print('12 5 1000 30000 500')
colors = [(0.8,0.2,0.1), (0.1,0.9,0.2), (0.3,0.1,0.8), (0.5,0.5,0.5), (0.7,0.3,0.4)]
for r,g,b in colors:
    print(f'{r} {g} {b}')
for i in range(1000):
    r,g,b = random.random(), random.random(), random.random()
    print(f'{r:.6f} {g:.6f} {b:.6f}') 