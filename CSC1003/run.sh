clear
rm linear_regression.exe
gcc -o linear_regression ./src/*.c

if [ -e linear_regression.exe ]; then
    ./linear_regression.exe	
else
	read -p ""
fi
