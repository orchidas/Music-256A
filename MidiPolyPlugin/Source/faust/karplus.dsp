//-----------------------------------------------
// 				karplus-strong
//-----------------------------------------------

import("stdfaust.lib");

freq = hslider("freq", 440, 20, 20000, 1); // Hz
gain = hslider("gain", 1, 0, 10, 0.01);  

// Excitator
//-----------

upfront(x) 	= (x-x') > 0.0;
decay(n,x)	= x - (x>0.0)/n;
release(n)	= + ~ decay(n);
trigger(n) 	= upfront : release(n) : >(0.0);

size 		= hslider("excitation [unit:f]", 128, 2, 512, 1);

// resonator
//------------

dur 		= hslider("duration [unit:f] [midi: ctrl 7]", 128, 2 ,512, 1);
att 		= hslider("attenuation", 0.03, 0, 1, 0.01);
average(x)	= (x+x')/2;
//P = ma.SR/freq;
resonator(d, a) = (+ : de.fdelay4(4096, d-1.5)) ~ (average : *(1.0-a)) ;

//string frequency is modulated by sine wave, added to LPF and distorted
modIndex = hslider("modIndex" ,3 ,1, 10 ,1);
modFreq = hslider("modFreq", 5, 0, 10, 1);
modulation = os.osc(modFreq) * modIndex;
cutoff =  hslider("cutoff", 2000, 500, 20000, 50) : si.smoo;
drive = hslider("drive", 0, 0, 1, 0.01);


process = no.noise * hslider("level", 0.5, 0, 1, 0.01)
		: hgroup("excitator", *(button("play"): trigger(size)))
: hgroup("resonator", resonator(dur+modulation, att)) : hgroup("lpf", fi.lowpass(2, cutoff)) : hgroup("distortion", ef.cubicnl(drive,0))
* hgroup("am", modIndex * os.osc(modFreq));

