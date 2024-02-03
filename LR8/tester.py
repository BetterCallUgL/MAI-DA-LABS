import random


N = 30
M = 600


def generate_test_case():
    num_segments = N  # Количество отрезков
    segments = []

    for _ in range(num_segments):
        start = random.randint(-(M/N) * 10, M / 20 * int(M / N))  # Начальная координата
        end = start + random.randint(0, M / 10 * int(M/N))  # Конечная координата
        segments.append(f'{start} {end}')

    return segments, M


def main():
    with open('input.txt', 'w') as file1:
        segments, _ = generate_test_case()
        print(N, *segments, M, sep='\n', file=file1)


if __name__ == "__main__":
    main()