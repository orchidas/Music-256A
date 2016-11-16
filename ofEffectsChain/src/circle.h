#ifndef CIRCLE_H
#define CIRCLE_H

#include<math.h>
using namespace std;


class Circle{
private:
    float radius;
    float centerX;
    float centerY;
    ofImage bindImg;
    ofMesh mesh;
    ofPolyline border;
    ofColor color;

public:
    Circle(){
        radius = 0.0;
        centerX = 0;
        centerY = 0;
    }

    ~Circle(){}

    void setup(float radius, tuple<float,float> center, const string &p_img, ofColor color){
        setRadius(radius);
        setCenter(center);
        setImage(p_img);
        drawMeshPoints();
        //need this to keep tex coords between 0-1
        ofEnableNormalizedTexCoords();
        this->color = color;

    }

    void setRadius(float r){
        radius = r;
    }

    float getRadius(){
        return radius;
    }

    void setCenter(tuple<float,float> c){
        centerX = get<0>(c);
        centerY = get<1>(c);
    }

    tuple<float, float> getCenter(){
        return make_tuple(centerX, centerY);
    }

    void setImage(const string& path){
        bindImg.load(path);
    }

    void drawMeshPoints(){
        int nPts  = 60;

        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        mesh.addVertex(ofVec2f());
        mesh.addTexCoord(ofPoint(0.5, 0.5));

           for(int i = 0; i < nPts; i++){
               float n = ofMap(i, 0, nPts - 1, 0.0, TWO_PI);
               float x = sin(n);
               float y = cos(n);
               mesh.addVertex(ofPoint(x * radius, y * radius));
               border.addVertex(ofPoint(x * (radius + 1), y * (radius + 1)));
               mesh.addTexCoord(ofPoint((x + 1) / 2, (y + 1) / 2));
           }
    }

    void update(){}

    void draw(){
        ofPushMatrix();
        ofTranslate(centerX, centerY);


        bindImg.bind();
        ofSetColor(color);
        mesh.draw();
        ofSetColor(0,0,0);
        border.draw();
        bindImg.unbind();

        ofPopMatrix();
    }

};


#endif // CIRCLE_H
