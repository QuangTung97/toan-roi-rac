import numpy as np
import numpy.random as random
import math


pos = []
stddev = 50
min_distance = 10

with open('positions.txt', 'r') as f:
    s = f.readline()
    num_nodes = int(s)
    for i in range(0, num_nodes):
        s = f.readline()
        split = s.split()
        x = int(split[0])
        y = int(split[1])
        pos.append((x, y))

mat = np.zeros(num_nodes ** 2, dtype=np.uint64)     \
    .reshape((num_nodes, num_nodes))


def distance(i, j):
    dx = abs(pos[i][0] - pos[j][0])
    dy = abs(pos[i][1] - pos[j][1])

    return int(math.sqrt(dx ** 2 + dy ** 2))


for i in range(0, num_nodes):
    for j in range(0, i):
        d = distance(i, j)
        mat[i, j] = max(min_distance, int(random.normal(
            loc=d, scale=stddev)))
        mat[j, i] = mat[i, j]


with open('matrix.txt', 'w') as f:
    f.write(str(num_nodes) + '\n')
    for i in range(0, num_nodes):
        for j in range(0, num_nodes):
            f.write(str(mat[i, j]) + ' ')
        f.write('\n')
