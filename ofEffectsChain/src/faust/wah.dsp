import("stdfaust.lib");

level = hslider("level", 0.5,0,1,0.01);
//wah(level) =  : (ve.autowah(level));

process = hgroup("wah", ve.autowah(level));

