# Old School RuneScape Duel Simulator
This project allows users to simulate duels between two players in Old School RuneScape. Users can input a player's stats in the following ways: entering 
their stats into the level box, dragging the level sliders, using the lookup player button, or by randomly generate players through the randomizer gui. 
Moreover, users can select a player's weapon and their corresponding weapon style from two dropdown menus. Looking up players and selecting weapons draws 
data from the two APIs listed below. Users can also select whether they want their player to alternate weapon styles in the duel. This means that the player 
will select the weapon style that gives them the most damage per second when they are attacking and switch to the most defensive style when they are 
being attacked. 

Additionally, users can run a simulation of the duel and select to include a long-run analysis over a chosen number of duels. When the run 
simulation button is clicked, a simulation of a duel will be run. Health bars of each player update as the duel progresses. If the long-run analysis 
option is selected, the following data will also be displayed: max hit, damage per second, long-term win rate, and whether the simulation duel was won. 
The simulated duel will continue to loop and reset. Since this can cause other parts of the program to lag, it is recommended to stop the simulation of
the duel by pressing F5 before clicking buttons to setup and run a new simulation. Pressing F5 again will show this new simulation.

# Other uses
Using the application, users can easily investigate the balance of the game's combat system. Users can obtain concrete data about the strength of 
a player and determine which account builds are stronger than others. In this way, users can experiment with different account builds without having 
to train hundreds or thousands of hours.

# Built with
- openFrameworks: https://openframeworks.cc/
- ofxDatGui, a GUI addon for oF: https://github.com/braitsch/ofxDatGui
- ofxJSON, a JSON support addon for oF: https://github.com/jeffcrouse/ofxJSON
- Player data API: http://www.sudo.tf/api/hiscores/grab.php?rsn=
- Item API for weapons: https://www.osrsbox.com/projects/osrsbox-db/#accessing-the-json-api

# Pictures
- Start menu of the application
![Start menu of the application](https://i.imgur.com/osz1PCm.png)
- Random player generation
![Random player generation](https://i.imgur.com/7C6EdTV.png)
- Player lookup
![Player lookup](https://i.imgur.com/8RABhtB.png)
- Running a simulation
![Running a simulation](https://i.imgur.com/uxqNSjT.png)
- Running a simulation with an analysis
![Running a simulation with an analysis](https://i.imgur.com/UJhLirO.png)

# Author
- Tony Ruan

# Acknowledgements
Combat calculation formulas were collected and crosschecked from the sources listed below.
- Comprehensive combat formulas: http://services.runescape.com/m=forum/forums.ws?317,318,712,65587452,goto,1
- Video with accuracy formula: https://www.youtube.com/watch?v=Ucvn8hQEs2A
- OSRS wiki page with max hit formula: https://oldschool.runescape.wiki/w/Maximum_melee_hit
- Overview of combat formulas: https://www.osrsbox.com/blog/2019/01/22/calculating-melee-dps-in-osrs/#1-calculating-max-hit