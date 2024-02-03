import random

n = 80
m = 80

with open('input.txt', 'w') as file:
    for i in range(n):
        for j in range(m):
                file.write(random.choice('10'))
        file.write('\n')
        