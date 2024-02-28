#! /usr/bin/env python3

import sys
import random
import string

TESTS_CNT = 10

CNT_TEST_PER_FILE = 10

def generate_kv():
    key = random.randint(0, 2 ** 64 - 1)
    value = random.choice(string.ascii_letters)
    return key, value


def main():

    if len(sys.argv) != 2:
        print("Usage: {sys.argv[0]} <test path>")
        sys.exit(1)

    test_dir = sys.argv[1]
    for num in range(1, TESTS_CNT + 1):
        filename = f"{test_dir}/{num:02}"
        with open(f"{filename}.t", 'w') as tfd, \
            open(f"{filename}.a", 'w') as afd:
            arr = []

            for _ in range(CNT_TEST_PER_FILE):
                key, value = generate_kv()
                arr.append((key, value))
                tfd.write(f"{key}\t{value}\n")
            arr.sort(key=lambda x: x[0])
            for elem in arr:
                afd.write(f"{elem[0]}\t{elem[1]}\n")

if __name__ == "__main__":
    main()