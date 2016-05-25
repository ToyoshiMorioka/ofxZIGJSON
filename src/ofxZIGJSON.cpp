//
//  ofxZIGJSON.cpp
//
//  Created by MORIOKAToyoshi on 2016/05/19.
//
//

#include "ofxZIGJSON.h"

using namespace jsonxx;

ofxZIGJSON::ofxZIGJSON(){
    device.name = "unknown";
    device.uuid = "unknown";
    time.year = 0;
    time.month = 0;
    time.day = 0;
    time.hour = 0;
    time.minuite = 0;
    time.second = 0.0;
    time.timeString = "";
    hasAccel = false;
    accel.set(ofVec3f(0.0,0.0,0.0));
    hasGyro = false;
    gyro.set(ofVec3f(0.0,0.0,0.0));
    hasQuaternion = false;
    quaternion.set(0.0, 0.0, 0.0, 0.0);
    hasCompass = false;
    compass.faceup = false;
    compass.angle = 0.0;
    hasPressure = false;
    pressure.altitude = 0.0;
    pressure.pressure = 0.0;
    hasGPS = false;
    gps.latitude = 0.0;
    gps.longitude = 0.0;
    hasTouch = false;
    hasTouch2D = false;
    hasTouchRadius = false;
    hasTouchForce = false;
    touchPoints.resize(0);
    hasBeacon = false;
    beacons.resize(0);
}

ofxZIGJSON::~ofxZIGJSON(){
    
}

bool ofxZIGJSON::toParams(string &message){
    Object jsonObject;
    bool result = jsonObject.parse(message);
    
    if (!result){
        std::cout << "parse failed!" << endl;
        return false;
    }
    
    // device info
    Object deviceInfo;
    if (getJsonObject(deviceInfo, jsonObject, "device")) {
        getStringFromObject(device.name, deviceInfo, "name");
        getStringFromObject(device.uuid, deviceInfo, "uuid");
    }
    
    // time stamp
    string timeStampString;
    getStringFromObject(timeStampString, jsonObject, "timestamp");
    time.timeString = timeStampString;
    getTimeStampFromString(time, timeStampString);
    
    // sensor
    Object sensorInfo;
    getJsonObject(sensorInfo, jsonObject, "sensordata");
    
    // accel
    Object accelInfo;
    if (getJsonObject(accelInfo, sensorInfo, "accel")) {
        hasAccel = true;
        getValueFromObject(accel.x, accelInfo, "x");
        getValueFromObject(accel.y, accelInfo, "y");
        getValueFromObject(accel.z, accelInfo, "z");
    }
    
    // gyro
    Object gyroInfo;
    if (getJsonObject(gyroInfo, sensorInfo, "gyro")) {
        hasGyro = true;
        getValueFromObject(gyro.x, gyroInfo, "x");
        getValueFromObject(gyro.y, gyroInfo, "y");
        getValueFromObject(gyro.z, gyroInfo, "z");
    }
    
    // quaternion
    Object quaternionInfo;
    if (getJsonObject(quaternionInfo, sensorInfo, "quaternion")) {
        hasQuaternion = true;
        float x = 0.0;
        float y = 0.0;
        float z = 0.0;
        float w = 0.0;
        getValueFromObject(x, quaternionInfo, "x");
        getValueFromObject(y, quaternionInfo, "y");
        getValueFromObject(z, quaternionInfo, "z");
        getValueFromObject(w, quaternionInfo, "w");
        quaternion.set(x, y, z, w);
    }
    
    // compass
    Object compassInfo;
    if (getJsonObject(compassInfo, sensorInfo, "compass")) {
        hasCompass = true;
        getValueFromObject(compass.faceup, compassInfo, "faceup");
        getValueFromObject(compass.angle, compassInfo, "compass");
    }
    
    // pressure
    Object pressureInfo;
    if (getJsonObject(pressureInfo, sensorInfo, "pressure")) {
        hasPressure = true;
        getValueFromObject(pressure.altitude, pressureInfo, "altitude");
        getValueFromObject(pressure.pressure, pressureInfo, "pressure");
    }
    
    // gps
    Object gpsInfo;
    if (getJsonObject(gpsInfo, sensorInfo, "gps")) {
        hasGPS = true;
        getValueFromObject(gps.latitude, gpsInfo, "latitude");
        getValueFromObject(gps.longitude, gpsInfo, "longitude");
    }
    
    // touch
    Array touchArray;
    if (getJsonArray(touchArray, sensorInfo, "touch")) {
        int count = 0;
        touchPoints.resize(0);
        hasTouch = true;
        while (touchArray.has<Object>(count)) {
            Object tempTouch = touchArray.get<Object>(count);
            TouchPoint temp;
            if (getValueFromObject(temp.x, tempTouch, "x")) {
                hasTouch2D = true;
            }
            if (getValueFromObject(temp.y, tempTouch, "y")) {
                hasTouch2D = true;
            }
            
            if (getValueFromObject(temp.radius, tempTouch, "radius")) {
                hasTouchRadius = true;
            }
            if (getValueFromObject(temp.force, tempTouch, "force")) {
                hasTouchForce = true;
            }
            touchPoints.push_back(temp);
            count++;
        }
    }
    
    // beacon
    Array beaconArray;
    if (getJsonArray(beaconArray, sensorInfo, "beacon")) {
        int count = 0;
        beacons.resize(0);
        hasBeacon = true;
        while (beaconArray.has<Object>(count)) {
            Object tempBeacon = beaconArray.get<Object>(count);
            Beacon temp;
            
            getStringFromObject(temp.uuid, tempBeacon, "uuid");
            getValueFromObject(temp.rssi, tempBeacon, "rssi");
            getValueFromObject(temp.major, tempBeacon, "major");
            getValueFromObject(temp.minor, tempBeacon, "minor");
            beacons.push_back(temp);
            count++;
        }
    }
    
    return true;
}

