cls
del probability.exe
gcc ./src/*.c -o probability -lpthread


if exist probability.exe (
    probability
) else (
    pause
)
