<h1>Effectrons</h1>

<h3>Guitar Effects chain + Physics Engine in OpenFrameworks</h3>

<p>The audio input represents the nucleus of the cell. The effects that can be dragged and dropped represent electrons around it. The radius of the effect from the input determines its parameters. Thanks to Mark for the name! </p>

<p align="center">
  <img src="app screenshots/laser.png" width = "600" height = "350"/>
</p>

<h2>Idea</h2>

<p>I play the guitar, I don't have a processor in California, or the money to buy one. I also want to learn the DSP behind these awesome audio effects, so the next best option was to build a virtual processor in C++. </p>

<p>But..this is not your ordinary effects plugin. The interface is inspired by the building blocks of matter itself, i.e., the <b>atom</b> (The classical model, not the fancy quantum mechanics stuff). Drag and drop effects onto a canvas and they start rotating around the nucleus (your audio input) in fixed orbits. The radius of the effect in its orbit determines its parameters.</p>

<p>More fun. Remember if you inject the right amount of energy into the electrons, they can jump orbits? With the right packet of energy input, an electron jumps from a lower energy orbit to a higher energy orbit. This is exactly how a laser operates. Our effects can do the same. Shoot them with a packet of energy, and they should change orbits. Since the effect parameters are related to orbital radius, they should change too! And you have a whole different sound from your effects chain.</p>

<h2>Implementation</h2>

<p>The GUI is done with OpenFrameworks. The audio effects are in Faust, and are integrated with OpenFrameworks' audio callback function. An external HID device like the GameTrak can be used with the application, and a little ChucK script sends OSC messages to OpenFrameworks to read input from the GameTrak.<p>

<h2>Features</h2>
<ul>
<li>Drag and drop guitar effects on the screen and move them around to create unique sounds!</li>
<li>Use a laser gun to shoot effects. Impart them with a packet of energy and watch them jump to different radii,
altering the sound.</li>
<li>Use a GameTrak (or keyboard) to move the gun around and shoot lasers. Ask a friend or do it yourself while playing your guitar - go crazy with the performance!</li>
<li>Record a backing guitar track and loop it to jam with yourself. </li>
</ul>

<h2>Setup</h2>
<h3>Software Dependencies</h3>
<ul><li>OpenFrameworks Version 0.9</li><li>Addons - <b>OfxStk, OfxOsc</b></li><li>ChucK (optional - only for GameTrak)</li><li>JackD and QJackctl (for linux only - optional but recommended)</ul>

<h3>Hardware (optional but recommended)</h3>
<ul><li>Audio Interface <p align = "center"><img src = "app screenshots/ua101.jpg"/></p></li>
<li>Electric Guitar <p align = "center"><img src = "app screenshots/guitar.jpg"/></p></li>
<li>GameTrak Controller <p align = "center"><img src = "app screenshots/gametrak.jpg"/></p></li>
</ul>

<h2>Installation</h2>
<ul><li>Clone this repository</li> <li>For gametrak, go to ofEffectsChain/src/chuck and run the command 'chuck GametrakTetherToOSC.ck' </li>  <li>On Linux, go to the directory where project is cloned (which should have a Makefile) and type 'make'</li></ul>

<h3>That's all for now!</h3>
