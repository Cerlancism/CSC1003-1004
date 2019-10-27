# Documentation CSC1003 Assignment 1
**__Linear Regression__**

## Problem Definition
TODO

## Problem Analysis
TODO

## Pseudo Code
`regression`
```
TODO
```
`file opening`
```
TODO
```
`console plotting`
```
TODO
```

## Source Code
This assignment is written in ANSI C(C89).  
All source files (`.c` `.h`) in `./src/` folder are originally written by the team.  
They contain the algorithms for calculating the regression line equation, file operations, plotting graph as ASCII art on the console as well as an optional feature to open and plot the graph on a additional program called [GNU Plot]("http://www.gnuplot.info/").

## Dependencies
- C Standard Library
- C Maths Library
- unistd
- GNU Plot (Optional)

## Compiling
This is compiled and tested using GCC.  
  
> `gcc ./src/*c -o regression -lm`

### Platforms Supported
- Windows
- MAC OS
- Raspbian (Raspberry PI)

## Operating the Program
The program by default requires a data input file of 10000 lines of comma seperated value tuple named `Group1_8.txt` (Assigned by the Lecturer) in the same directory as the regression executable.

### Executing the program with default configuration on command line
In the current directory: 
> `./regression`

#### Expected Output on Standard Out
```
File: Group1_8.txt, Lines: 10000, Console Plot Height: 20, Console Plot Width: 100
-h to display command line options
Min Y: -9.626100 , Max Y: 25.362000
y = 0.514091 x + 2.315605
Correlation coefficient: 0.596192
Coefficient of determination: 35.544506 %
Standard error of estimate: 3.996875
           _____________________________________________________________________________________________________
  25.000  |        |                                                                  X         X X             |
  23.250  |        |                                                               X    X  X   X  X             |
  21.500  |        |                                              X                XX       X    XX             |
  19.750  |        |                      X         X  X      X     XX X   XX XXX  X XXXX XXXXXXXXXX            |
  18.000  |        |              X          XX  X  XXXXXX XX   X XXXXXX XXXXXXXXXXXXXXXXXXXXXXXXXXX            |
  16.250  |        |          X   X X X X X     XXXX X  X XXXXXX  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX            |
  14.500  |        X X  X  X   X X XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX       **** |
  12.750  |        |XXXX XXXX XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*************     |
  11.000  |        XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX**************XXXXXX            |
   9.250  |        XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*************XXXXXXXXXXXXXXXXXXXX            |
   7.500  |        XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX**************XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX            |
   5.750  |        XXXXXXXXXXXXXXXXXXXX**************XXXXXXXX.------------------.XXXXXXXXXXXXXXXXXXX            |
   4.000  |        XXXXXXX*************XXXXXXXXXXXXXXXXXXXXXX| y = 0.51x + 2.32 |XXXXXXXXXXXXXXXXX X            |
   2.250  | **************XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'------------------'XXXX XXX XXXXXXX X             |
   0.500  |*-------XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX-XXXXXXX-XX--XX----------X------------- |
  -1.250  |        XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX XX XX  XXXX XX  X        X                     |
  -3.000  |        XXXXXXXXXXXXXXXXXXXXXXXX XX XXXXXXXXX XX XXX      X     X  XXX                               |
  -4.750  |        |XXXXXXXXXXXXXX XX XXX X X   X    X      X   X    X                                          |
  -6.500  |        XXXXXXXXX XX  X   X     X     X           X          X                                       |
  -8.250  |        |  X X         X                                                                             |
 -10.000  |________|______X_____________________________________________________________________________________|
           |-2.000   |0.500    |3.000    |5.500    |8.000    |10.500   |13.000   |15.500   |18.000   |20.500

Type < > ^ v + - to pan and zoom the graph. Current scaling: 1.00
_
```
Entering a combination of `< > ^ v + -` can be used to control the plot view.

### Command Line Arguments
This program accepts command line options to configure how it runs, such as using a different data file, changing the console display height and width of the ASCII art plotting.

| Option    | Description                                   | Default Value | Example           |
|:----:     |:----                                          |:----          |:-----             |
| `-f`      | Name of the data file                         | Group1_8.txt  | `-f Group9_15.txt`|
| `-l`      | Amount of lines to scan in the data file      | 10000         | `-l 1000`         |
| `-c`      | Columns of the console for the ASCII plotting  | 100           | `-c 200`          |
| `-r`      | Rows of the console for the ASCII plotting    | 20            | `-r 40`           |  

So for example to execute the program with the data file `Group9_15.txt` with a maximised console which gives about 200 columns and 50 rows of spacing to plot the ASCII graph:  

> `./regression -f Group9_15.txt -c 200 -r 50`

### Additional Feature - Launching GNU Plot
If the user has GNU Plot installed and added to the environment PATH, this program will automatically ask the user if to launch GNU Plot to display the graph.

> Executing the program with GNU Plot installed
```
File: Group1_8.txt, Lines: 10000, Console Plot Height: 20, Console Plot Width: 100
-h to display command line options
Min Y: -9.626100 , Max Y: 25.362000
y = 0.514091 x + 2.315605
Correlation coefficient: 0.596192
Coefficient of determination: 35.544506 %
Standard error of estimate: 3.996875
Looks like you have GNU Plot installed, do you want to open it? Y/N
(This program will still alternatively plot on console as ASCII art)
```
Entering `Y` will pop up an additional Window for GNU Plot.
#### Installing GNU Plot
##### Windows
TODO
##### MAC OS
TODO
