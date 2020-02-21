# Game
An AI agent dodges missile to survive in a hostile environment
## Program Overview
* My AI agent keeps track of the unsafe zones using an array of size w (same size as the width of the game screen) using binary values (1 - Unsafe, 0 - Safe). At every time instant:
     * Any explosions taking place at the current time instant are marked unsafe by accessing the list of explosions struct using an instance of the game class.
     * Projectile motion is tracked using equations of motion:
     
        ![](eqn.png)
        
        With this information I predict when and where a projectile will fall ahead of time. Giving a buffer for the agent to move when a spot becomes unsafe prior itself, I mark the corresponding spots unsafe.
     * Depending on the safe spots and the current location of the agent, if it is at a safe spot, the agent remains there, otherwise, I check if the closest safest spot is in the left or right and move in that direction by player speed, owing to kinematic constraint.

## Debugging Challenges
* I made a mistake in calculating position of the projectile and hence was facing issues in moving the agent to the desired location.
* Another  mistake  I  made  was  not  realizing  what  pickSafeSpot  function  returns.I  assumed  it  can  return  float  and  hence  was  not  able  to  impose  the  kinematic constraint

This was developed as a part of a course assignment at CMU
