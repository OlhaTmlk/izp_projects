#!/bin/bash
#
# Tests for 2. IZP project [2023]
# author: pseja
# heavily inspired by: _ramb0_
# Usage:
#     (1) Download the gist to your "maze.c" directory: wget https://gist.githubusercontent.com/pseja/12a4e5635d9231649a2d449cb94bc8d8/raw/test_maze.sh
#     (2) Then (for adding permission):                 chmod u+x test_maze.sh
#     (3) Execute this command in "maze.c" directory:   ./test_maze.sh
#     (4) If any test fails, it will output the difference between the expected result and your output with diff command into the diff folder
#     (4) Debug :D

#     For Makefile enjoyers:
#     (1) Add this to your Makefile:              test: maze
#	                                                  @./test_maze.sh
#     (2) Run this command for compile and tests: make test


# color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
NORMAL='\033[0m'

# test variables
test_count=0
correct=0

# compile maze.c just in case
gcc -std=c11 -Wall -Wextra -Werror maze.c -o maze

rm -rf diff

run_test() {
    input_file=$1
    test_arg=$2
    expected_output=$3
    
    echo -n -e "$test_count. Running $input_file, argument ${test_arg}\n"
    
    actual_output=$(./maze $test_arg $input_file)
    
    if [[ "$actual_output" == "$expected_output" ]]; then
        echo -e "${GREEN} [OK] ${NORMAL}"
        correct=$((correct + 1))
    else
        echo -e "${RED}[FAIL]${NORMAL}"
        mkdir -p diff
        echo -e "$expected_output" > ./diff/$test_count:diff_expected.txt
        echo -e "$actual_output" > ./diff/$test_count:diff_yours.txt
        diff ./diff/$test_count:diff_expected.txt ./diff/$test_count:diff_yours.txt
    fi
    test_count=$((test_count + 1))
}

# tests
echo -e "6 7\n1 4 4 2 5 0 6\n1 4 4 0 4 0 2\n1 0 4 0 4 6 1\n1 2 7 1 0 4 2\n3 1 4 2 3 1 2\n4 2 5 0 4 2 5" > test_01.txt

run_test "test_01.txt" "--lpath 6 1" "6,1
6,2
5,2
5,3
5,4
6,4
6,5
6,6
5,6
5,7
4,7
4,6
4,5
5,5
4,5
4,4
3,4
3,3
3,2
4,2
4,1
5,1
4,1
4,2
3,2
3,1
2,1
2,2
2,3
2,4
1,4
1,3
1,2
1,1"

run_test "test_01.txt" "--lpath 6 7" "6,7"

run_test "test_01.txt" "--rpath 6 1" "6,1
6,2
5,2
5,3
5,4
6,4
6,3
6,4
6,5
6,6
5,6
5,7
4,7
4,6
4,5
4,4
3,4
3,5
3,6
3,5
3,4
3,3
3,2
3,1
2,1
2,2
2,3
2,4
2,5
2,6
2,7
3,7"

run_test "test_01.txt" "--rpath 6 7" "6,7"

# tests by cubko
run_test "test_01.txt" "--lpath 3 7" "3,7
2,7
2,6
2,5
2,4
2,3
2,2
2,1
3,1
3,2
3,3
3,4
3,5
3,6
3,5
3,4
4,4
4,5
4,6
4,7
5,7
5,6
6,6
6,5
6,4
6,3
6,4
5,4
5,3
5,2
6,2
6,1"

run_test "test_01.txt" "--rpath 1 1" "1,1
1,2
1,3
1,4
2,4
2,3
2,2
2,1
3,1
3,2
4,2
4,1
5,1
4,1
4,2
3,2
3,3
3,4
4,4
4,5
5,5
4,5
4,6
4,7
5,7
5,6
6,6
6,5
6,4
5,4
5,3
5,2
6,2
6,1"

run_test "test_01.txt" "--test" "Valid"

echo -e "6 7\n1 4 4 2 5 0 6\n1 4 4 0 4 0 2\n1 0 4 0 3 6 1\n1 2 7 1 0 4 2\n3 1 4 2 3 1 2\n4 2 5 0 4 2 5" > test_02.txt
run_test "test_02.txt" "--test" "Invalid"

echo -e "6 7\n1 4 4 2 5 0 6\n1 4 4 0 4 0 2\n1 0 4 0 H 6 1\n1 2 7 1 0 4 2\n3 1 4 2 3 1 2\n4 2 5 0 4 2 5" > test_03.txt
run_test "test_03.txt" "--test" "Invalid"

echo -e "6 7\n1 4 4 2 5 0 6\n1 4 4 0 4 0 2\n1 0 4 0 4 6 1\n1 2 7 1 0 4 2\n3 1 4 2 3 1 2\n4 2 5 0 4 2 5 6" > test_04.txt
run_test "test_04.txt" "--test" "Invalid"

echo -e "10 10\n7 1 2 3 5 0 6 1 4 2\n1 2 3 1 4 0 6 1 2 3\n1 2 1 0 6 1 4 0 2 3\n1 0 0 2 1 0 2 1 4 2\n1 2 1 4 0 2 1 4 4 2\n3 1 4 4 2 1 0 6 1 2\n1 0 4 2 1 2 3 1 2 3\n1 2 3 1 4 0 2 1 0 2\n1 4 2 1 4 2 3 1 0 2\n5 4 4 0 2 1 4 2 5 2" >test_05.txt

