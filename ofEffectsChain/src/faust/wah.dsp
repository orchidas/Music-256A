import("stdfaust.lib");

level = hslider("level", 0.5,0,1,0.01);
gain = hslider("gain",1,0,1,0.01) : si.smoo;
gate = button("gate");
//wah(level) =  : (ve.autowah(level));

process = hgroup("wah", ve.autowah(level)*gain*gate);

