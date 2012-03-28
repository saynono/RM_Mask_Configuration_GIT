/*
 *  SerialManager.h
 *  RM_Mask_Configuration_of_070
 *
 *  Created by say nono on 02.08.11.
 *  Copyright 2011 nono. All rights reserved.
 *
 */


#pragma once

#include "ofMain.h"
#include "ofxThread.h"
#include <vector>
using namespace std;

struct SerialCommand {
	unsigned char* data;
	int length;
};

class SerialManager : public ofxThread{
	
public:

	void setup();
	void connect();
	bool connectTestConnection();
	bool connectConfigConnection();	
	void close();
	void sendBytes(unsigned char* data, int length);
	void read();
	
	void start();
	void stop();
	void threadedFunction();
	
	bool isTestMode();
	bool isConnected();	
//	int writeBytes(unsigned char* data,int length);

private:
	
	ofSerial				serial;
//	vector<SerialCommand>	serialCommands;
	queue<SerialCommand>	serialCommands;	
	
	int						nBytesRead;	
	bool					_isSerialConnected;
	bool					_testMode;

};