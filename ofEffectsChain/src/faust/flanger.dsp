import("stdfaust.lib");

flangerMaxDelay = hslider("delay",10,0,20,0.01)*0.001*ma.SR : si.smoo; //control either this parameter or depth
flangerDepth = hslider("depth",0,0,1,0.01) : si.smoo;
flangerSpeed = hslider("speed",1,0.1,10,0.01) : si.smoo;

flanger(maxDelay,depth,speed) = comb(delLength,depth)
with{
	delLength = maxDelay*(os.osc(speed)+1)/2;
	comb(delLength,b1) = _ <: (de.fdelay4(2048,delLength) : *(b1)),_ : + ;
};

process = hgroup("[1]Flanger",flanger(flangerMaxDelay,flangerDepth,flangerSpeed));

