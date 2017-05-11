import random


num_nodes = 100
width = 1000
height = 1000

pos = []

for i in range(0, num_nodes):
    x = random.randint(0, width - 1)
    y = random.randint(0, height - 1)
    pos.append((x, y))

print(pos)

# write result
with open("positions.txt", "w") as f:
    f.write(str(num_nodes) + "\n")
    for i in range(0, num_nodes):
        f.write(str(pos[i][0]) + ' ' + str(pos[i][1]) + '\n')
