import("stdfaust.lib");

d = hslider("Duration",0.5,0,1,0.01) : si.smoo;
f = hslider("Feedback",0,0,1,0.01) : si.smoo;

echo(duration,feedback) = +~(de.delay(ma.np2(44100),delayLength) : *(feedback))
with{
	delayLength = duration*ma.SR-1;
};

process = vgroup("delay",echo(d,f));

