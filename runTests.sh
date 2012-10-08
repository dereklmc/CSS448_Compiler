mkdir bin
make

TEST_INPUT=sample/*.p
for testInput in $TEST_INPUT
do
    echo "Tokenizing " $testInput "..."
    ./bin/analyzer.run < $testInput > test.out
    testOutput=${testInput/\.p/\.output}
    diff -wBs --strip-trailing-cr test.out $testOutput
    echo "========================================"
done
