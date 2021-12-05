#!/bin/bash

WERR_CFLAGS="\
  -Werror=implicit-function-declaration \
  -Werror=varargs \
"
WERR_CFLAGS_OSX="\
  -Werror=unguarded-availability-new \
"

if [ "${TRAVIS_OS_NAME}" = "osx" ]; then
    WERR_CFLAGS+=" ${WERR_CFLAGS_OSX}";
fi

export WERR_CFLAGS