run_test "test_05.txt" "--rpath 1 3" "1,3
1,2
2,2
2,1
3,1
3,2
4,2
4,1
5,1
5,2
6,2
6,3
6,4
6,5
7,5
7,6
8,6
8,5
8,4
7,4
7,3
7,2
7,1
6,1
7,1
7,2
8,2
8,1
9,1
9,2
9,3
8,3
9,3
9,2
9,1
8,1
8,2
7,2
7,3
7,4
8,4
8,5
8,6
8,7
9,7
8,7
8,6
7,6
7,5
6,5
6,4
6,3
6,2
5,2
5,1
4,1
4,2
4,3
5,3
5,4
5,5
5,6
6,6
6,7
7,7
6,7
6,8
6,7
6,6
5,6
5,5
4,5
4,6
4,7
5,7
5,8
5,9
5,10
6,10
6,9
7,9
7,8
8,8
8,9
9,9
9,8
10,8
10,7
10,6
9,6
9,5
9,4
10,4
10,3
10,2
10,1
10,2
10,3
10,4
10,5"

run_test "test_05.txt" "--lpath 1 3" "1,3
1,2
2,2
2,1
3,1
3,2
4,2
4,3
4,4
3,4
3,3
2,3
3,3
3,4
3,5
3,4
4,4
4,3
5,3
5,4
5,5
4,5
4,6
3,6
3,7
3,8
3,9
2,9
2,8
1,8
1,9
1,10
2,10
1,10
1,9
1,8
2,8
2,9
3,9
3,8
4,8
4,9
4,10
3,10
4,10
4,9
4,8
3,8
3,7
3,6
4,6
4,7
5,7
5,8
5,9
5,10
6,10
6,9
7,9
7,8
8,8
8,9
8,10
7,10
8,10
8,9
9,9
9,10
10,10
10,9
10,10
9,10
9,9
9,8
10,8
10,7
10,6
9,6
9,5
9,4
10,4
10,5"

# if this is giving you "Invalid", check that you are handling the last line correctly, because there is no new line before the end of the file
echo -e "6 7\n5 6 1 2 1 0 6\n1 4 4 0 4 0 2\n1 0 4 0 4 6 1\n1 2 7 1 0 4 0\n2 1 4 2 3 1 2\n4 2 5 0 0 2 5" > test_06.txt

run_test "test_06.txt" "--test" "Valid"

run_test "test_06.txt" "--rpath 1 3" "1,3
1,4
2,4
2,3
2,2
2,1
3,1
3,2
4,2
4,1
5,1"

run_test "test_06.txt" "--lpath 1 3" "1,3
1,4
2,4
2,5
2,6
1,6
1,5"

run_test "test_06.txt" "--rpath 1 5" "1,5
1,6
2,6
2,5
2,4
1,4
1,3"

run_test "test_06.txt" "--lpath 1 5" "1,5
1,6
1,7
1,6
2,6
2,7
3,7"

run_test "test_06.txt" "--rpath 6 1" "6,1
6,2
5,2
5,3
5,4
6,4
6,3
6,4
6,5"

run_test "test_06.txt" "--lpath 6 1" "6,1
6,2
5,2
5,3
5,4
6,4
6,5
6,6
5,6
5,7
4,7
4,6
4,5
5,5
4,5
4,4
3,4
3,3
3,2
4,2
4,1
5,1"

# if this is giving you an error, check if you are correctly working with whitespace characters
echo -e "  6 7   1 4 4 2 5  0    6 1 4    4 0 4                0 2\n   1 0 4 0  4 6 1\n  1 2 7  1  0 4 2  3 1   4 2 3 1 2\n   4    2 5 0 4 2    5" > test_07.txt

run_test "test_07.txt" "--test" "Valid"

# 43 cells instead of 42
echo -e "6 7\n1 4 4 2 5 0 6\n1 4 4 0 4 0 2\n1 0 4 0 4 6 1\n1 2 7 1 0 4 2\n3 1 4 2 3 1 2\n4 2 5 0 4 2 5 1" > test_08.txt

run_test "test_08.txt" "--test" "Invalid"

# 41 cells instead of 42
echo -e "6 7\n1 4 4 2 0 6\n1 4 4 0 4 0 2\n1 0 4 0 4 6 1\n1 2 7 1 0 4 2\n3 1 4 2 3 1 2\n4 2 5 0 4 2 5" > test_09.txt

run_test "test_09.txt" "--test" "Invalid"

# letters instead of some numbers
echo -e "6 7\n1 4 4 2 5 0 6\n1 4 4 C 4 0 2\n1 0 4 0 4 6 1\n1 A 7 1 0 4 2\n3 1 4 2 3 1 2\n4 2 5 0 4 2 T" > test_10.txt

run_test "test_10.txt" "--test" "Invalid"

# print test results
if [[ "$correct" == "$test_count" ]]; then
    echo -e "\nPassed $correct / $test_count 🎉"
else
    echo -e "\nPassed $correct / $test_count"
fi

# remove temp test files
# if you want individual tests comment line with the test you want to keep
# make sure to later uncomment tho :D
rm test_10.txt
rm test_09.txt
rm test_08.txt
rm test_07.txt
rm test_06.txt
rm test_05.txt
rm test_04.txt
rm test_03.txt
rm test_02.txt
rm test_01.txt
