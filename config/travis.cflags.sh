#!/bin/bash

WARN_CFLAGS="\
  -O2 \
  -Wall -Wextra \
  -fno-common \
  -Wshadow \
  -Wpointer-arith \
  -Wformat=2 \
  -Wswitch-default \
  -Wswitch-enum \
  -Wcast-align \
  -Wcast-qual \
  -Wconversion \
  -Wpointer-arith \
  -Wbad-function-cast \
  -Wstrict-overflow=5 \
  -Winline \
  -Wundef \
  -Wnested-externs \
  -Wunreachable-code \
  -Wfloat-equal \
  -Wstrict-aliasing=2 \
  -Wredundant-decls \
  -Wold-style-definition \
  -Werror=implicit-function-declaration \
  -Werror=strict-prototypes \
  -Werror=missing-prototypes \
  -Werror=missing-braces \
  -Werror=missing-field-initializers \
  -Werror=varargs \
"

if [[ ${TRAVIS_OS_NAME} = "osx" ]]; then
    WARN_CFLAGS+=" -Werror=unguarded-availability-new";
fi

if [[ ${CC_FOR_BUILD} == "gcc" ]]; then
    WARN_CFLAGS+=" -Wlogical-op";
fi

export WARN_CFLAGS
