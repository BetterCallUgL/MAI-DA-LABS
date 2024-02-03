import random

n1 = 5000000
n2 = 50

line = str()
for i in range(1,10000):
    line = line + random.choice('abcr')

pattern = str()
for i in range(1,n2):
    pattern = pattern + random.choice('abcr')

with open('input.txt', 'w') as file:
    file.write(line + '\n')
    file.write(pattern)
