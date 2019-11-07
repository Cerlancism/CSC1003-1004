cls
del probability.exe
gcc ./src/*.c -o probability -ansi -lpthread

if exist probability.exe (
    probability
) else (
    pause
)
