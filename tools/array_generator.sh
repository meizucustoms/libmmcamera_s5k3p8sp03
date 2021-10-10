#!/bin/bash

# MeizuCustoms team (c) 2021
# Needs preparation before use

shit="$(cat ~/got.txt | sed 's/h//g')"
array=($shit)

for ((a=0;$a<${#array[@]};a+=3)); do
    echo -ne '  {'

    for ((b=0;$b<=2;b++)); do
        mynum="$(expr $a + $b)"

        if [ $(expr $(expr $mynum + 1) % 3) = 0 ]; then
            printf "0x%02X" 0x${array[$mynum]}
            break
        else
            if [ "$(wc -m <<< ${array[$mynum]})" != 5 ] && [ "$(expr $(expr $mynum + 1) % 3)" = 1 ]; then
            printf "0x%04X, " 0x${array[$mynum]}
            else
            printf "0x%02X, " 0x${array[$mynum]}
            fi
            continue
        fi
    done

    echo -ne "}, \\"
    echo
done