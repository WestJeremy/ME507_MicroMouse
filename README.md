# ME507_MicroMouse
This repository contains files for a custom PCB and c based code for a micromouse project for ME507 at Cal Poly SLO. This README.md file covers the task diagram and finite state implimentation used for this project.

## TASK DIAGRAM

## FINITE STATE MACHINES
### FloodFill Task
The following finite state machine is implimented for the task that impliments the floodfill algorithm for maze solving. The task begins in an idle state and begins maze solving once it recieves feedback from a start button. States 1-4 perform floodfill depending on the orientation of the micromouse. Micromouse competitions typically start with the robot in the top left corner facing towards the right. so this is what we designated state 1 to be. from there, once floodfill has been completed for a given cell the micromouse is oriented to the neighboring cell with the lowest manhatten distance and the state transitions accordingly.
![State Diagrams-1](https://github.com/WestJeremy/ME507_MicroMouse/assets/98365124/ac6ae4f0-c1ca-4aad-91e7-937100d98f6f)
