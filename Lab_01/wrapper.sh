#! /usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail

readonly TEST_DIR=tests


function log_info() {
    local message=${1}
    _log "\e[34mINFO\e[0m" "${message}"
}


function log_error() {
    local message=${1}
    _log "\e[31mERROR\e[0m" "${message}"
}


function _log() {
    local level=${1}
    local message=${2}
    local now=$( date +"%Y-%m-%d %H:%M:%S" )
    echo -e "[${level}] [${now}] ${message}"
}

function main() {
    log_info "Start"

    log_info "\e[32mStage #1. Compiling...\e[0m"
    if ! make; then
        log_error "\e[31mFailed to compile\e[0m"
        return 1
    fi

    log_info "\e[32mStage #2. Test generating...\e[0m"

    rm -rf ${TEST_DIR}
    mkdir ${TEST_DIR}

    if ! ./test_generator.py ${TEST_DIR}; then
        log_error "\e[31mFailed to generate test\e[0m"
        return 1
    fi

    log_info "\e[32mStage #3. Checking...\e[0m"
    
    for test_file in $( ls ${TEST_DIR}/*.t ) ; do
        local tmp_output=tmp

        if ! ./lab1 < ${test_file} > ${tmp_output} ; then
            log_error "\e[31mFailed to run test\e[0m"
            return 1
        fi

        local file_line_cnt=; file_line_cnt=$(cat ${test_file} | wc -l | sed -e 's/ *//g')
        local answer_file=${test_file%.*}.a

        if ! diff -u ${tmp_output} ${answer_file} ; then
            log_error "\e[31mFailed to check test\e[0m ${test_file}."
            return 1
        fi
        log_info "${test_file}, lines=${file_line_cnt} \e[32mOK\e[0m"
    done

    log_info "\e[32mStage #4. Benchmark...\e[0m"
    if ! ./bench1 < ${TEST_DIR}/01.t; then
        log_error "\e[31mFailed to run benchmark\[0m"
        return 1
    fi

    log_info "\e[32mbenchmark calculated\e[0m"
}

main