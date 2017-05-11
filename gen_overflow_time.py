import math
import numpy.random as random


overflow_time = [0]
pos = []
stddev = 200
min_time = 100

# read file
with open('positions.txt', 'r') as f:
    s = f.readline()
    num_nodes = int(s)
    for i in range(0, num_nodes):
        s = f.readline()
        split = s.split()
        x = int(split[0])
        y = int(split[1])
        pos.append((x, y))


def distance_to(i):
    dx = abs(pos[i][0] - pos[0][0])
    dy = abs(pos[i][1] - pos[0][1])

    return int(math.sqrt(dx ** 2 + dy ** 2))

for i in range(1, num_nodes):
    d = distance_to(i)
    time = max(min_time, int(random.normal(loc=d, scale=stddev)))
    overflow_time.append(time)

print(overflow_time)

with open('overflow_time.txt', 'w') as f:
    f.write(str(num_nodes) + '\n')
    for t in overflow_time:
        f.write(str(t) + '\n')
