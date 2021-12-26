# CS20_AllegroPoolGame

TODO
- [FINISHED] [PRIORITY HIGH] Make Vector2 parameter of Ball constructor a reference. (const Vector2& posVector)
- [FINISHED] [PRIORITY HIGH] Remove useless old file in the repo: GameEnvironment, resources, scheduler.
- [FINISHED] [ABOVE] For the above change, also see if the files are present in the Visual Studio work directory and remove it.
- [FINISHED] [PRIORITY HIGH] Make a class that handles all allegro resource. Idea: https://www.youtube.com/watch?v=WKs685H6uOQ https://github.com/JuUnland/Chess
- [PRIORITY HIGH] Randomized racks.
- [ABOVE] To randomize racks, set up an array of ball indexes (except for index 0 and 8 which are cue and eight ball). Loop through the rack positions and assign that position to the corresponding ball in the ball index.
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
