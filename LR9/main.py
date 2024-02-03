from random import randint

with open('input.txt', 'w') as file:
    file.write('10000 10000 1 100\n')
    for i in range(1, 5001):
        file.write(f'{i} {i*2} {randint(1,30)}\n')
        file.write(f'{i} {i*2 + 1} {randint(1,30)}\n')
