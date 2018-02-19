#!/usr/bin/env bats
# -*- shell -*-

testbin="./src/tests/buildinfo"
verfile=${BATS_TEST_DIRNAME}/../../VERSION

@test "should return correct status code" {
    run "${testbin}"
    [ "${status}" -eq 0 ]
}

@test "should print build info" {
    run "${testbin}"

    got_release=$(echo "${output}" | cut -d \; -f 1)
    got_vcsid=$(echo "${output}" | cut -d \; -f 2)

    echo $(env) >&2

    release=$(< "${verfile}")
    [ ${release} == "${got_release}" ]

    vcsid=$(git rev-parse --short HEAD 2>/dev/null)
    [ x"${vcsid}" == x"${got_vcsid}" ]
}
