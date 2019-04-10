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

## List of major tasks
- [ ] Project structure/classes
- [ ] Test cases
- [ ] GUI design and layout
- [ ] Random stat generator button
- [ ] Damage/Accuracy calculations
- [ ] Combat simulation loop (take into account unarmed/armed, attack style changes, timing between attacks)
- [ ] Images/Videos for animating the combat (possibly use another library)
- [ ] Run simulations after the first
- [ ] Analye the simulations
- [ ] Display results of the simulations

## Reaches
- [ ] Weapons besides the primary one (named "Abyssal tentacle") used in duels
- [ ] ...