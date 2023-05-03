rm -f output.txt
touch output.txt

./newspaper utf8_text.txt output.txt 3 40 21 10

python tester.py output.txt expected_output.txt