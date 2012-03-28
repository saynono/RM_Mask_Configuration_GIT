/*
 *  arduinoRaw.h
 *  ArduinoCommTest
 *
 *  Created by Vincent R. on 07.11.09.
 *  Copyright 2009 www.say-nono.com. All rights reserved.
 *
 */


#ifndef NONO_ARDUINO_RAW_H
#define NONO_ARDUINO_RAW_H

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "ofEvents.h"
#include "arduinoRawEvent.h"
#include "ofSerial.h"

#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
#include <sys/ioctl.h>
#include <getopt.h>
#include <dirent.h>
#endif


#if defined( TARGET_WIN32 )
	#include <winbase.h>
	#include <tchar.h>
	#include <iostream>
	#include <string.h>
	#include <setupapi.h>
	#include <regstr.h>
	#define MAX_SERIAL_PORTS 256
	 #include <winioctl.h>
	#ifdef __MINGW32__
			#define INITGUID
			#include <initguid.h> // needed for dev-c++ & DEFINE_GUID
    #endif
#endif

using namespace std;

#define MAX_DATA_BYTES									64 // max number of data bytes in non-Sysex messages
#define FIRMATA_START_SYSEX                             0xF0 // start a MIDI Sysex message
#define FIRMATA_END_SYSEX                               0xF7 // end a MIDI Sysex message

class arduinoRaw{

public:
	arduinoRaw();

	virtual ~arduinoRaw();

	// --- setup functions
	bool connect(string device, int baud = 115200);
	bool connect(int deviceID, int baud = 115200);
	// opens a serial port connection to the arduino

	void disconnect();
	// closes the serial port connection

	void update();
	// polls data from the serial port, this has to be called periodically

	bool isInitialized();
	// returns true if a succesfull connection has been established and the Arduino has reported a firmware

	void printDevices();
	void nextDevice();
	void prevDevice();
	bool selectDevice(string id);
	void selectBestDevice();
	string getCurrentDeviceIDString();
	
	int sendDataSingleBytes(unsigned char* data, int length);
	int sendData(unsigned char* data, int length);
	
	int readBytes(unsigned char * buffer, int length);

protected:

	void processData(unsigned char inputData);
	void processDigitalPort(int port, unsigned char value);
	void getDevicesList();
	int getIDfromDevicesIDString(string id);
	string getIDStringFromDevicesID(int id);



	ofSerial _port;
	int _portStatus;
	bool _isConnected;

	// --- data processing variables
	int _dataLength;
	int _waitForData;
	int _executeMultiByteCommand;
	int _multiByteChannel; // indicates which pin data came from

	// --- data holders
//	unsigned char _storedInputData[MAX_DATA_BYTES];
	char _storedInputData[MAX_DATA_BYTES];
	vector<unsigned char> _sysExData;

	list<vector<unsigned char> > _sysExHistory;
	// maintains a history of received sysEx messages (excluding SysEx messages in the extended command set)


	int			maxPorts;
	string		portsList[10];
	int			currentPortID;
	string		currentPortIDString;
	int			_portsFound;

    #ifdef TARGET_WIN32

        char 		** portNamesShort;//[MAX_SERIAL_PORTS];
        char 		** portNamesFriendly; ///[MAX_SERIAL_PORTS];
        HANDLE  	hComm;		// the handle to the serial port pc
        int	 		nPorts;
        bool 		bPortsEnumerated;
        void 		enumerateWin32Ports();
        COMMTIMEOUTS 	oldTimeout;	// we alter this, so keep a record

    #endif

	// events
public:
	void addListener(int id,arduinoRawListener * listener);
	void removeListener(int id,arduinoRawListener * listener);
	void addListener(arduinoRawListener * listener);
	void removeListener(arduinoRawListener * listener);


	map<int,ofEvent<arduinoRawEventArgs>*> 	newIdMessageEvents;
	ofEvent<arduinoRawEventArgs> 			newMessageEvent;

};

#endif
