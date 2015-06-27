# Personal_Practice_GameOfLife
A win32 GUI of Conway's Game of Life, based on DirectX 9. Just a personal C++ and directx practice.
Currently it seems to have memory leaking problems but I am not sure where it happens and how to fix.

The rule of the game is based on Conway's Game of Life.

The size of panel is 65 * 65.
Click on one cell to activate or deactivate it. Any click will pause the game.
Can store the current state into a file and load it later.
Under borderless mode, the left border is treated as neighbor to the right border, and the top border is treated as neighbor to the bottom border.

PS. Originally I used Code::Blocks to write this project and recently I just installed VS2013. So there may be some changes.
