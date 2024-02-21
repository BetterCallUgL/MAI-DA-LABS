from random import randint

f = open('input.txt', 'w')

n = 100000
m = 100000

f.write(str(n) + ' ' + str(m) + '\n')

for i in range(n):
  l = randint(-1000000000, 900000000)
  r = randint(l + 1, 1000000000)
  h = randint(-1000000000, 1000000000)
  f.write(str(l) + ' ' + str(r) + ' ' + str(h) + '\n')


for i in range(m):
  f.write(str(randint(-1000000000, 1000000000)) + ' ' +
          str(randint(-1000000000, 1000000000)) + '\n')

f.close()