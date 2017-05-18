#!/usr/bin/python
import matplotlib.pyplot as plt


overflow_time = []
pos = []
path = []
coefficient = 30

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

with open('overflow_time.txt', 'r') as f:
    s = f.readline()  # num of nodes
    for i in range(0, num_nodes):
        s = f.readline()
        overflow_time.append(int(s))

with open('result.txt', 'r') as f:
    s = f.readline()
    num_vertices = int(s)
    s = f.readline()
    split = s.split()
    for i in range(0, num_vertices):
        path.append(int(split[i]))

X = [e[0] for e in pos]
Y = [e[1] for e in pos]

marksize = [t / 50.0 / coefficient for t in overflow_time]

# Draw points
plt.plot(X[0], Y[0], "o", color="r", ms=20)
for i in range(1, num_nodes):
    plt.plot(X[i], Y[i], "o", color="g", ms=marksize[i])

# Put numbers
i = 0
for x, y in zip(X, Y):
    plt.annotate(str(i), xy=(x, y))
    i += 1

# Draw path vector
X = [pos[e][0] for e in path]
Y = [pos[e][1] for e in path]
del X[-1]
del Y[-1]
U = []
V = []
for i in range(1, len(path)):
    U.append(pos[path[i]][0] - pos[path[i - 1]][0])
    V.append(pos[path[i]][1] - pos[path[i - 1]][1])

for i in range(0, len(path) - 1):
    plt.arrow(X[i], Y[i], U[i], V[i],
              shape='full', lw=1, length_includes_head=True,
              head_width=10, head_length=20)

fig = plt.gcf()
fig.set_size_inches(15, 15)
fig.savefig("path.png")
