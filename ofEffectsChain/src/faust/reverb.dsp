import ("stdfaust.lib");

fsmax = 48000;
rdel = hslider("in_delay", 60,20,100,1);
f1 = 200;
f2 = 6000;
t60dc = 3;
t60m = 2;

//process = dm.zita_rev1;
process = hgroup("reverb", re.zita_rev1_stereo(rdel,f1,f2,t60dc,t60m,fsmax));
