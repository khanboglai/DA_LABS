#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import random
import os
import sys

def generate_test_case(n, m):
    edges = set()  # Используем множество для уникальности рёбер
    while len(edges) < m:
        u = random.randint(1, n)
        v = random.randint(1, n)
        if u != v:  # Исключаем петли
            weight = random.randint(1, 100)  # Случайный вес от 1 до 100
            edges.add((min(u, v), max(u, v), weight))  # Сохраняем рёбра в упорядоченном виде

    # Генерируем случайные start и finish
    start = random.randint(1, n)
    finish = random.randint(1, n)
    while start == finish:  # start и finish не должны совпадать
        finish = random.randint(1, n)

    return n, m, edges, start, finish

def write_test_case_to_file(filename, n, m, edges, start, finish):
    with open(filename, 'w') as f:
        f.write(f"{n} {m} {start} {finish}\n")
        for u, v, weight in edges:
            f.write(f"{u} {v} {weight}\n")

def main():
    num_tests = int(sys.argv[1])  # Количество тестов для генерации
    n = 20  # Количество вершин
    m = 25  # Количество рёбер

    # Создаем директорию для тестов, если она не существует
    os.makedirs("tests", exist_ok=True)

    for i in range(num_tests):
        test_case = generate_test_case(n, m)
        filename = f"tests/test_{i + 1}.t"
        write_test_case_to_file(filename, *test_case)

if __name__ == "__main__":
    main()
