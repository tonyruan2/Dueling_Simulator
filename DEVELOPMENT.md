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
properly. Visually, it does--the dropdown option changes to "Unarmed"--but printing the selected dropdown after selecting unarmed 
displays the abyssal tentacle instead. I'm not sure why the code to select an option in the dropdown doesn't change what's returned by a
subsequent get() method on that option. However, if I manually reselect "Unarmed" (though "Unarmed is already selected"), then the player's 
data is randomly generated.

## List of major tasks
- [ ] Project structure/classes
- [ ] Test cases
- [ ] GUI design and layout
- [ ] Random stat generator button
- [ ] Damage/Accuracy calculations
- [ ] Combat simulation loop (take into account unarmed/armed, attack style changes, timing between attacks)
- [ ] Images/Videos for animating the combat (possibly use another library)
- [ ] Run numerous simulations after the first
- [ ] Analye the simulations
- [ ] Display the result of the simulations
