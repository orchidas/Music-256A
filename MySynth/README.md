# MySynth

<h3>Advanced Audio Synthesis in JUCE - Week 2 homework</h3>

<h5>Subtractive synthesizer </h5>

<p> A square and a sawtooth wave are added (their frequencies an octave apart) and fed through an ADSR envelope followed by a 5th order butterworth low pass filter and finally a reverb.</p>

<p>The cutoff frequency of the filter and the beat frequency between the two waves is controlled by the user to generate interesting sounds. The keyboard (that introduces latency, unfortunately) is used to play different musical notes. </p>

<p>The signal processing code was written in FAUST and the interface was created with JUCE. </p>

<p> Demo can be viewed in this <a href = "https://www.youtube.com/watch?v=iJrxDy0zurY&feature=youtu.be">video</a>. </p>

