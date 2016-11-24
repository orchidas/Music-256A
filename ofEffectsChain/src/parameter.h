#ifndef PARAMETER_H
#define PARAMETER_H
#include<string>
#include<map>
#include<tuple>
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

    void setMaxMinValues(tuple<float, float> mm){
        min = get<0>(mm);
        max = get<1>(mm);
    }

    tuple<float, float> getMinMaxValues(){
        return make_tuple(min,max);
    }



private:
    //using a hash map with a key-value pair here seems to make a lot of sense
    map<const string, float> paramMap;
    float min;
    float max;

};

#endif // PARAMETER_H
