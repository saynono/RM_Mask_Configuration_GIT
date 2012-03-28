/*
 *  SerialManager.cpp
 *  RM_Mask_Configuration_of_070
 *
 *  Created by say nono on 02.08.11.
 *  Copyright 2011 nono. All rights reserved.
 *
 */

#include "SerialManager.h"


void SerialManager::setup(){
	serial.setVerbose(true);
	serial.listDevices();
	start();
}

void SerialManager::connect(){
	connectConfigConnection();
}


bool SerialManager::connectTestConnection(){
	close();
//	_isSerialConnected = serial.setup("/dev/tty.usbserial-FTF4AMO2", B115200);
	_isSerialConnected = serial.setup(0, B115200);
	if(_isSerialConnected){
		printf("starting test mode\n");
		_testMode = true;
	}
	return _isSerialConnected;
}

bool SerialManager::connectConfigConnection(){
	close();
	_isSerialConnected = serial.setup(0, B57600);
	if(_isSerialConnected){
		printf("starting config mode\n");
		_testMode = false;
	}
	return _isSerialConnected;
}


void SerialManager::close(){
	serial.close();
//	serialCommands.clear();
	serialCommands.front();
	while(!serialCommands.empty()){
		serialCommands.pop();
	}
}

void SerialManager::sendBytes(unsigned char* data, int length){
	SerialCommand cmd;
	cmd.data = data;
	cmd.length = length;
	serialCommands.push(cmd);
}


void SerialManager::read(){
	int nBytesReturnSize = 70;
	unsigned char bytesReturned[nBytesReturnSize];
	
	memset(bytesReturned, 0, nBytesReturnSize);
	int nRead = 0;
	bool bRead = false;
	while( (nRead = serial.readBytes( bytesReturned, nBytesReturnSize)) > 0){
		bRead = true;
		nBytesRead = nRead;
	};
	
	//	memcpy(bytesReadString, bytesReturned, nBytesReturnSize);
	if(bRead){
		//			serial.sendData(bytesReturned, nRead);
		printf("Received (%i):								",nBytesRead,bytesReturned);
		for(int i=0;i<nBytesRead;i++){
			printf("%X  ",bytesReturned[i]);
		}
		printf("\n");
	}	
}


void SerialManager::start(){
	startThread(true, false);   // blocking, verbose
}

void SerialManager::stop(){
	stopThread();
}

//--------------------------
void SerialManager::threadedFunction(){
	
	while( isThreadRunning() != 0 ){
		if( lock() ){
//			printf("serialCommands length : %i\n",(int)serialCommands.size());
			bool hasElement = false;
			SerialCommand cmd;
			if(!serialCommands.empty()){
				cmd = serialCommands.front();
				serialCommands.pop();
				hasElement = true;
			}
			
			unlock();
			
			if(hasElement){
//				ofSleepMillis(1 * 100);
				int res = serial.writeBytes(cmd.data, cmd.length);
//				printf("command length : %i -> sent: %i			",(int)cmd.length,res);
//				for(int i=0;i<cmd.length;i++){
//					printf("%X  ",cmd.data[i]);
//				}
//				printf("\n");
			}
			
			if(_testMode) ofSleepMillis(1 * 20);
			else ofSleepMillis(1 * 10);
			
			read();
			
		}
	}
}

bool SerialManager::isTestMode(){
	return _testMode;
}

bool SerialManager::isConnected(){
	return _isSerialConnected;
}


//int SerialManager::writeBytes(unsigned char* data,int length){
//}
