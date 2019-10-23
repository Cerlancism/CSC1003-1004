cls
del regression.exe
gcc -o regression ./src/*.c


if exist regression.exe (
    regression
) else (
    pause
)
