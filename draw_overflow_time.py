#!/usr/bin/python
import matplotlib.pyplot as plt


overflow_time = []
pos = []
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


X = [e[0] for e in pos]
Y = [e[1] for e in pos]
marksize = [t / 50.0 / coefficient for t in overflow_time]

plt.plot(X[0], Y[0], "o", color="r", ms=20)
for i in range(1, num_nodes):
    plt.plot(X[i], Y[i], "o", color="g", ms=marksize[i])

i = 0
for x, y in zip(X, Y):
    plt.annotate(str(i), xy=(x, y))
    i += 1

fig = plt.gcf()
fig.set_size_inches(15, 15)
fig.savefig("overflow.png")
