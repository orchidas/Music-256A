#ifndef GUN_H
#define GUN_H
#include <tuple>

//this class draws and controls movement of the laser gun
class Gun{
private:
    ofCylinderPrimitive barrel;
    float rad;
    float height;
    bool shoot;
    //angle of rotation of barrel
    float offset;
    float posX, posY;
    ofImage texture;
public:
    Gun(){
        offset = 0.0;
        rad = 10;
        height = 150;
    }

    ~Gun(){}

    void setup(const string &p_img, float posX, float posY){
        setPosition(posX, posY);
        setTexture(p_img);
        barrel.set(rad, height);
    }

    tuple<float, float> getPosition(){
        return make_tuple(posX+offset,posY + 100);
    }

    void setPosition(float x, float y){
        posX = x;
        posY = y;
    }

    void setOffset(float ang){
        offset = ang;
    }

    float getOffset(){
        return offset;
    }

    void setShootMode(bool b){
        shoot = b;
    }

    bool getShootMode(){
        return shoot;
    }

    float getRad(){
        return rad;
    }

    float getHeight(){
        return height;
    }

    void setTexture(const string &p_img){
        texture.load(p_img);
    }

    void update(){}

    void draw(){
        ofPushMatrix();
        ofTranslate(offset, 0, 0);
        texture.bind();
        barrel.setPosition(posX , posY, 0);
        barrel.draw();
        texture.unbind();
        ofPopMatrix();
    }


};

#endif // GUN_H
