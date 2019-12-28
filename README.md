# HollowFaith
Hollow Faith is a 2d arcade platformer game developed by 2 students in CITM during the "development of videogames" subject. The first level gives you time to adapt to the mechanics, in the second level those skills will be put to test.

*All feedback has been applied at the project. Thanks for it.


## Find our project Web Page in: 
https://juanha2.github.io/HollowFaith/

## Find this project's repository in:
https://github.com/juanha2/HollowFaith

## How to play:

### General
- ESC: Open Menu in Scene, or Close game in Main Menu.
### Keyboard:
- A: Move left
- D: Move right
- W: Climb up
- S: Climb down

- Space: Jump (jump on enemy to eliminate him in only one action (It is dangerous))
- Hold Space while in air: hover

- K: Throw a rock to hit the enemies. (You need two hits to eliminate them)

### Console:
- º: Open/Close Console 
#### Commands:
- list: display the list of commands
- god_mode: toggles god mode on/off
- quit: exits the game
- FPS<number>: change FPS cap with some safe limits (30-120)
- map<map_name>: load another map

### Debug functionality:
- F1 ​Start from level 1
- F2 ​Start from level 2
- F3 ​Start from current level
- F5 ​Save the current state
- F6 ​Load the previous state
- F8 Debug UI
- F9 ​See paths and colliders
- F10 ​God Mode
- F11 ​Cap/Uncap Frames


## Credits
- Aarón Guerrero Cruz
> Github profile: https://github.com/AaronGCProg

- Juan Hernández Almagro
> Github profile: https://github.com/juanha2

- Art from:

https://craftpix.net/freebies/free-pixel-art-tiny-hero-sprites/?utm_source=dribbble.com&utm_medium=myself&utm_campaign=sprite

https://anokolisa.itch.io/forest-game-assets-20

https://vnitti.itch.io/glacial-mountains-parallax-background

- Music from:

https://www.bensound.com/royalty-free-music/track/sad-day

https://www.bensound.com/royalty-free-music/track/november

- SFX from:

https://freesound.org/people/shinephoenixstormcrow/sounds/337049/

https://freesound.org/people/elijahdanie/sounds/487436/


## Innovations:

### 1rst Pack Innovations (NP)
- Animation loading from XML
- Particle effects system
- Procedural collision system
- New mechanics: Climb and hover!
- Parallax made from Tiled
- Music path from Tiled Map editor
- Only blit current Camera Tiles

### 2nd Pack Innovations (100 %)
- Enemies spawns loading from TMX
- Extra feature: Throw a rock with parabolic path
- Skills with cooldown (Rock)
- Checkpoints autosave
- Spatial Audio
- Land Enemies know when they need jump with Pathfinding

### 3r Pack Innovations
- Saved High Score & Best Time


## Member contributions

Juan Hernández Almagro
- Creation of the maps
- Implemented colliders from Tiled
- Implemented animations 
- Implemented transitions between maps 
- Win/Lose Condition
- Added music and sfx
- Camera limits
- Blit only current map tiles
- Climb mechanic
- Creation of entity system
- Checkpoints logic
- UML Entities
- Implementation of Score & Life systems
- Implementation base of main menu & game menu
- UI Image, Inputbox & Label


Aarón Guerrero Cruz
- Creation of player time/movement logic system
- Implemented collision logic system
- Implemented animation logic system
- Implemented particles logic system
- Implemented camara movement logic system
- Added XML Animation method
- Added particles effects in-game
- Added debug functions
- Hover mechanic
- Pathfinding Logic and Sub-systems
- Delta Time (Cap frames and balancing it)
- Enemies behavior
- Brofiler Implementation at the Code
- GUI Manager
- GUI Elements system
- UI Button & Scrollbar
- Gameplay Trailer
- Web Page


## License:
MIT License

Copyright (c) [2019] [Aarón Guerrero Cruz / Juan Hernández Almagro]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


