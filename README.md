# Multi-threaded-Mandelbrot

To compile the source use a C++11 compiler and link with SDL2!  
https://www.libsdl.org/download-2.0.php

## Controls
Use the arrow keys to navigate the Mandelbrot.  
Use the plus and minus keys on the top row to zoom in / out.  

M to mix 100 random lines of the Mandelbrot  
S to preform std::shuffle on the lines of the Mandelbrot  
B to reorder the Mandelbrot using bubble sort  
R redraws the image and prints the time taken to the console  
C clears all tasks in the queue  

1 - 8 on the top row change the number of threads used  

1 - 4 on the numpad change the dispersion of the lines as they are drawn  
