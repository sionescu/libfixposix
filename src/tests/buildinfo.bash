# Test framework

ok() {
    ((++TEST_COUNTER))
    echo "ok ${TEST_COUNTER}"
}

fail() {
    local errmsg=${1}
    [[ -n ${errmsg} ]] && echo "${errmsg}" >&2
    ((++TEST_COUNTER))
    echo "not ok ${TEST_COUNTER}"
    exit 1
}

TEST_COUNTER=0
echo 1..3

# Actual tests.

testbin="src/tests/buildinfo"

out=$(${testbin} 2>&1)
ret=$?
if [[ ${ret} != 0 ]]; then
    fail "${testbin} returned ${ret}, expected 0"
else
    ok
fi

release="0.4.2"
vcsid=$(git rev-parse --short HEAD)

got_release=$(echo "${out}" | cut -d \; -f 1)
got_vcsid=$(echo "${out}" | cut -d \; -f 2)

if [[ ${release} == ${got_release} ]]; then
    ok
else
    fail "Got release ${got_release}, expected ${release}"
fi

if [[ ${vcsid} == ${got_vcsid} ]]; then
    ok
else
    fail "Got VCSID ${got_vcsid}, expected ${vcsid}"
fi

exit 0
