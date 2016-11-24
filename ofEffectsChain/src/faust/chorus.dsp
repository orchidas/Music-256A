
import("stdfaust.lib");

level	= hslider("level", 0.5, 0, 1, 0.01);
//control this parameter freq
freq	= hslider("freq", 2, 0, 10, 0.01);
dtime	= hslider("delay", 0.025, 0, 0.2, 0.001);
depth	= hslider("depth", 0.02, 0, 1, 0.001);
gain = hslider("gain",1,0,1,0.01) : si.smoo;
gate = button("gate");

tblosc(n,f,freq,mod)	= (1-d)*rdtable(n,wave,i&(n-1)) +
			  d*rdtable(n,wave,(i+1)&(n-1))
with {
	wave	 	= ba.time*(2.0*ma.PI)/n : f;
	decimal		= _ <: - ( floor );
	phase		= freq/ma.SR : (+ : decimal) ~ _;
	modphase	= decimal(phase+mod/(2*ma.PI))*n;
	i		= int(floor(modphase));
	d		= decimal(modphase);
};

chorus(d,freq,depth)	= de.fdelay(1<<16, t)
with {	t		= ma.SR*d/2*(1+depth*tblosc(1<<16, sin, freq, 0)); };

process			= hgroup("chorus", (c, c) * gain * gate)
with { c(x) = x+level*chorus(dtime,freq,depth,x); };
