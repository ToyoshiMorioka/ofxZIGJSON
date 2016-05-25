//
//  ofxZIGJSON.h
//
//  Created by MORIOKAToyoshi on 2016/05/19.
//
//

#ifndef ofxZIGJSON_h
#define ofxZIGJSON_h

#include "ofMain.h"
#include "jsonxx.h"

struct Device{
    string name;
    string uuid;
};

struct Time{
    int year;
    int month;
    int day;
    int hour;
    int minuite;
    float second;
    string timeString;
};

struct Pressure{
    float pressure;
    float altitude;
};

struct Compass{
    bool faceup;
    float angle;
};

struct GPS{
    float latitude;
    float longitude;
};

struct TouchPoint{
    float x;
    float y;
    float radius;
    float force;
};

struct Beacon{
    string uuid;
    int rssi;
    int major;
    int minor;
};

class ofxZIGJSON{
public:
    Device device;
    Time time;
    
    bool hasAccel;
    ofVec3f accel;
    
    bool hasGyro;
    ofVec3f gyro;
    
    bool hasQuaternion;
    ofQuaternion quaternion;
    
    bool hasCompass;
    Compass compass;
    
    bool hasPressure;
    Pressure pressure;
    
    bool hasGPS;
    GPS gps;
    
    bool hasTouch;
    bool hasTouch2D;
    bool hasTouchRadius;
    bool hasTouchForce;
    vector<TouchPoint>touchPoints;
    
    bool hasBeacon;
    vector<Beacon>beacons;
    
    ofxZIGJSON();
    ~ofxZIGJSON();
    
    bool toParams(string &message);
    bool toStrings(string &message);
    
private:
    bool getJsonObject(jsonxx::Object &dst, jsonxx::Object &src, string key);
    bool getJsonArray(jsonxx::Array &dst, jsonxx::Object &src, string key);
    bool getStringFromObject(string &dst, jsonxx::Object &src, string key);
    bool getValueFromObject(int &dst, jsonxx::Object &src, string key);
     bool getValueFromObject(bool &dst, jsonxx::Object &src, string key);
    bool getValueFromObject(float &dst, jsonxx::Object &src, string key);
    bool getValueFromObject(double &dst, jsonxx::Object &src, string key);
    
    bool getTimeStampFromString(Time &dst, string src);
};

#endif /* ofxZIGJSON_h */
