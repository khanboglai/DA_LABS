#! /usr/bin/env bash




function main()
{
    local bin=main
    local benchmark=bench
    if ! make ; then
        echo "\e[31mFailed to compile lab5.cpp\e[0m"
        return 1
    fi

    count_of_tests=1
    if ! ./generator.py ${count_of_tests} ; then
        echo "\e[31mFailed to generate tests\e[0m"
        return 1
    fi

    local test_file=tests/01.t
    if ! ./${bin} < ${test_file} ; then
      echo "\e[31mFailed to run main\e[0m"
      return 1
    fi

    if ! ./${benchmark} < ${test_file} ; then
      echo "\e[31mFailed to run bench\e[0m"
      return 1
    fi
}

main