#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
    "-e s ../common/test_0_grep.txt VAR"
    "-e for s21_grep.c s21_grep.h Makefile VAR"
    "-e for s21_grep.c VAR"
    "-e for -e ^int s21_grep.c s21_grep.h Makefile VAR"
    "-e for -e ^int s21_grep.c VAR"
    "-e regex -e ^print s21_grep.c VAR -f ../common/test_ptrn_grep.txt"
    "-e while -e void s21_grep.c Makefile VAR -f ../common/test_ptrn_grep.txt"
)

declare -a extra=(
    "-n for ../common/test_1_grep.txt ../common/test_2_grep.txt"
    "-n for ../common/test_1_grep.txt"
    "-n -e ^\} ../common/test_1_grep.txt"
    "-c -e \/ ../common/test_1_grep.txt"
    "-ce ^int ../common/test_1_grep.txt ../common/test_2_grep.txt"
    "-e ^int ../common/test_1_grep.txt"
    "-nivh = ../common/test_1_grep.txt ../common/test_2_grep.txt"
    "-e"
    "-ie INT ../common/test_5_grep.txt"
    "-e char ../common/test_1_grep.txt ../common/test_2_grep.txt"
    "-ne = -e out ../common/test_5_grep.txt"
    "-iv int ../common/test_5_grep.txt"
    "-in int ../common/test_5_grep.txt"
    "-c -l aboba ../common/test_1_grep.txt ../common/test_5_grep.txt"
    "-v ../common/test_1_grep.txt -e ank"
    "-noe ) ../common/test_5_grep.txt"
    "-l for ../common/test_1_grep.txt ../common/test_2_grep.txt"
    "-o -e int ../common/test_4_grep.txt"
    "-e = -e out ../common/test_5_grep.txt"
    "-noe ing -e as -e the -e not -e is ../common/test_6_grep.txt"
    "-e ing -e as -e the -e not -e is ../common/test_6_grep.txt"
    "-c -e . ../common/test_1_grep.txt -e '.'"
    "-l for no_file.txt ../common/test_2_grep.txt"
    "-f ../common/test_3_grep.txt ../common/test_5_grep.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > test_s21_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $t"
    else
      (( FAIL++ ))
      echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $t"
    fi
    rm test_s21_grep.log test_sys_grep.log
}

# специфические тесты
# for i in "${extra[@]}"
# do
#     var="-"
#     testing $i
# done

# 1 параметр
for var1 in v c l n h o
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing "$i"
    done
done

# # 2 параметра
# for var1 in v c l n h o
# do
#     for var2 in v c l n h o
#     do
#         if [ $var1 != $var2 ]
#         then
#             for i in "${tests[@]}"
#             do
#                 var="-$var1 -$var2"
#                 testing $i
#             done
#         fi
#     done
# done

# # 3 параметра
# for var1 in v c l n h o
# do
#     for var2 in v c l n h o
#     do
#         for var3 in v c l n h o
#         do
#             if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
#             then
#                 for i in "${tests[@]}"
#                 do
#                     var="-$var1 -$var2 -$var3"
#                     testing $i
#                 done
#             fi
#         done
#     done
# done

# # 2 сдвоенных параметра
# for var1 in v c l n h o
# do
#     for var2 in v c l n h o
#     do
#         if [ $var1 != $var2 ]
#         then
#             for i in "${tests[@]}"
#             do
#                 var="-$var1$var2"
#                 testing $i
#             done
#         fi
#     done
# done

# # 3 строенных параметра
# for var1 in v c l n h o
# do
#     for var2 in v c l n h o
#     do
#         for var3 in v c l n h o
#         do
#             if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
#             then
#                 for i in "${tests[@]}"
#                 do
#                     var="-$var1$var2$var3"
#                     testing $i
#                 done
#             fi
#         done
#     done
# done

echo "\033[31mFAIL: $FAIL\033[0m"
echo "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"
