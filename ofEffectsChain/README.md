<h1>  </h1>

<h3>Guitar Effects chain + Physics Engine in OpenFrameworks</h3>

<p>The audio input represents the nucleus of the cell. The effects that can be dragged and dropped represent electrons around it. As of now this app does nothing, I am just building the GUI. </p>

<h2>Idea</h2>

<p>I play the guitar, I don't have a processor in California, or the money to buy one. I also want to learn the DSP behind these awesome audio effects, so the next best option was to build a virtual processor in C++. </p>

<p>But..this is not your ordinary effects plugin. The interface is inspired by the building blocks of matter itself, i.e., the <b>atom</b> (The classical model, not the fancy quantum mechanics stuff). Drag and drop effects onto a canvas and they start rotating around the nucleus (your audio input) in fixed orbits. The radius of the effect in its orbit determines its parameters.</p>

<p>More fun. Remember if you inject the right amount of energy into the electrons, they can jump orbits? With the right packet of energy input, an electron jumps from a lower energy orbit to a higher energy orbit. This is exactly how a laser operates. Our effects can do the same. Shoot them with a packet of energy, and they should change orbits. Since the effect parameters are related to orbital radius, they should change too! And you have a whole different sound from your effects chain.</p>

<h2>Implementation</h2>

<p>The interaction will be coded in OpenFrameworks. The plan is to make the audio effects in Faust, and integrate them with OpenFrameworks' audio callback function.<p>

<h3>That's all for now!</h3>