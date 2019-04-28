# Sunday, April 7th, 2019
After I finished writing my proposal, I looked into what libraries would be beneficial to use. I found an openFrameworks addon 
for GUI design called ofxDatGui. I also found an addon called ofxJSON that supports loading JSON data.

In order to better understand the usages of openFrameworks, I skimmed through Lewis Lempton’s openFrameworks tutorial videos 
on YouTube. They were very useful and taught me how to load images and videos, draw shapes, and use classes in oF.

# Monday, April 8th, 2019
Today I explored ofxDatGui and ofxJSON. I successfully used ofxDatGui to create a basic interface where users can select or input
a player’s stats. I was also able to connect a text input field “Player name” with a button “Player lookup”. When the button is
clicked, the I use a button event that utilizes ofxJSON to make an API request to the player data API. I was able to print the data 
to the console. I still need to parse this data and update the GUI with the stats of the looked up player. I also need to handle
issues with bad player names or players that weren’t able to be found.

# Tuesday, April 9th, 2019
I added some code that allows me to parse player stat data from the API. Now, after I enter a username in the "Player name" text input
field, I can press the "Player lookup" button and have the sliders update to match the data retrieved from the API. This is the base
of linking the GUI created by ofxDatGui with ofxJSON's parsing capabilities.

# Sunday, April 14th, 2019
I added a gui class so that users can input the data of two players. Since I have one interface for each player in the duel, I reused as 
much code by passing in an argument for which player one of my methods (setupPlayer()) was supposed to generate a gui for. 

I also discovered how to set event handlers using ofxDatGui. This was important because I wanted to impose a limitation on the sliders. 
The hitpoints stat for each player cannot be less than 10, but I wanted the gui to show a hitpoints bar representing values 1 to 99. 
I wanted this to allow users the ability to visually compare a player's hitpoints stat bar with their other stats that range from 1 to 99. 
I prevented the user from setting a player's hitpoints below 10 by using a slider event to reset the hitpoints slider to 10 whenever it has 
a value below 10.

Another feature that I added to my GUI was generating stats of players randomly. I also added an option to generate the stats of each player 
so that the total stats were within a user-selected amount of one another.

Finally, the total stats value of each player is now displayed as a slider that is not directly modifiable. The slider value changes 
accordingly whenever users lookup players, generate random player data, or manually change the values of a player's stats. I chose to use 
a slider view so that users can gauge the strength of a player at a glance.

# Monday, April 15th, 2019
I added a duel runner gui component. Users can select whether they want to run a long-run analysis as well as the simulation. I also 
modified the gui layout and switched some buttons/sliders around to add clarity.

I ran into my first major bug while writing code for limitations on a player's stats. The abyssal tentacle requires a 
player to have an attack level of 75. I tried to make changing a player's weapon to abyssal tentacle cause their attack to be set to 
75 if it is below it. I also tried to enforce users not being able to change a player's attack below 75 if the abyssal tentacle 
is selected for that player.

The bug occurs when the "Abyssal Tentacle" option for a player is selected. When I click generate random players, the new player's attack
never falls below 75. During the generation of random players, I make sure to select the unarmed option for each player before generating 
new stats. This should, in theory, cause no limitation to be placed. However, it appears that the player's weapon does not get reset 
properly when I call the select() method. Visually, it does--the dropdown option changes to "Unarmed"--but printing the selected dropdown 
after selecting unarmed displays the abyssal tentacle instead. I'm not sure why the code to select an option in the dropdown doesn't 
change what's returned by asubsequent getSelected() method on that option. However, if I manually reselect "Unarmed" (though "Unarmed" is 
already selected), then the player's data is randomly generated.

UPDATE: After testing in a separate application, it appears getSelected() only takes into account what was manually selected in the 
dropdown menu by the user. This makes sense with the program appearing to fix itself when I manually select a weapon. 
getSelected() doesn't update when the program (through the code I have written) selects something. I'll have to keep track of what the
user and program selects, probably in a private variable. I'll also have to modify my code accordingly.

# Tuesday, April 16th, 2019
After trying for a few hours to find a work-around to the unexpected getSelected() functionality, I couldn't find a way to store what 
the user selects. There isn't a way for me to update what the player has selected in the weapon dropdown without a call to getSelected(). 
And, as I found out yesterday, getSelected() does not update when I force a selection of a new option through using the gui select() method. 
This means that I can only visually reset what the user sees in the dropdown menu, but the program thinks that the previous option is still 
selected.

I've decided to leave the so-called 'bug' as a feature. If the user is trying to generate random stats with an abyssal tentacle as their 
weapon, then it makes sense NOT to reset the attack option to "Unarmed" and instead force the 'randomly' generated player to have attack 
level of at least 75. Thinking about this more, I like this implementation. If a user wanted to simulate an unarmed fight, then generate 
random should do just that. If a user wants a fight to be between players that use abyssal tentacles that require an attack level, then 
the randomly generated players should have an attack of at least 75 to follow game rules.

# Wednesday, April 17th, 2019
I updated how the GUI looks by adding some color to buttons and making them highlight when hovered. Another change I made was that the 
input for a player's name resets to 12 characters (the max length of a username). Users can still input as many characters in the box 
because input isn't read until after the box is exited, but any inputs longer than 12 characters will be reduced to the first 12 characters.

