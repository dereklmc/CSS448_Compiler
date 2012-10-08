CSS448\_Compiler
===============

Repository for compiler project for UWB CSS 448, Autumn 2012

To build, use make

To run, either:
bin/tokenizer.run
sh runTokenizer.sh

Sample \*.p test files are in sample/. Each input file has a corresponding output file, with the extension .output. If a .p file is added to sample/, add a corresponding .output file to sample/.

To test files in sample/, run:

sh runTests.sh

This will display a diff of any differences from the ideal.
