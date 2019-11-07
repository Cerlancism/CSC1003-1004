cls
del probability.exe
gcc -o probability ./src/*.c


if exist probability.exe (
    probability
) else (
    pause
)
