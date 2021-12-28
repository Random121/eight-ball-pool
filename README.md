# CS20_AllegroPoolGame

Cool school project nice.

TODO
- [FINISHED] [PRIORITY HIGH] Make Vector2 parameter of Ball constructor a reference. (const Vector2& posVector)
- [FINISHED] [PRIORITY HIGH] Remove useless old file in the repo: GameEnvironment, resources, scheduler. For the above change, also see if the files are present in the Visual Studio work directory and remove it.
- [FINISHED] [PRIORITY HIGH] Make a class that handles all allegro resource. Idea: https://www.youtube.com/watch?v=WKs685H6uOQ https://github.com/JuUnland/Chess
- [PRIORITY MEDIUM] Randomized racks. To randomize racks, set up an array of ball indexes (except for index 0 and 8 which are cue and eight ball). Loop through the rack positions and assign that position to the corresponding ball in the ball index.
__Sample Code For Racking:__
```
vector<int> ballIndexes{ 1, 2, 3, 4, 5, 6, 7, 9, 10 };
vector<vector<int>> rackPosition{
  {0,0}
  {1,1}
  {2,2}
  {3,3}
  ....
  {11, 11}
};
  
shuffle_array(ballIndexes);
  
// array is now {2, 4, 1, 7, 9, 5, 10, 3, 6 }

int ballIndex{};
for (int i{ 1 }; i < rackPosition.size(); ++i)
{
    if (i == 8)
       i++;
    gameBalls[ballIndexes[ballIndex]].setPosition(rackPosition[i]);
    ballIndex++;
}

gameBalls[8].setPosition(rackPositions[8]);
gameBalls[0].setPosition(rackPositions[0]);

```
- [PRIORITY HIGH] Make Players constructor take only player count, and implement a setter for the player index. This allows us to randomize the index in the constructor of GameLogic, also enabling us to print the name of the player who breaks.
- [PRIORITY HIGH] Implementing player names rather than just 1 and 2. To implement default names, change the playerName1 and playerName2 variables in main.cpp to have "1" and "2" rather than just empty.
- [PRIORITY LOW] Clean up old code that is no longer in use.
