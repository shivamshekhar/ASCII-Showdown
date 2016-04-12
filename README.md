# ASCII Showdown

#### Description:
This is a simple shooting game made in C.

#### Controls:
* A: move left
* D: move right
* W: fires a bullet
* O: fire homing boomerang
* P: Place a wall
* I: Engage weapon boost(fires 3 bullets at once. Lasts for 5 seconds after which a recharge of 10 seconds is required)

#### Features:
* flashes the screen whenever the bullets hit either player
* collision of bullets:
  1. bullets destroy each other on collision
  2. homing boomerang destroy each other on collision
  3. homing boomerang destroys bullet on collision
  4. A wall can sustain 4 bullet impact and 1 homing boomerang 
     impact. 
* Limited Ammo for user but unlimited for the CPU
* User has 10 hit points whereas CPU has 40
* When CPU health falls below 10, it can also shoot triple bullets

#### Technology:
This game was made using windows.h library on Code::Blocks IDE having GNU GCC compiler

#### Written by:
Shivam Shekhar  
Email: shivam.shekhar.ece14@itbhu.ac.in
 