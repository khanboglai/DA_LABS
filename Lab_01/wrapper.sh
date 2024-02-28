#! /usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail

readonly TEST_DIR=tests


function log_info() {
    local message=${1}
    _log "INFO" "${message}"
}


function log_error() {
    local message=${1}
    _log "ERROR" "${message}"
}


function _log() {
    local level=${1}
    local message=${2}
    local now=$( date +"%Y-%m-%d %H:%M:%S" )
    echo "[${level}] [${now}] ${message}"
}

function main() {
    log_info "Start"

    log_info "Stage #1. Compiling..."
    if ! make; then
        log_error "Failed to compile"
        return 1
    fi

    log_info "Stage #2. Test generating..."

    rm -rf ${TEST_DIR}
    mkdir ${TEST_DIR}

    if ! ./test_generator.py ${TEST_DIR}; then
        log_error "Failed to generate test"
        return 1
    fi

    log_info "Stage #3. Checking..."
    
    for test_file in $( ls ${TEST_DIR}/*.t ) ; do
        local tmp_output=tmp

        if ! ./lab1 < ${test_file} > ${tmp_output} ; then
            log_error "Failed to run test"
            return 1
        fi

        local file_line_cnt=; file_line_cnt=$(cat ${test_file} | wc -l | sed -e 's/ *//g')
        local answer_file=${test_file%.*}.a

        if ! diff -u ${tmp_output} ${answer_file} ; then
            log_error "Failed to check test ${test_file}."
            return 1
        fi
        log_info "${test_file}, lines=${file_line_cnt} OK"
    done
}

main