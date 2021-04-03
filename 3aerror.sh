#!/bin/bash
k=1;
for ((i=1; i<24; i=i+1))
do
    echo -e "i=$i\n"
    if [[ $j -lt 10 ]]; then
        wget -O "Koleksi_0$k.jpg" -o ->> Foto.log https://loremflickr.com/320/240/kitten
    else
        wget -O "Koleksi_$k.jpg" -o ->> Foto.log https://loremflickr.com/320/240/kitten
    fi
   
    files="$( find -type f )"
    for file1 in $files; do
        for file2 in $files; do
            # echo "cek $file1 and $file2"
            if [[ "$file1" != "$file2" && -e "$file1" && -e "$file2" ]]; then
                if diff "$file1" "$file2" > /dev/null; then
                    #echo "$file1 dan $file2 duplikat"
                    rm -v "$file2"
                    k=$[$k-1]
                fi
            fi
        done
    done
    k=$[$k+1]
done
