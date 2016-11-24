
import("stdfaust.lib");

g = hslider("gain",1,0,1,0.01) : si.smoo;

tremolo = *(1-depth*(os.osc(freq)*0.5+0.5))
with{
	freq = hslider("frequency",5,0.1,15,0.01) : si.smoo;
	depth = hslider("depth",0,0,1,0.01) : si.smoo;
};

process = hgroup("tremolo", tremolo*g);
