#ifndef PARAMETER_H
#define PARAMETER_H
#include<string>
#include<map>
#include<tuple>
#include<iterator>
using namespace std;

class Parameter{
public:
    Parameter(){}
    ~Parameter(){
        paramMap.clear();
    }

    void setParam(const string &name, float val){
        paramMap[name] = val;
    }

    map<const string, float> getParamList(){
        return paramMap;
    }

    float getParamValue(const string& name){
        try{
            if(paramMap.find(name) == paramMap.end())
                throw "Parameter does not exist for this effect!";
        }
        catch(const char* e){
            return -99.99;
        }
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
