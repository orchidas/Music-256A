#ifndef PARAMETER_H
#define PARAMETER_H
#include<string>
#include<map>
#include<iterator>
using namespace std;

//class parameter contains effect parameters
/*class Parameter{
public:
    Parameter(){
    }

    ~Parameter(){}

    void setName(const string &n){
        paramName = n;
    }

    const string & getName(){
        return paramName;
    }

    void setValue(float val){
        paramValue = val;
    }

    float getValue(){
        return paramValue;
    }

private:
    string paramName;
    float paramValue;
};*/

class Parameter{
public:
    Parameter(){
        //paramMap = map<const string, float>();
    }
    ~Parameter(){
        paramMap.clear();
        //delete paramMap;
    }

    void setParam(const string &name, float val){
        paramMap[name] = val;
    }

    map<const string, float> getParamList(){
        return paramMap;
    }

    float getParamValue(const string& name){
        if(paramMap.find(name) == paramMap.end()){
            throw "Parameter does not exist for this effect!";
        }
        else
            return paramMap[name];
    }

    /*void setParamMaxMin(tuple<float,float> maxMin){
        valMax = get<0>(maxMin);
        valMin = get<1>(maxMin);
    }

    tuple<float,float> getParamMaxMin(){
        return make_tuple(valMax, valMin);
    }*/

private:
    //using a hash map with a key-value pair here seems to make a lot of sense
    map<const string, float> paramMap;
    //float valMax;
    //float valMin;
};

#endif // PARAMETER_H
