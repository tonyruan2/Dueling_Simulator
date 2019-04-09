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
