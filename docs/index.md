input[id^="spoiler"]{
 display: none;
}
input[id^="spoiler"] + label {
  display: block;
  width: 200px;
  margin: 0 auto;
  padding: 5px 20px;
  background: #e1a;
  color: #fff;
  text-align: center;
  font-size: 24px;
  border-radius: 8px;
  cursor: pointer;
  transition: all .6s;
}
input[id^="spoiler"]:checked + label {
  color: #333;
  background: #ccc;
}
input[id^="spoiler"] ~ .spoiler {
  width: 90%;
  height: 0;
  overflow: hidden;
  opacity: 0;
  margin: 10px auto 0; 
  padding: 10px; 
  background: #eee;
  border: 1px solid #ccc;
  border-radius: 8px;
  transition: all .6s;
}
input[id^="spoiler"]:checked + label + .spoiler{
  height: auto;
  opacity: 1;
  padding: 10px;
}


<h1  align="center" id="hollow-faith-2d">Hollow Faith: 2D Plataform Game</h1>

<p>Welcome to the webpage of <a href="https://github.com/juanha2/HollowFaith">Hollow Faith by Aarón Guerrero & Juan Hernández</a></p>

<p align="justify">Hollow Faith is a 2d arcade platformer game developed by 2 students in CITM during the "development of videogames" subject. The first level gives you time to adapt to the mechanics, in the second level those skills will be put to test.</p>


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


<br><br>

<div class="spoiler">
  Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam eu sodales tortor, posuere mattis nunc. Integer eget sapien ullamcorper diam mollis laoreet. Praesent dignissim id urna at malesuada. Etiam id nisl vitae ante vestibulum volutpat. 
  <br/><br/>
  Etiam congue, neque a commodo iaculis, nisl massa varius felis, eget commodo eros mauris vitae felis. Pellentesque sed nisi bibendum, faucibus lectus id, varius metus. Vestibulum lacinia vehicula justo, ut molestie magna tincidunt nec. Suspendisse sit amet sagittis enim.
</div>

