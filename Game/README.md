# FPGA-Racing The Game
FPGA Racing is a multiplayer Go Kart Racing game designed in Unreal Engine 5 (C++) for Imperial College London Information Processing Final Project. The project consists of an FPGA accelerometer as the controller, Amazon AWS dedicated server with Lambda functions for login and matchmaking, and a fully replicated online multiplayer racing game. The theme was inspired by Mario Kart and Sonic and Sega All Stars Racing.

## How to Play

1. **Download and Install:** Download the Windows.zip file in the releases tab, unzip the file and run the FPGARacing.exe.
2. **Login:** You will then be prompted with a login screen *Note in order to use your personal account you must first be added to our auth user login database*.
3. **Select Game Mode:** In order to play online first select the multiplayer button, then on create session, you could also instead select join session to find current active sessions. Conversely you could select single player and time trials or grand prix to play offline *offline game mode may have bugs due to time being spent on the multiplayer aspect of the game*
4. **Race to Victory:** After successfully joining a session ready up to start the game. You can use the FPGA files coupled with the python script provided in the FPGA section of the repo, or you could play using the keyboard. W - forward, S - Backwards, A - Left, D - Right, Q - Drift, E - Use Item. 


## Features

### Custom Movement Physics simulation

For this project I designed and implemented myself custom vehicle physics to simulate the feel / look of a Go kart. Every frame we perform a raycast from each bottom corner of the vehicle body going downwards in the local vehicle space for a certain predefined length, in this case it was 60cm. We then calculate a compression ratio between 1 and 0 based off of the raycast hit distance, if it did hit anything that is. We then apply a force based  on the compression ratio in the upwards vehicle space at that location, this allows us to simulate the suspension of a car. One problem with this was that the car would keep bouncing up and down, to fix this I added a dampening force to act in the opposite direction to the Z-axis velocity. 

Next was the driving force. This was not as simple as just applying a force in the same direction the vehicle is facing, first we use the stored impact normal that was calculated when simulating the suspension of the car, we then project our forward vector onto that impact normal in order to find the true driving direction along the same vector as the floor. We can then apply a force in that direction for the duration the 'forward button is pressed'. For steering we apply a torque to the vehicle which allows us to turn. However I added simple linear interpolation to the steering so that it felt more like a car would, gradual turning rather than abrupt.

There were lots of smaller forces that helped make the kart act more 'arcadey' this was things like turning traction, restitution, air resistance, centre of mass and body tilt. 

This was the basics of vehicle movement however alot of fine tuning with values was needed in order for the gameplay to feel nice. All the code can be found in the FPGARacingMovementComponent header and source file here[].

### Multiplayer

The most exciting aspect about the game is its online capabilities, in FPGA Racing you are able to play online against up to 10 people from around the globe. When designing this project the team initially chose to use AWS GameLift to host the servers as well as use Lambda functions for login and matchmaking, this satisfied our goals for this project as it was versatile, scalable and feature heavy. Near the end of the project we then made the decision to move to Epic Online Services. The reasoning for this was so that we did not have to incurr any costs from keeping the AWS EC2 server instance running, we could play the game indefinetly as we moved to a ListenServer architecture meaning less server load, less costs and greater game lifespan, also it made sense to use Epic Online Services as they integrate perfectly with the architecture of Unreal Engine and we could make use of all their free features and open source engine.

In order to make a multiplayer racing game we had to replicate player movement, player stats and game state. In order to implement this I implemented client prediction. What this means is that whenever a player presses an input key instead of sending that data to the server first and waiting for the server to respond with a validation we simulate the move locally on our machine and at the same time send a server RPC with player transform and inputs, the server then simulates the move and corrects the player if there were any large deltas between the two simulations, this is called Server-Authoritative architecture with Client Prediction. The reason I did this was to prevent cheating in the game. Now if the player tried to give himself 2 x speed the server would stop the player, this helps with fairness and is how larger games like Mario Kart and Need for speed implement multiplayer.

### Gameplay

In this game I added a working lap system where a full lap must be completed without taking shortcuts in order to advance, a leaderboard screen that pops up at the end of the game indicating everyones placements.
Boost pads where the player can gain a temporary speed boost, and is of course fully replicated.
Item boxes where upon overlap the player is given a random item in an item list *currently only mushroom and banana items are implemented*.
A mushroom item which gives the player a temporary speed boost on use.
A banana, when dropped stops any player that overlaps it.
And to tie it all up a Heads up Display which updates with all this information in real time.

**Note:** This Game is a work in progress. Stay tuned for updates and new features! If you encounter any issues or have suggestions for improvement, please let us know by opening an issue. Happy racing! 🏎💨
