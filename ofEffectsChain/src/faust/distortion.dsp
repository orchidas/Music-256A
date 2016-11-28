import("stdfaust.lib");

d = hslider("drive",0,0,1,0.01) : si.smoo;
o = hslider("offset",0,-1,1,0.01) : si.smoo;
gain = hslider("gain",1,0,1,0.01) : si.smoo;
gate = button("gate");

distortion(drive,offset) = +(offset) : *(pregain) : clip : cubic : fi.dcblocker
with{
	pregain = pow(10,2*drive); // pregain range is 0 - 100
	clip = min(1) : max(-1); // clip the signal between 1 and -1
	cubic(x) = x - pow(x,3)/3;
};

process = hgroup("distortion",par(i,2,distortion(d,o)*gain*gate));

