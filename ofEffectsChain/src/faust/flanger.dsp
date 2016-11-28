import("stdfaust.lib");

flangerMaxDelay = hslider("maxDelay",10,0,20,0.01)*0.001*ma.SR : si.smoo; //control either this parameter or depth
flangerDepth = hslider("depth",0,0,1,0.01) : si.smoo;
flangerSpeed = hslider("speed",1,0.1,10,0.01) : si.smoo;
gain = hslider("gain",1,0,1,0.01) : si.smoo;
gate = button("gate");

flanger(maxDelay,depth,speed) = comb(delLength,depth)
with{
	delLength = maxDelay*(os.osc(speed)+1)/2;
	comb(delLength,b1) = _ <: (de.fdelay4(2048,delLength) : *(b1)),_ : + ;
};

process = hgroup("flanger",par(i,2,flanger(flangerMaxDelay,flangerDepth,flangerSpeed)*gain*gate));

