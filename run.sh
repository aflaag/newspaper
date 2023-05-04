#!/usr/bin/sh

run_test() {
    rm -f output.txt
    touch output.txt

    echo -n "Test $1: "
    ./newspaper $1 output.txt $3 $4 $5 $6
    python tester.py output.txt $2
}

run_test sample_inputs/utf8_prof_in.txt expected_outputs/utf8_prof_out.txt 3 40 21 10
run_test sample_inputs/lorem_long_in.txt expected_outputs/lorem_long_out.txt 3 40 21 10
run_test sample_inputs/lorem_short_in.txt expected_outputs/lorem_short_out.txt 3 4 5 1
run_test sample_inputs/capo_in.txt expected_outputs/capo_out.txt 3 7 15 1
run_test sample_inputs/spacing_in.txt expected_outputs/spacing_out.txt 3 7 18 1
run_test sample_inputs/spacing2_in.txt expected_outputs/spacing2_out.txt 3 7 18 1
run_test sample_inputs/super_short_in.txt expected_outputs/super_short_out.txt 3 7 21 1
run_test sample_inputs/utf8_ending_in.txt expected_outputs/utf8_ending_out.txt 3 7 4 1