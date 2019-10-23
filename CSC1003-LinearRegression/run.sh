clear
rm regression
gcc -o regression ./src/*.c -lm

if [ -e regression ]; then
    ./regression	
else
	read -p ""
fi
