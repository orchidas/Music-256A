import ("stdfaust.lib");


//need to fix this to add gate button

fsmax = 48000;
rdel = hslider("in_delay", 60,20,100,0.1);
f1 = 200;
f2 = 6000;
t60dc = 3;
t60m = 2;
gate = button("gate");

process = hgroup("reverb", re.zita_rev1_stereo(rdel,f1,f2,t60dc,t60m,fsmax));

