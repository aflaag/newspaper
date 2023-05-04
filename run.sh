#!/usr/bin/sh

# FAI LA FUNZIONE FATTA BENE CHE RAPPRESENTA UN SINGOLO TEST
# E FAGLI STAMPARE IL NOME DEL FILE AL POSTO DEL NUMERO

flush_output() {
    rm -f output.txt
    touch output.txt
}

flush_output

echo -n "Test 1: "
./newspaper sample_inputs/utf8_prof_in.txt output.txt 3 40 21 10
python tester.py output.txt expected_outputs/utf8_prof_out.txt

# flush_output

# echo -n "Test 2: "
# ./newspaper sample_inputs/lorem_long_in.txt output.txt 3 40 21 10
# python tester.py output.txt expected_outputs/lorem_long_out.txt

# flush_output

# echo -n "Test 3: "
# ./newspaper sample_inputs/lorem_short_in.txt output.txt 3 4 5 1
# python tester.py output.txt expected_outputs/lorem_short_out.txt

# flush_output

# echo -n "Test 4: "
# ./newspaper sample_inputs/capo_in.txt output.txt 3 7 15 1
# python tester.py output.txt expected_outputs/capo_out.txt

# flush_output

# echo -n "Test 5: "
# ./newspaper sample_inputs/spacing_in.txt output.txt 3 7 18 1
# python tester.py output.txt expected_outputs/spacing_out.txt

# exit

# flush_output

# echo -n "Test 6: "
# ./newspaper sample_inputs/spacing2_in.txt output.txt 3 7 18 1
# python tester.py output.txt expected_outputs/spacing2_out.txt

# flush_output

# echo -n "Test 7: "
# ./newspaper sample_inputs/super_short_in.txt output.txt 3 7 21 1
# python tester.py output.txt expected_outputs/super_short_out.txt

# flush_output