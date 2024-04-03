# Assignment 3
The purpose of this lab is to simulate the Game of Life, invented by John Horton Conway. The overarching idea is that we have a universe that contains a two-dimensional grid of cells, and each cell follows three rules for each generation of the universe: any live cell with two or three neighbors survives, any dead cell with exactly three live neighbors becomes a live cell, and all other cells die due to either loneliness or overcrowding. The universe can be toroidal.

## Build
    $ make all

## Run
    $ ./life

### Command-line options
    -t: Specify that the Game of Life is to be played on a toroidal universe.
    -s: Silence ncurses.
    -n <generations>: Specify the number of generations that the universe goes through. The default number of generations is 100.
    -i <input>: Specify the input file to read in order to populate the universe. The default input file is stdin.
    -o <output>: Specify the output file to print the final state of the universe to. The default output file is stdout.

## Clean
    $ make clean

## Format
    $ make format
