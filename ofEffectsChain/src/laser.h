#ifndef LASER_H
#define LASER_H

#include <tuple>

//class represents each laser packet
class Laser{
private:
    ofCylinderPrimitive laser;
    ofImage texture;
    float posX;
    float posY;
    bool shoot;

public:

   Laser(){}

    ~Laser(){}

   void setup(const string &p_img, float rad, float height){
       setTexture(p_img);
       laser.set(rad, height);
       setShoot(true);
   }


   void setShoot(bool b){
       shoot = b;
   }

   bool getShoot(){
       return shoot;
   }

    void setTexture(const string &p_img){
        texture.load(p_img);
    }

    void setPosition(tuple<float,float> pos){
        posX = get<0>(pos);
        posY = get<1>(pos);
    }

    tuple <float, float> getPosition(){
        return make_tuple(posX, posY);
    }

    void update(){
        posY += 10;
    }

    void draw(){
        ofPushMatrix();
            texture.bind();
            laser.setPosition(posX, posY, 0);
            laser.draw();
            texture.unbind();
        ofPopMatrix();


    }
};

#endif // LASER_H
