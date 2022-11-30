# Arduino Snake

 ## Components used 
    * Arduino MEGA 2560 R3
    * VGA Cable 
    * 4x4 Button Matrix (HW-834)

 ## Schematic
 ![VGA Schematic](./docs/schematic.png)
 
 ## Project functionality 
 The main Arduino library used for this project is VGAx that generates the corresponding frequency for the HS and VS signals and helps with the screen drawing and rendering process.
 * This is an early version 
 * The snake can eat only one type of food:  
    * Apple: enlarges the snake 
 * The food is generated randomly based on the snake position 
 * Death of the snake can occur only in one scenario: 
    * Eating its own tail 
 * The texture of the snake and food can be changed easily by using some code generation tools from the VGAx library 

 ## Useful links
 https://github.com/smaffer/vgax 

 https://learn.digilentinc.com/Documents/269 

 https://simple-circuit.com/interfacing-arduino-vga-example/ 

 http://www.sandromaffiodo.com/ 