bool ofxZIGJSON::toStrings(string &message){
    Object data;
    
    // device info
    Object deviceInfo;
    deviceInfo << "name" << device.name;
    deviceInfo << "uuid" << device.uuid;
    data << "device" << deviceInfo;
    
    // timestamp
    //    "timestamp" : "2016/04/06 15:36"
    string timestampString = ofToString(time.year) + "/" + ofToString(time.month, 2, '0') + "/" + ofToString(time.day, 2, '0') + " " + ofToString(time.hour, 2, '0') + ofToString(time.minuite, 2, '0') + ":" + ofToString(time.second, 3);
    data << "timestamp" << timestampString;
    
    // sensordata
    Object sensorInfo;
    
    if (hasAccel) {
        Object accelInfo;
        accelInfo << "x" << accel.x;
        accelInfo << "y" << accel.y;
        accelInfo << "z" << accel.z;
        sensorInfo << "accel" << accelInfo;
    }
    
    if (hasGyro) {
        Object gyroInfo;
        gyroInfo << "x" << gyro.x;
        gyroInfo << "y" << gyro.y;
        gyroInfo << "z" << gyro.z;
        sensorInfo << "gyro" << gyroInfo;
    }
    
    if (hasQuaternion) {
        Object quaternionInfo;
        quaternionInfo << "x" << quaternion.x();
        quaternionInfo << "y" << quaternion.y();
        quaternionInfo << "z" << quaternion.z();
        quaternionInfo << "w" << quaternion.w();
        sensorInfo << "quaternion" << quaternionInfo;
    }
    
    if (hasCompass) {
        Object compassInfo;
        compassInfo << "compass" << compass.angle;
        compassInfo << "faceup" << (int)compass.faceup;
        sensorInfo << "compass" << compassInfo;
    }
    
    if (hasPressure) {
        Object pressureInfo;
        pressureInfo << "pressure" << pressure.pressure;
        pressureInfo << "altitude" << pressure.altitude;
        sensorInfo << "pressure" << pressureInfo;
    }
    
    if (hasGPS) {
        Object gpsInfo;
        gpsInfo << "latitude" << gps.latitude;
        gpsInfo << "longitude" << gps.longitude;
        sensorInfo << "gps" << gpsInfo;
    }
    
    if (hasTouch) {
        Array touchArray;
        for (int i =0; i < touchPoints.size(); i++) {
            Object touch;
            if (hasTouch2D) {
                touch << "x" << touchPoints[i].x;
                touch << "y" << touchPoints[i].y;
            }
            if (hasTouchRadius) {
                touch << "radius" << touchPoints[i].radius;
            }
            if (hasTouchForce) {
                touch << "force" << touchPoints[i].force;
            }
            touchArray << touch;
        }
        sensorInfo << "touch" << touchArray;
    }
    
    if (hasBeacon) {
        Array beaconArray;
        for (int i =0; i < beacons.size(); i++) {
            Object beacon;
            beacon << "uuid" << beacons[i].uuid;
            beacon << "major" << beacons[i].major;
            beacon << "minor" << beacons[i].minor;
            beacon << "rssi" << beacons[i].rssi;
            beaconArray << beacon;
        }
        sensorInfo << "beacon" << beaconArray;
    }
    
    data << "sensordata" << sensorInfo;
    
    const string result = data.write(0);
    message = result;
    
    return true;
}

