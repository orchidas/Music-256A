import("stdfaust.lib");

d = hslider("duration",0.5,0,1,0.01) : si.smoo;
f = hslider("feedback",0,0,1,0.01) : si.smoo;
gain = hslider("gain",1,0,1,0.01) : si.smoo;
gate = button("gate");

echo(duration,feedback) = +~(de.delay(ma.np2(44100),delayLength) : *(feedback))
with{
	delayLength = duration*ma.SR-1;
};

process = vgroup("delay",par(i,2,echo(d,f)*gain*gate));

