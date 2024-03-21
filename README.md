# FPGA-Racing
Group 7's Information Processing Project - An FPGA Racing Game

![FPGA_Racing_Main_Menu](/Misc/Graphics/mainmenu.jpg)

| Contributors           | CID      |
| ----                   |   ---    |
| Gurjan Singh Samra     | 02288570 |
| Idrees Mahmood         | 02061101 |
| Mohammed Tayyab Khalid | 02287685 |
| Raymond La             | 02288579 |
| Arjan Hayre            | 02137475 |
| Alex Seferidis         | 02269571 |

## Demo Video

**Find the final demonstration video [here](https://youtu.be/EwAtyMyi2D0)** 

## Repo Organisation

### FPGA
[The FPGA folder](FPGA) contains all source files for configuring the DE10 Lite FPGA to be used as a controller for our FPGARacing game, once the FPGA is blasted with the project files in Quartus. [script_keyboard.py](FPGA/script_keyboard.py) is then run to read the FPGA's output and emulate a keyboard to use in game. 

### Game 
[The Game folder](Game) contains a full zipped build of the FPGARacing game and it's Unreal engine Project, the build has been configured to run on version of windows 10 or higher. 

### Server
[The Server folder](Server) contains all server configurations and scripts regarding the epic games and AWS api which is used for game login and multiplayer functionality such as game session creation, and real time updates of player position in game.

### Testing
[The Testing folder](Testing) contains performance metrics and tests for various areas in this project such as the performance and latency of the FPGA controller, and server client latency during multiplayer matches in game.

## Project Planning

As mentioned in the report we decided to use a Rapid Application Development (RAD) methodology to account for the short amount of time. We can see this manifested in the testing flow:

![testflow](/Misc/Graphics/TestFlow.png)

We also planned our project using a top down design, while prioritising potential features:

![topdown](/Misc/Graphics/TopDownDesignPNG.png)

Implementing essential features went as planned, with also also implementing some desired features too. See the project timeline below:

![timeline](/Misc/Graphics/ProjectTimeline.png)

