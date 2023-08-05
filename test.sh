#!/bin/bash

assert() {
    expected="$1"
    input="$2"

    ./minicc "$input" > tmp.s
    /bin/cc -o tmp tmp.s -fuse-ld=mold
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

assert 0 0
assert 42 42
assert 21 "5+20-4"
assert 41 " 12 + 34 - 5 "

echo OK
