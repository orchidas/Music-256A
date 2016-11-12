#ifndef PLANET_H
#define PLANET_H

#endif // PLANET_H

#include<string.h>
#include<math.h>

//create class Planet
class Planet{
public:
    Planet(){}
    ~Planet(){}

    void setup(const string &n,const string &p_img, float rad, float semiMajor, float ecc){
        setSize(rad);
        setRotationAngle(0.0);
        setOrbitAngle(0.0);
        setSemiMajorAxis(semiMajor);
        setEccentricity(ecc);
        setSemiMinorAxis();
        setName(n);
        setTexture(p_img);
        if(n.compare("Saturn") == 0)
            ring = true;
        else
            ring = false;
        blackHole = false;
        isInside = false;
        xPos = 0.0;
        yPos = 0.0;

    }

    void setName(const string &n){
        name = n;
    }

    const string & getName(){
        return name;
    }


    void isBlackHole(bool res){
        blackHole = res;
    }

    void setIsInside(bool in){
        isInside = in;
    }

    bool getIsInside(){
        return isInside;
    }

    void setEccentricity(float val){
        ecc = val;
    }

    void setSemiMajorAxis(float val){
        a = val;
    }

    void setSemiMinorAxis(){
        b = a * sqrt(1 - pow(ecc,2.0));
    }

    float getSemiMajorAxis(){
        return a;
    }

    float getSemiMinorAxis(){
        return b;
    }


    void setXpos(){
       xPos = a*cos(orbitAngle * PI/180);
    }

    void setYpos(){
        yPos =  b*sin(orbitAngle * PI/180);
    }

    float getXPos(){
        return xPos;
    }

    float getYPos(){
        return yPos;
    }

    void setSize(float rad){
        planet.setRadius(rad);
    }

    float getSize(){
        //if planet is saturn, ring radius must be taken into account
        if (name.compare("Saturn") == 0)
            return planet.getRadius() + 10.0;

        else
            return planet.getRadius();
    }

    void setRotationAngle(float ang){
        rotAngle += ang;
    }

    float wrapAngle( float angle )
    {
        while(angle > 2*PI){
            angle -= 2*PI;
        }
        return angle;
    }

    void setOrbitAngle(float orb){
        orbitAngle += orb;
    }

    void setTexture(const string & p_imgPath)
    {
        texture.loadImage(p_imgPath);
    }

    float distanceFromCenter(float xc, float yc){
        return sqrt(pow((xc-xPos),2) + pow((yc-yPos),2));

    }

    float getRotationAngle(){return rotAngle;}
    float getOrbitAngle(){return orbitAngle;}

    void update(float ang){
        setOrbitAngle(ang);
        //slowly decrease orbit
        if(blackHole){
            setSemiMajorAxis(a-0.1);
            setSemiMinorAxis();
        }
        setXpos();
        setYpos();
    }

    void draw(){

        ofPushMatrix(); // Save center state
        ofTranslate(xPos,yPos,0);

        ofPushMatrix();
            //rotate about their own axes
            ofRotateZ(rotAngle);
            ofPushStyle();
                texture.bind();
                planet.draw();
                texture.unbind();
            ofPopStyle();
        ofPopMatrix();

        //draw orbit
        ofPushMatrix();
        ofRotateZ(90);
            ofPushStyle();
                ofNoFill();
                ofSetCircleResolution(100);
                ofSetColor(222,222,222);
                ofDrawEllipse(0, 0, 0, 2*b, 2*a);

            ofPopStyle();
        ofPopMatrix();

        if (ring == true)
            drawRing();
    }


    void drawRing(){

        unsigned ringsNumber = 50;
            float multRad = 1.2;

            for (unsigned ring = 0; ring < ringsNumber; ring++)
            {
                ofMesh mesh;
                float cx = 0, cy = 0, zx = 0;
                int max = 50;
                float r = planet.getRadius() * multRad;
                //float r = (objects[SATURN]->radius()*multRad);

                ofPushMatrix();
                multRad += 0.005;
                mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
                for(int ii = 0; ii < max; ii++)
                {
                    float theta = ofMap(ii, 0, max-1, 0, 2*PI); //map i as circle divisions to actual radian values around the circle (note we don't go quite all the way around by one step, because it will be the same as where we started, so we'll just index that starting vertex when we make faces)
                    float x = r * cosf(theta);//calculate the x component
                    float y = r * sinf(theta);//calculate the y component
                    ofVec3f p(x + cx, y + cy, zx);
                    mesh.addColor(ofColor(247-(ring*2), 235-(ring*2), 168-(ring*2)));
                    mesh.addVertex(p);//output vertex
                    mesh.addTexCoord(ofVec2f((cosf(theta)+1.0)*0.5, (sinf(theta)+1.0)*0.5));
                }
                ofTranslate(xPos, yPos, 0);
                ofRotateZ(90);
                mesh.draw();
                ofPopMatrix();
        }
    }


private:
    float rotAngle;
    //orbit angle goes from 0 to 2*PI
    float orbitAngle;
    //a and b are semi major and semi-minor axes of ellipse
    float a, b, ecc;
    float xPos, yPos;
    ofSpherePrimitive planet;
    ofImage texture;
    string name;
    bool ring;
    bool blackHole;
    bool isInside;


};
