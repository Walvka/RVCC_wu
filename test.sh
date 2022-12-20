assert () {
    expected="$1"
    input="$2"

    ./rvcc $input > tmp.s || exit

    riscv64-unknown-elf-gcc -static tmp.s -o tmp
    qemu-riscv64 -L $RISCV/sysroot ./tmp

    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input ==> $actual"
    else
        echo "$input ==> $expected expected, but got $actual"
        exit 1
    fi
}

assert 0 0
assert 1 1
assert 2 2
assert 3 3
assert 4 4
assert 13 4+12-3
assert 67 64-5+8