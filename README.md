# Conway's Game of Life

### *\*Note\* - This code only works on Windows due to use of Windows API's for outputting to the console, requires c++11 and up*
  

## Organization  
- `src` contains all the source files pertinent to implemting Conway's Game of Life
- `src/utils` contains some utility functions used when printing to the console
- `src/patterns` contains examples of how to inherit the `pattern` class from src/pattern.h for use in the simulation
- `example` contains an example `main.cpp` using the **`ConwaysGameOfLife`** class with some basic simulation controls


## Build Instructions For
1. **This repository's example**
    * Files that need to be included in compilation
        * `example/main.cpp`
        * `src/utils/console.cpp`
        * `src/cgol.cpp`
        * `src/pattern.cpp`
        * `src/patterns/*.cpp`
    * Example using g++
        * `g++ -std=c++11 src/cgol.cpp src/pattern.cpp src/utils/console.cpp example/main.cpp -o ConwaysGameOfLife.exe`
2. **General Case**
    * Files that need to be included in compilation
        * `src/cgol.cpp`
        * `src/pattern.cpp`
        * `src/utils/console.cpp`
        * all other custom pattern cpp files

## Custom Implementation  

New patterns can be created by deriving the `Pattern` class in `pattern.h` and initializing the `m_cells` member in the constructor of the new pattern. Look at the existing patterns for help.

## Project Information
 This project runs the rendering and updating of the game in a seperate thread. The example `main` function includes some keyboard controls for the game. Press `left arrow` to toggle start/pause the game. Press `right arrow` to toggle an infomation bar that displays the current generation and thread sleep delay in the top left. Use `up arrow` and `down arrow` to change the value of the thread sleep delay. This in its current state will only run on windows and only when ran inside a windows cmd terminal. There are two ways to achieve this. Double click the binary or open a new terminal, cd to the directory, then run the binary. Running this any other ways will not properly display because it uses windows screen buffers to update and print without scrolling. 
 
 
 ## Learn More
 You can learn more about Conway's Game of Life and cellular automation here  
 [Wikipedia](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)  
 [LifeWiki](https://www.conwaylife.com/wiki/Main_Page)    
 [Playable In Browser Version 1](https://playgameoflife.com/)  
 [Playable In Browser Version 2](http://www.cuug.ab.ca/dewara/life/life.html)  
