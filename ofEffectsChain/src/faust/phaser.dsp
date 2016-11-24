import("stdfaust.lib");

Notches = 4;
width = 1000;
frqmin = 100;
fratio = 1.5;
frqmax = 800;
speed = hslider("speed",0.5,0,10,0.001) : si.smoo;
gain = hslider("gain",1,0,1,0.01) : si.smoo;
gate = button("gate");
depth = 1;
fb = 0;
invert = 0;

process = hgroup("phaser", pf.phaser2_stereo(Notches,width,frqmin,fratio,frqmax,speed,depth,fb,invert) * gain * gate);