bool ofxZIGJSON::getJsonObject(Object &dst, Object &src, string key){
    if (src.has<Object>(key)) {
        dst = src.get<Object>(key);
        return true;
    }else{
        //std::cout << "no " << key << " element." << endl;
        return false;
    }
}

bool ofxZIGJSON::getJsonArray(Array &dst, Object &src, string key){
    if (src.has<Array>(key)) {
        dst = src.get<Array>(key);
        return true;
    }else{
        //std::cout << "no " << key << " element." << endl;
        return false;
    }
}

bool ofxZIGJSON::getStringFromObject(string &dst, Object &src, string key){
    if (src.has<string>(key)) {
        dst = src.get<string>(key);
        return true;
    }else{
        //std::cout << "no " << key << " element." << endl;
        return false;
    }
}

bool ofxZIGJSON::getValueFromObject(int &dst, jsonxx::Object &src, string key){
    if (src.has<Number>(key)) {
        dst = src.get<Number>(key);
        return true;
    }else{
        //std::cout << "no " << key << " element." << endl;
        return false;
    }
}

bool ofxZIGJSON::getValueFromObject(bool &dst, jsonxx::Object &src, string key){
    if (src.has<Number>(key)) {
        dst = src.get<Number>(key);
        return true;
    }else{
        //std::cout << "no " << key << " element." << endl;
        return false;
    }
}

bool ofxZIGJSON::getValueFromObject(float &dst, jsonxx::Object &src, string key){
    if (src.has<Number>(key)) {
        dst = src.get<Number>(key);
        return true;
    }else{
        //std::cout << "no " << key << " element." << endl;
        return false;
    }
}

bool ofxZIGJSON::getValueFromObject(double &dst, jsonxx::Object &src, string key){
    if (src.has<Number>(key)) {
        dst = src.get<Number>(key);
        return true;
    }else{
        //std::cout << "no " << key << " element." << endl;
        return false;
    }
}

bool ofxZIGJSON::getTimeStampFromString(Time &dst, string src){
    //    "timestamp" : "2016/04/06 15:36"
    
    Time temp;
    bool success = true;
    
    vector<string> years = ofSplitString(src, "/");
    if (years.size() >= 3) {
        temp.year = ofToInt(years[0]);
        temp.month = ofToInt(years[1]);
        vector<string>days = ofSplitString(years[2], " ");
        if (days.size() >= 2) {
            temp.day = ofToInt(days[0]);
            vector<string> hours = ofSplitString(days[1], ":");
            if (hours.size() >= 3) {
                temp.hour = ofToInt(hours[0]);
                temp.minuite = ofToInt(hours[1]);
                temp.second = ofToFloat(hours[2]);
            }else{
                success = false;
            }
        }else{
            success = false;
        }
    }else{
        success = false;
    }
    
    if (success) {
        dst = temp;
        return true;
    }else{
        std::cout << "time stamp parse error!" << endl;
        return false;
    }
}
