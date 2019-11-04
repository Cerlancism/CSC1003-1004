cls
del probability.exe
gcc -ansi -o probability ./src/*.c


if exist probability.exe (
    probability
) else (
    pause
)
