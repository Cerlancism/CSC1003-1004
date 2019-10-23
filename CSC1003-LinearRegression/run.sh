clear
rm regression.
gcc -o regression ./src/*.c

if [ -e regression ]; then
    ./regression	
else
	read -p ""
fi
