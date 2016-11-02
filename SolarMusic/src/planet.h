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
        setSemiMinorAxis(ecc);
        setName(n);
        setTexture(p_img);
        if(n.compare("Saturn") == 0)
            ring = true;
        else
            ring = false;
        xPos = 0.0;
        yPos = 0.0;
    }

    void setName(const string &n){
        name = n;
    }

    const string & getName(){
        return name;
    }

    void update(float ang){
        setOrbitAngle(ang);
        setXpos();
        setYpos();
    }

    void setSemiMajorAxis(float val){
        a = val;
    }

    void setSemiMinorAxis(float e){
        b = a * sqrt(1 - pow(e,2.0));
    }


    void setXpos(){
       xPos = a*cos(orbitAngle * PI/180);
    }

    void setYpos(){
        yPos =  b*sin(orbitAngle * PI/180);
    }

    void draw(int zpos){

        ofPushMatrix(); // Save center state
        //ofRotateZ(orbitAngle);
        ofTranslate(xPos, yPos, -zpos);

        ofPushMatrix();
            //rotate about their own axes
            ofRotateY(rotAngle);
            ofPushStyle();
                texture.bind();
                planet.draw();
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
                ofTranslate(xPos, yPos, -250);
                ofRotateX(90);
                mesh.draw();
                ofPopMatrix();
        }
    }

    void setSize(float rad){
        planet.setRadius(rad);
    }

    void setRotationAngle(float ang){
        rotAngle += ang;
    }

    void setOrbitAngle(float orb){
        orbitAngle += orb;
    }

    void setTexture(const string & p_imgPath)
    {
        texture.loadImage(p_imgPath);
    }

    float getRotationAngle(){return rotAngle;}
    float getOrbitAngle(){return orbitAngle;}

private:
    float rotAngle;
    //orbit angle goes from 0 to 2*PI
    float orbitAngle;
    //a and b are semi major and semi-minor axes of ellipse
    float a, b;
    float xPos, yPos;
    ofSpherePrimitive planet;
    ofImage texture;
    string name;
    bool ring;

};
