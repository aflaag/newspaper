#!/usr/bin/sh

TOTAL_TESTS=0
PASSED_TESTS=0

run_test() {
    rm -f output.txt
    touch output.txt

    printf "Test $1: "
    ./newspaper $1 output.txt $3 $4 $5 $6

    if command diff output.txt $2 &> /dev/null
    then
        echo "passed ✅"
        let PASSED_TESTS++
    else
        echo "not passed ❌"
    fi

    let TOTAL_TESTS++
}

run_test sample_inputs/utf8_prof_in.txt expected_outputs/utf8_prof_out.txt 3 40 21 10
run_test sample_inputs/lorem_long_in.txt expected_outputs/lorem_long_out.txt 3 40 21 10
run_test sample_inputs/lorem_short_in.txt expected_outputs/lorem_short_out.txt 3 4 5 1
run_test sample_inputs/capo_in.txt expected_outputs/capo_out.txt 3 7 15 1
run_test sample_inputs/spacing_in.txt expected_outputs/spacing_out.txt 3 7 18 1
run_test sample_inputs/spacing2_in.txt expected_outputs/spacing2_out.txt 3 7 18 1
run_test sample_inputs/super_short_in.txt expected_outputs/super_short_out.txt 3 7 21 1
run_test sample_inputs/utf8_ending_in.txt expected_outputs/utf8_ending_out.txt 3 8 4 10
run_test sample_inputs/nothing_in.txt expected_outputs/nothing_out.txt 3 3 4 10
run_test sample_inputs/makefile_in.txt expected_outputs/makefile_out.txt 3 8 16 10

printf "\n[$PASSED_TESTS/$TOTAL_TESTS] tests passed.\n"