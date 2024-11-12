#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import random
import sys


MAX_NUM = 50


count_of_tests = int( sys.argv[1] )

for enum in range(count_of_tests):
    test_file_name = "tests/{:02d}".format( enum + 1 )
    with open( "{0}.t".format( test_file_name ), 'w' ) as output_file:
            # open( "{0}.a".format( test_file_name ), "w" ) as answer_file:
        
        
        n = random.randint(2, MAX_NUM)
        m = random.randint(n, MAX_NUM)

        output_file.write(str(m) + ' ' + str(n) + '\n')

        data = ""
        for _ in range(m):
            for _ in range(n):
                data += str(random.randint(0, MAX_NUM)) + ' '
                output_file.write(str(random.randint(0, MAX_NUM)) + ' ')
            data += str(random.randint(0, MAX_NUM)) + '\n'
            output_file.write(str(random.randint(0, MAX_NUM)) + '\n')

        # answer_file.write(str(answer(m, n, data)) + '\n')
