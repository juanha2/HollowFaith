

<h1  align="center" id="hollow-faith-2d">Hollow Faith: 2D Plataform Game</h1>

<p>Welcome to the webpage of <a href="https://github.com/juanha2/HollowFaith">Hollow Faith by Aarón Guerrero & Juan Hernández</a></p>

<p align="justify">Hollow Faith is a 2d arcade platformer game developed by 2 students in CITM during the "development of videogames" subject. The first level gives you time to adapt to the mechanics, in the second level those skills will be put to test.</p>

<br>
<details> 
  <summary> GAME LICENSE </summary>
<br><br>
MIT License
<br><br>
Copyright (c) [2019] [Aarón Guerrero Cruz / Juan Hernández Almagro]
<br><br>
<p align="justify">Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:</p>
<br>
<p align="justify">The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.</p>
<br>
<p align="justify">THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.</p>
</details>


<br><br>

<h2  align="center" id="hollow-faith-trailer">Hollow Faith: Trailer</h2>

<iframe width="510" height="412"
src="https://www.youtube.com/embed/7baUOVYobgs" frameborder="0" allowfullscreen>
</iframe>
<p>You can also enjoy the trailer by  <a href="https://drive.google.com/open?id=1WSQBV5l0FuWW0jMDmDEWp5pAttX_yvbt">clicking here.</a></p>

<br><br>

<center>

<font size="6"><a href="https://www.youtube.com/watch?v=2Z4m4lnjxkY">Download our game Build</a></font>

</center>

<br><br>
<h2  align="center" id="main-core-subsystems">Main Core Subsystems</h2>

Entities Manager and Entities Elements - GUI Manager and GUI Elements

<details> 
  <summary> Extra Features: Innovation </summary>
- Animation loading from XML<br>
- Particle effects system<br>
- Procedural collision system<br>
- New mechanics: Climb and hover!<br>
- Parallax made from Tiled<br>
- Music path from Tiled Map editor<br>
- Only blit current Camera Tiles<br>
- Enemies spawns loading from TMX<br>
- Extra feature: Throw a rock with parabolic path<br>
- Skills with cooldown (Rock)<br>
- Checkpoints autosave<br>
- Spatial Audio<br>
- Land Enemies know when they need jump with Pathfinding<br>
- High Score & Best Time<br>

</details>

<br><br>

<center>
  
<h2  align="center" id="team-members">Team Members</h2>

<table style="width:100%">
  <tr>
    <th>Member</th>
    <th>Role</th> 
    <th>Portrait</th>
  </tr>
  <tr>
    <td> <a href="https://github.com/AaronGCProg">Aarón Guerrero</a></td>
    <td>Code</td> 
    <td><img src="https://raw.githubusercontent.com/juanha2/HollowFaith/master/docs/aaronPortrait.png" alt="Aaron" class="inline"></td>
  </tr>
  <tr>
    <td><a href="https://github.com/juanha2">Juan Hernández</a></td>
    <td>Code</td> 
    <td><img src="https://raw.githubusercontent.com/juanha2/HollowFaith/master/docs/juanPortrait.png" alt="Juan" class="inline"></td>
  </tr>
</table>
</center>

<br>
<details> 
  <summary> Aarón Guerrero - Individual Work </summary>
- Creation of player time/movement logic system<br>
- Implemented collision logic system<br>
- Implemented animation logic system<br>
- Implemented particles logic system<br>
- Implemented camara movement logic system<br>
- Added XML Animation method<br>
- Added particles effects in-game<br>
- Added debug functions<br>
- Hover mechanic<br>
- Pathfinding Logic and Sub-systems<br>
- Delta Time (Cap frames and balancing it)<br>
- Enemies behavior<br>
- Brofiler Implementation at the Code<br>
- GUI Manager<br>
- GUI Elements system<br>
- UI Button & Scrollbar<br>
- Gameplay Trailer<br>
- Web Page<br>
- UML GUI
</details>

<br>
<details> 
  <summary> Juan Hernández - Individual Work </summary>
- Creation of the maps<br>
- Implemented colliders from Tiled<br>
- Implemented animations <br>
- Implemented transitions between maps <br>
- Win/Lose Condition<br>
- Added music and sfx<br>
- Camera limits<br>
- Blit only current map tiles<br>
- Climb mechanic<br>
- Creation of entity system<br>
- Checkpoints logic<br>
- UML Entities<br>
- Implementation of Score & Life systems<br>
- Implementation base of main menu & game menu<br>
- UI Image, Inputbox & Label<br>
 - Console
</details>

<br><br><br>

<h1  align="center" id="how-to-play">How to Play it?</h1>
<br>

<h2  align="center" id="game-inputs">Game Inputs</h2>
<table style="width:100%">
  <tr>
    <th>Input</th>
    <th>Action</th> 
  </tr>
    <tr>
    <td> ESC </td>
    <td> Open Menu at Game Scene or Close game at Main Menu </td> 
  </tr>
  <tr>
    <td> WASD </td>
    <td> Classical Movement </td> 
  </tr>
  <tr>
    <td> Space </td>
    <td> Jump </td> 
  </tr>
   <tr>
    <td> HOLD Space </td>
    <td> Hover </td> 
  </tr>
   <tr>
    <td> K </td>
    <td> Throw a Rock </td> 
  </tr>
   <tr>
    <td> º </td>
    <td> Open/Close Console </td> 
  </tr>
</table>

<br><br>
<h2  align="center" id="game-inputs">Console Commands</h2>
<table style="width:100%">
  <tr>
    <th>Command</th>
    <th>Action</th> 
  </tr>
  <tr>
    <td> list </td>
    <td> Display the list of commands </td> 
  </tr>
  <tr>
   <td> god_mode </td>
    <td> Toggles god mode on/off </td> 
  </tr>
   <tr>
    <td> quit </td>
    <td> Exits the game </td> 
  </tr>
   <tr>
    <td> FPS&lt;number&gt; </td>
    <td> Change FPS cap with some safe limits (30-120) </td> 
  </tr>
   <tr>
    <td> map&lt;map_namer&gt; </td>
    <td> Load another map </td> 
  </tr>
</table>

<br><br>
<h2  align="center" id="game-inputs">Debug Functionality</h2>
<table style="width:100%">
  <tr>
    <th>Key</th>
    <th>Functionality</th> 
  </tr>
  <tr>
    <td> F1 - F2 </td>
    <td> Start from level 1 - 2 </td> 
  </tr>
  <tr>
   <td> F3 </td>
    <td> Start from current level </td> 
  </tr>
   <tr>
    <td> F5 </td>
    <td> Save the current state </td> 
  </tr>
   <tr>
    <td> F6 </td>
    <td> Load the previous state </td> 
  </tr>
   <tr>
    <td> F8 </td>
    <td> Debug UI </td> 
  </tr>
     <tr>
    <td> F9 </td>
    <td> Debug pathfinding and colliders </td> 
  </tr>
     <tr>
    <td> F10 </td>
    <td> God Mode </td> 
  </tr>
   <tr>
    <td> F11 </td>
    <td> Cap/Uncap Frames </td> 
  </tr>
</table>
