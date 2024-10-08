#! /usr/bin/env bash

set -o errexit
set -o nounset

readonly TESTS_DIR=tests

function log_info()
{
  log_ "\e[34minfo\e[0m" "$@"
}

function log_error()
{
  log_ "\e[31merror\e[0m" "$@"
}

function log_()
{
  local type=$1
  local message=$2
  local date_str=; date_str=$(date +'%Y-%m-%d %H:%M:%S')
  echo -e "[${type}] [${date_str}] ${message}"
}

function main()
{
  local bin=lab5
  log_info "\e[32mStage #1. Compiling...\e[0m"
  if ! make ; then
    log_error "\e[31mFailed to compile lab5.cpp\e[0m"
    return 1
  fi
  log_info "\e[34mCompiling OK\e[0m"

  log_info "\e[32mStage #2. Test generating...\e[0m"
  rm -rf ${TESTS_DIR}
  mkdir ${TESTS_DIR}
  local count_of_tests=1
  if ! ./generator.py ${count_of_tests} ; then
    log_error "\e[31mFailed to generate tests\e[0m"
    return 1
  fi
  log_info "\e[34mGenerating OK\e[0m"

  log_info "\e[32mStage #3 Checking...\e[0m"
  for test_file in $( ls ${TESTS_DIR}/*.t ) ; do
    local tmp_output=tmp
    if ! ./${bin} < ${test_file} > ${tmp_output} ; then
      log_error "\e[31mFailed to run test\e[0m"
      return 1
    fi
    local file_line_cnt=; file_line_cnt=$(cat ${test_file} | wc -l | sed -e 's/ *//g')
    local answer_file=${test_file%.*}.a
    if ! diff -u ${answer_file} ${tmp_output} ; then
      log_error "\e[31mFailed to check test ${test_file}.\e[0m"
      return 1
    fi
    log_info "\e[34m${test_file}\e[0m, lines=${file_line_cnt} \e[32mOK\e[0m"
  done

  log_info "\e[32mStage #4 Benchmarking...\e[0m"
  if ! make benchmark ; then
   log_info "\e[31mFailed to compile benchmark.\e[0m"
   return 1
  fi
  local benchmark_bin=./benchmark
  for test_file in $( ls ${TESTS_DIR}/*.t ) ; do
   log_info "\e[32mRunning ${test_file}\e[0m"
   if ! ${benchmark_bin} < ${test_file} ; then
     log_error "\e[31mFailed to run ${benchmark_bin} for ${test_file}.\e[0m"
     return 1
   fi
  done
}

# $1 -- первый аргумент
# %2 -- второй аргумент
# $@ -- все аргументы
main $@