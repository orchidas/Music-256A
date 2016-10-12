//Subtractive synthesis - sawtooth + square wave is passed through ADSR envelope and a
//butterworth lowpass filter

import("stdfaust.lib");

freqsaw = hslider("frequency", 440, 50, 2000, 0.1) : si.smoo; //create frequency slider and smooth it
freqsqr = hslider("frequency", 218, 50, 2000, 0.1) : si.smoo; 
cutoff = hslider("cutoff", 2000, 500, 20000, 50) : si.smoo;
gain = hslider("gain" , 0.5,0,0.5,0.01) : si.smoo;
gate = button("gate");

saw = hgroup("saw", os.sawtooth(freqsaw) * gain);
sqr = hgroup("sqr", os.square(freqsqr) * gain);

process = hgroup("sub_synth", (saw + sqr) * env : lpf)
with{	
	env = hgroup("env",en.adsr(0.01,0.3,50,0.2, gate));
	lpf = hgroup("lpf", fi.lowpass(5,cutoff));	
};