# Saturday, April 20th, 2019
I modified the duel.h and duel.cpp files. Now, the duel has a public struct for a Player, but the duel class itself has private variables
to manage player data during the duel. Previously, the player's data (in addition to what was initially set in the GUI) was all publicly 
exposed. This has been corrected. The Player struct now only contains GUI information that the user can choose when they create the players.

# Sunday, April 21st, 2019
I implemented JSON support for weapons in my GUI. Now, weapon data (attack styles, requirements, etc) is fetched from a weapon api. Moreover, 
I also use data from the API to enforce level requirements in the GUI. If a user selects a weapon that a player does not have the attack level 
for, the player's attack level will change in the GUI. This enforcement was hard-coded before, but now it uses data taken directly from the API. 
This means I can add as many weapons as I want in the future.

I also tied together my gui class and duel class. My duel class now supports the creation of two players using data from the gui class. I also 
added functions to calculate the max hit of a player and functions to calculate the accuracy of one player against the other. The formulas for 
these calculations came from the resources listed in the development.md file. There is more than one source because I wanted to cross-check 
calculations to ensure accuracy.

To end the day, I implemented a basic long-run analysis of each player's win-rate over the course of a set amount of games.

# Monday, April 22nd, 2019
I added alternating attack style support to the duel simulator. If a user selects for one of the players to alternate their attack style, 
then during the duel, that player's attack style will change to give them the best performance. Different attack styles give different 
bonuses (i.e. an accurate style gives more accuracy, an aggressive style gives more strength, and a defensive style gives more defence). 
So, during the duel, if a player is alternating attack styles, the player will switch to the max damage per second style when they are 
attacking and the most defensive style when they are being attacked. Alternating attack styles causes players to gain marginal to 
tremendous advantages in duels.

# Tuesday, April 23rd, 2019
I looked online for images/gifs/videos I could use for my simulator. However, I couldn't find anything suitable, so I went into Old School 
Runescape and recorded videos to use. From these videos, I took screenshots and cropped and rotated images to get a set to use for the 
simulation. For each player, I have a player 'attack' image, a player 'defending' image, and an image for each of the player's starting 
position. 

I explored using recorded videos with players having different weapons, but I quickly realized that this approach was infeasible. If I 
wanted to add more weapons in the future, I'd have to record more videos. Moreover, using videos had many limitations, including 
asymmetric recordings (recordings for one player vs the other) and the sheer difficulty of recording precise videos for duels of
different weapon speeds.

My current plan is to cycle through the collected images to simulate the duel. I'll have to think about how to switch the images to match 
the attack interval of each player. While in-game, I also took a screenshot of the game's Duel Arena. Players go to this area to duel. I plan 
to use this image as the background picture for my project.

# Friday, April 26th, 2019
I tried to add animations to the duel simulator, but I ran into a problem: The entire oF program freezes (but runs the simulation in the background) 
when I click a button to run the animation. In other words, the code works, but the animation isn't displayed because oF doesn't run update() or draw() 
when a button is clicked. I'm trying to look for alternatives now.

# Saturday, April 27th, 2019
After about 5 hours, I managed to get a basic animation running. I did this by storing the 'simulation' in two vectors and then use draw() with numerous 
condition checks in my simulation class. The two vectors store the players' actions as integers, with a value of -1 representing no action and a non-negative 
value representing damage being dealt by that player. Each index in the vectors model the action of a game tick (600 milliseconds). I then used if/then statements 
to draw the appropriate pictures and display the damage dealt. Because draw() updates every frame, I also had to save the previous frame's data if I wasn't drawing 
anything new. If I didn't save the previous frame, there were stutters (no image displayed) in the animation. Finally, I used ofSleepMillis() to model the 
timings in a real Old School RuneScape duel.

More work on the simulation class today. My program now displays hit splats when the players attack each other. It was also laggy to use 
the duel simulator (i.e. for a new duel) while the simulation was running, so I made pressing F5 pause the simulation. When the simulation 
is paused, the user can use the simulator with less lag. They can then press F5 again to show the new duel simulation.

I've also implemented a hitpoints bar for each player that changes during the duel. The simulated duel replays continuously and this bar resets. 
Finally, I added a display of the results of the duel. For each player, this includes their max hit, damage per second, long-term win rate 
against the other player, and whether or not they won the duel that was visually simulated.

Everything seems to be working well!

## Things left to do in the final week 4/24 to 4/31
- Figure out looping through the combat images for the simulation
- Display the results of the simulation
- Test JSON parsing in gui.cpp and 
- Test JSON parsing parsing and calculations in duel.cpp
- (Extra) Regeneration mechanic 
(doesn't matter as much in the long run, but players regenerate 1 missing health every 100 game ticks, and they enter the duel 
with a random game tick of 0 to 99.)

## List of major tasks
- [ ] Project structure/classes
- [ ] Test cases (json?)
- [ ] GUI design and layout
- [ ] Random stat generator button
- [ ] Damage/Accuracy calculations
- [ ] Combat simulation loop (take into account unarmed/armed, attack style changes, timing between attacks)
- [ ] Images/Videos for animating the combat (possibly use another library)
- [ ] Run numerous simulations after the first
- [ ] Analyze the simulations
- [ ] Display the result of the simulations
