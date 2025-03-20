# SimpleMazeGenerator UE5  

This repository contains different maze generation techniques implemented in Unreal Engine using Instanced Static Meshes for efficient rendering.   
The Maze generation is placed into a seperate module to work within Unreal Engine 5.
This project is to be used for education purposes. Architecture and code choices made are for readability and simplicity to extend.  

<img src="https://github.com/josh-hall-griffith/SimpleMazeGenerator/blob/main/images/Backtrace_2_Maze_Gen.PNG" width="512">


## Instructions  

**Clone the repository**    

git clone <your-repo-url> 
Checkout a branch to see a specific implementation:  


### Available branches:  
 - main (general development branch)  (Recursive Backtracking Maze & Random Wall Rotation Maze)  
 - Backtrace_Maze (Recursive Backtracking Maze & Random Wall Rotation Maze)  
 - Turn_Maze (Random Wall Rotation Maze)  

# Turn Maze
Implementation of a turn-based algorithm where each wall in a grid has a random binary choice to rotate 90 degrees.  

 - Does not guarantee a completable maze, but produces unique and interesting patterns.  
 - Uses Instanced Static Meshes for optimized rendering.  
 - Controlled by grid-based logic with a randomization step.
<img src="https://github.com/josh-hall-griffith/SimpleMazeGenerator/blob/main/images/Turn_Maze_Gen.PNG" width="512">

# Backtrace Maze  
Implementation of a recursive backtracking algorithm that generates a fully completable maze.  

 - Ensures a single connected path without isolated areas.  
 - Uses depth-first search (DFS) with recursion to create a winding, organic maze.  
 - Walls between connected cells are removed to form the final path.  
 - Entry and exit points are automatically generated on opposite sides of the maze.  
 - Uses Instanced Static Meshes for performance optimization.  
<img src="https://github.com/josh-hall-griffith/SimpleMazeGenerator/blob/main/images/Backtrace_1_Maze_Gen.PNG" width="512">

Settings for the Actor:  
<img src="https://github.com/josh-hall-griffith/SimpleMazeGenerator/blob/main/images/Turn_Maze_Gen_Settings.PNG" width="512">
