mkdir bin
make

if [ $# -lt 1 ];
then
	echo "usage runTokenizer.sh <input_file> <optional output file>"
fi

if [ $# -gt 1 ];
then
	outputFile=$2
else
	outputFile="test.out"
fi

echo "Tokenizing $1 to $outputFile..."
./bin/analyzer.run < $1 > $outputFile
