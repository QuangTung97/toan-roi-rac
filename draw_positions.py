#!/usr/bin/python3
import matplotlib.pyplot as plt


pos = []

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

print("Number of nodes: %d" % num_nodes)

X = [e[0] for e in pos]
Y = [e[1] for e in pos]

plt.plot(X[0], Y[0], "o", color="r", ms=20)
plt.plot(X[1:], Y[1:], "o", color="g", ms=6)
fig = plt.gcf()
fig.set_size_inches(15, 15)
fig.savefig("positions.png")
