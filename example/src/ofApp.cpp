#include "ofApp.h"

const int packetSize = 65507;

void debugZIGJSON(ofxZIGJSON &data){
    std::cout << "device:" << data.device.name << endl;
    std::cout << "uuid:" << data.device.uuid << endl;
    std::cout << "timestamp:" << data.time.year << "/" << data.time.month << "/" << data.time.day << "_" << data.time.hour << ":" << data.time.minuite << ":" << data.time.second << endl;
    
    if (data.hasAccel) {
        std::cout << "accel x:" << data.accel.x << " y:" << data.accel.y << " z:" << data.accel.z << endl;
    }
    
    if (data.hasGyro) {
        std::cout << "gyro x:" << data.gyro.x << " y:" << data.gyro.y << " z:" << data.gyro.z << endl;
    }
    
    if (data.hasQuaternion) {
        std::cout << "quaternion x:" << data.quaternion.x() << " y:" << data.quaternion.y() << " z:" << data.quaternion.z() << " w:" << data.quaternion.w() << endl;
    }
    
    if (data.hasCompass) {
        std::cout << "compass angle:" << data.compass.angle << " faceup:" << data.compass.faceup << endl;
    }
    
    if (data.hasPressure) {
        std::cout << "pressure pressure:" << data.pressure.pressure << " altitude:" << data.pressure.altitude << endl;
    }
    
    if (data.hasGPS){
        std::cout << "gps latitude:" << data.gps.latitude << " longitude:" << data.gps.longitude << endl;
    }
    
    if (data.hasTouch) {
        for (int i = 0; i < data.touchPoints.size(); i++) {
            std::cout << "touch " << i << endl;
            if (data.hasTouch2D) {
                std::cout << "x:" << data.touchPoints[i].x << " y:" << data.touchPoints[i].y << endl;
            }
            if (data.hasTouchRadius) {
                std::cout << "radius:" << data.touchPoints[i].radius << endl;
            }
            if (data.hasTouchForce) {
                std::cout << "force:" << data.touchPoints[i].force << endl;
            }
        }
    }
    
    if (data.hasBeacon) {
        for (int i = 0; i < data.beacons.size(); i++) {
            std::cout << "beacon " << i << " uuid:" << data.beacons[i].uuid << " major:" << data.beacons[i].major << " minor:" << data.beacons[i].minor << " rssi:" << data.beacons[i].rssi << endl;
        }
    }
}


//--------------------------------------------------------------
void ofApp::setup(){
    udp.Create();
    udp.Bind(50000);
    udp.SetNonBlocking(true);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    char udpMessage[packetSize];
    int result = udp.Receive(udpMessage,packetSize);
    if (result == SOCKET_ERROR) {
        std::cout << "socket error!" << endl;
    }
    
    string message = udpMessage;
    
    while (message != ""){
        ofxZIGJSON tempData;
        std::cout << message << endl;
        tempData.toParams(message);
        
        bool isNew = true;
        int number = 0;
        for (int i = 0; i < zigDevice.size(); i++) {
            if (tempData.device.uuid == zigDevice[i].device.uuid) {
                std::cout << "we already recieve this device." << endl;
                isNew = false;
                number = i;
            }
        }
        
        if (isNew) {
            std::cout << "we don't recieve this device." << endl;
            zigDevice.push_back(tempData);

        }else{
            zigDevice[number] = tempData;
        }
        
        udp.Receive(udpMessage, packetSize);
        message = udpMessage;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    for (int i = 0;  i < zigDevice.size(); i++) {
        debugZIGJSON(zigDevice[i]);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
