cls
del linear_regression.exe
gcc -o linear_regression ./src/*.c


if exist linear_regression.exe (
    linear_regression
) else (
    pause
)
