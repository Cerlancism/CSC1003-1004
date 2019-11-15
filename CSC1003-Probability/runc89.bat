cls
del probability.exe
gcc ./src/*.c -o probability -lpthread -lm -ansi -Wall -Wextra -pedantic

if exist probability.exe (
    probability %*
) else (
    pause
)
