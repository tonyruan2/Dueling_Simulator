# Proposal
I want to create a dueling simulator for a MMORPG called Old School Runescape (OSRS). 
An overview of the game can be found at this link: https://en.wikipedia.org/wiki/Old_School_RuneScape.

In broad terms, 'dueling' refers to one-on-one combat between two players. However, dueling is more often 
associated with legal gambling in OSRS. More specifically, most duels in OSRS consist of players staking 
money against one another and participating in a rule-enforced fight to determine the winner. Of the numerous 
rules, the most important ones are melee combat only (no ranged or magic), no stat boosting potions, no armor, 
no movement, and either no weapon or one weapon.

The aforementioned rules are what I will follow when implementing my dueling simulator. However, I will not 
implement the gambling part of the duel because most gamblers will only duel players of similar stats and items. 
This results in most duels being a coin-flip.

However, I plan to implement an analysis feature on the simulated duels. First, some background explanation 
is needed. In OSRS, rule-enforced duels are turn-based and the damage that a player deals to another player 
is primarily determined by two factors: the combat stats of each player (Hitpoints, Attack, Strength, and 
Defense) and an element of randomness.

I'll allow users to input each competing player's combat stats. With this data, I'll then calculate each 
player's maximum hit and accuracy against the other player. Afterwards, I’ll simulate the duel by using the 
calculations and randomly rolling numbers during each player’s turn. These numbers determine whether a player
lands a hit, and if they do land a hit, how much damage they deal. The duel ends when one player’s health is 
brought to zero.

At the end of the duel, I want to also simulate a few thousand more duels of the same type to collect data 
and display an analysis of how each player fares against the other in the long run. This will allow users of 
the simulator to gain knowledge about which combat stat combinations are superior to others in duels. This is 
especially interesting when players have the same total value of combat stats but have a different combination
of them.

# Outside Resources
- Comprehensive combat formulas: http://services.runescape.com/m=forum/forums.ws?317,318,712,65587452,goto,1
- Video with accuracy formula: https://www.youtube.com/watch?v=Ucvn8hQEs2A
- OSRS wiki page with max hit formula: https://oldschool.runescape.wiki/w/Maximum_melee_hit
- Overview of combat formulas: https://www.osrsbox.com/blog/2019/01/22/calculating-melee-dps-in-osrs/#1-calculating-max-hit
- Player data API: http://www.sudo.tf/api/hiscores/grab.php?rsn=
- Item API for weapons: https://www.osrsbox.com/projects/osrsbox-db/#accessing-the-json-api
- openFrameworks tutorial videos by Lewis Lempton: https://www.youtube.com/watch?v=IKSTo_0pB28&list=PL4neAtv21WOmrV8z9rSzL20QpdLU1zJLr&index=1
- Credit Jagex, the company which created Old School Runescape, for any images/assets of their game used

# Libraries
- To design my user interface, I’ll use ofxDatGui: https://github.com/braitsch/ofxDatGui
- To load API data, I’ll use ofxJSON: https://github.com/jeffcrouse/ofxJSON
