cls
del main.exe
gcc ./test/*.c -o ./main 


if exist main.exe (
    main
) else (
    pause
)
