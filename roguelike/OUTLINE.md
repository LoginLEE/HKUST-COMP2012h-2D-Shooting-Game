# TBD

As group H16, we made a game for COMP2012H Group Project. We still haven't put a name so its name is TBD. Here are some information about the most important classes and the organization of the program.

Run in command prompt to try the game:

        compile.bat *.cpp &* run.bat

## GameEntity

This is the base class for most objects. It's in a recursive structure so it's possible to create trees of GameEntity nodes. 

In a game, order of updates and draws of certain objects must be ensured. For this reason GameEntity includes priority values for update and draw functions. While updating, a GameEntity first updates itself, and the its children in order of their priorities.

GameEntities also have a pointer to the related GameManager object to access data related to the context they are in.

They also have simple transformal properties such as position, size and velocity. We did not include rotation because it's use is limited. With a similar reason, only position value represents a value relative to the parent node (GameEntity).

## CollidingEntity

CollidingEntity is a derived class of GameEntity and it has additional features mostly related to physics simulation including collision (as the name suggests). Binary search algorithm is used for calculating the final position of a CollidingEntity after an update tick.

## GameManager

GameManager is the object that runs the toplevel game logic and connects everything together. GameEntities can receive information about the world with the help of the GameManager.

## Maps

Maps are loaded from a file and interpreted according to the descriptions in MAP_DEFINITIONS.txt .