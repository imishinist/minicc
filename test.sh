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

assert 0 '0;'
assert 42 '42;'
assert 21 "5+20-4;"
assert 41 " 12 + 34 - 5 ;"
assert 47 '5 + 6 * 7;'
assert 15 '5*(9 - 6);'
assert 4 "(3 + 5) / 2;"

assert 10 '5 * +2;'
assert 10 '-10+20;'
assert 10 '- - +10;'

assert 0 '0 == 1;'
assert 1 '42 == 42;'
assert 1 '0 != 1;'
assert 0 '42 != 42;'

assert 1 '0 < 1;'
assert 0 '1 < 1;'
assert 0 '2 < 1;'
assert 1 '0 <= 1;'
assert 1 '1 <= 1;'
assert 0 '2 <= 1;'

assert 1 '1 > 0;'
assert 0 '1 > 1;'
assert 0 '1 > 2;'
assert 1 '1 >= 0;'
assert 1 '1 >= 1;'
assert 0 '1 >= 2;'

assert 1 'a = 1; a;'
assert 6 'a = 1; b = 2 + 3; a + b;'
assert 10 'a = 2; z = 2 + 3; a * z;'

assert 6 'foo = 1; bar = 2 + 3; foo + bar;'

assert 5 'return 5;'
assert 10 'a = 2; z = 2 + 3; return a * z;'
assert 6 'foo = 1; bar = 2 + 3; return a = foo + bar;'

echo OK
