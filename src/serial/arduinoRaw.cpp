/*
 *  arduinoRaw.cpp
 *  ArduinoCommTest
 *
 *  Created by Vincent R. on 07.11.09.
 *  Copyright 2009 www.say-nono.com. All rights reserved.
 *
 */

#include "arduinoRaw.h"

arduinoRaw::arduinoRaw(){
	_portStatus = -1;
	_waitForData = 0;
	_portsFound = 0;
}

arduinoRaw::~arduinoRaw() {
	_isConnected = false;
//	disconnect();
//	if(_port.available()) _port.close();
}

bool arduinoRaw::connect(int deviceID, int baud){
	return connect(getIDStringFromDevicesID(deviceID),baud);
}

bool arduinoRaw::connect(string device, int baud){
//	_port.enumerateDevices();
	printf("Serial connecting...");
	bool res = _port.setup(device.c_str(), baud);
	if(res) printf("connected to %s.\n",device.c_str());
	else printf("failed to connect to %s.\n",device.c_str());
	return res;
}


bool arduinoRaw::selectDevice(string id){

	if(_port.available()){
		_port.close();
		_isConnected = false;
	}
	bool result = false;
	getDevicesList();
	int i = getIDfromDevicesIDString(id);
	printf("------------> getIDfromDevicesIDString : %i   \n",i);
	if(i >= 0){
		currentPortID = i;
		currentPortIDString = id;
		printf("futbolinArduino::selectDevice : connecting to: %s\n",id.c_str() );
		// arduino.connect(id,9600);
		// _port.enumerateDevices();
		result = connect(id.c_str(),57600);
		_isConnected = result;
		if(result) printf(" ... done\n");
		else printf(" ... ERROR\n");
	}else{
		printf("ERROR futbolinArduino::selectDevice : Device not found: %s\n",id.c_str() );
		result = connect(id.c_str(),57600);
		_isConnected = result;
	}
	return result;
}


void arduinoRaw::disconnect(){
	_isConnected = false;
	_port.close();
}

void arduinoRaw::update(){
	if(!_isConnected) return;
	int dataRead=0;
	// try to empty the _port buffer
	while (dataRead<512) {

		int byte = _port.readByte();

		// process data....
		if (byte!=-1) {
			processData((char)(byte));
			dataRead++;
		}
		// _port buffer is empty
		else{
			break;
		}
	}
}




void arduinoRaw::getDevicesList(){
	//	ofSerial serial;
	//	serial.enumerateDevices();

	//---------------------------------------------
#if defined( TARGET_OSX )
	//---------------------------------------------

	//----------------------------------------------------
	//We will find serial devices by listing the directory

	DIR *dir;
	struct dirent *entry;
	dir = opendir("/dev");
	string str			= "";
	string device		= "";
	int deviceCount		= 0;

	if (dir == NULL){
		printf("futbolinArduino: error listing devices in /dev\n");
	} else {
		while ((entry = readdir(dir)) != NULL){
			str = (char *)entry->d_name;
			if( str.substr(0,4) == "tty."){
				printf("OSX serial found #%i = %s \n",deviceCount,str.c_str());
				portsList[deviceCount] = "/dev/"+str;
				deviceCount++;
//			}else{
//				printf("OSX serial found but not accepted #%i = %s \n",deviceCount,str.c_str());
			}
		}
	}
	_portsFound = deviceCount;

	//---------------------------------------------
#endif
    //---------------------------------------------

	//---------------------------------------------
#if defined( TARGET_LINUX )
	//---------------------------------------------

	//----------------------------------------------------
	//We will find serial devices by listing the directory

	DIR *dir;
	struct dirent *entry;
	dir = opendir("/dev");
	string str			= "";
	string device		= "";
	int deviceCount		= 0;

	if (dir == NULL){
		printf("futbolinArduino: error listing devices in /dev\n");
	} else {
		while ((entry = readdir(dir)) != NULL){
			str = (char *)entry->d_name;
			if( str.substr(0,3) == "tty" || str.substr(0,3) == "rfc" ){
				portsList[deviceCount] = "/dev/"+str.c_str();
				deviceCount++;
			}
		}
	}
	_portsFound = deviceCount;

	//---------------------------------------------
#endif
	//---------------------------------------------

	//---------------------------------------------
#ifdef TARGET_WIN32
	//---------------------------------------------

//	enumerateWin32Ports();
	for (int i = 0; i < nPorts; i++){
		portsList[i] = portNamesFriendly[i];
	}
	_portsFound = nPorts;
	//---------------------------------------------
#endif
    //---------------------------------------------

}

//---------------------------------------------
#ifdef TARGET_WIN32
//---------------------------------------------

//------------------------------------

   // needed for serial bus enumeration:
   //4d36e978-e325-11ce-bfc1-08002be10318}
   DEFINE_GUID (GUID_SERENUM_BUS_ENUMERATOR_, 0x4D36E978, 0xE325,
   0x11CE, 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18);

//------------------------------------

void arduinoRaw::enumerateWin32Ports(){

    // thanks joerg for fixes...

	if (bPortsEnumerated == true) return;

	HDEVINFO hDevInfo = NULL;
	SP_DEVINFO_DATA DeviceInterfaceData;
	int i = 0;
	DWORD dataType, actualSize = 0;
	unsigned char dataBuf[MAX_PATH + 1];

	// Reset Port List
	nPorts = 0;
	// Search device set
	hDevInfo = SetupDiGetClassDevs((struct _GUID *)&GUID_SERENUM_BUS_ENUMERATOR_,0,0,DIGCF_PRESENT);
	if ( hDevInfo ){
      while (TRUE){
         ZeroMemory(&DeviceInterfaceData, sizeof(DeviceInterfaceData));
         DeviceInterfaceData.cbSize = sizeof(DeviceInterfaceData);
         if (!SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInterfaceData)){
             // SetupDiEnumDeviceInfo failed
             break;
         }

         if (SetupDiGetDeviceRegistryProperty(hDevInfo,
             &DeviceInterfaceData,
             SPDRP_FRIENDLYNAME,
             &dataType,
             dataBuf,
             sizeof(dataBuf),
             &actualSize)){

			sprintf(portNamesFriendly[nPorts], "%s", dataBuf);
			portNamesShort[nPorts][0] = 0;

			// turn blahblahblah(COM4) into COM4

            char *   begin    = NULL;
            char *   end    = NULL;
            begin          = strstr((char *)dataBuf, "COM");


            if (begin)
                {
                end          = strstr(begin, ")");
                if (end)
                    {
                      *end = 0;   // get rid of the )...
                      strcpy(portNamesShort[nPorts], begin);
                }
                if (nPorts++ > MAX_SERIAL_PORTS)
                        break;
            }
         }
            i++;
      }
   }
   SetupDiDestroyDeviceInfoList(hDevInfo);

   bPortsEnumerated = false;
}


//---------------------------------------------
#endif
//---------------------------------------------


int arduinoRaw::getIDfromDevicesIDString(string id){
	printf("SEARCHING FOR PORT, PORTS AVAILABLE : %i\n",_portsFound);
	for (int i = 0; i < _portsFound; i++){
		int comp = strcmp(id.c_str(),portsList[i].c_str());
		printf("%i  COMP : %i	___   %s   |   %s\n",i,comp,id.c_str(),portsList[i].c_str());
		if(comp == 0) return i;
	}
	return -1;
}

string arduinoRaw::getIDStringFromDevicesID(int id){
	if(id >= 0 && id < _portsFound) return portsList[id];
	return "";
}

void arduinoRaw::printDevices(){
	getDevicesList();
	printf("\nfutbolinArduino: listing devices:\n");
	for (int i = 0; i < _portsFound; i++){
		printf("  [%i] - %s\n", i, portsList[i].c_str());
	}
	printf("\n");
}




void arduinoRaw::selectBestDevice(){
	//	for (int i = 0; i < portsFound; i++){
	//		if(portsList[i].find("usb",0) > -1){
	//			selectDevice(portsList[i]);
	//			return;
	//		}
	//	}
	printf("ERROR futbolinArduino::selectBestDevice : no best match Device found.\n" );
}
void arduinoRaw::nextDevice(){
	int prev = (currentPortID-1 + _portsFound) % _portsFound;
	string id = getIDStringFromDevicesID(prev);
	selectDevice(id);
}
void arduinoRaw::prevDevice(){
	int next = (currentPortID+1 + _portsFound) % _portsFound;
	string id = getIDStringFromDevicesID(next);
	selectDevice(id);
}

string arduinoRaw::getCurrentDeviceIDString(){
	return currentPortIDString;
}



// --------------------------------------------------------------------------------------
void arduinoRaw::addListener(arduinoRawListener* listener){
	ofAddListener(newMessageEvent, listener, &arduinoRawListener::newArduinoRawMessage);
}

void arduinoRaw::removeListener(arduinoRawListener* listener){
	ofRemoveListener(newMessageEvent, listener, &arduinoRawListener::newArduinoRawMessage);
}
// ------------------------------ private functions

int arduinoRaw::sendDataSingleBytes(unsigned char* data,int length){
	int cnt = 0;
	for (int i=0; i<length; i++) {
		//		printf("%X ",data[i]);
		cnt += _port.writeByte(*(data+i));
	}
	return cnt;
}
int arduinoRaw::sendData(unsigned char* data,int length){
	printf("-------sendSerialFrame BEGIN------->");
	int result = _port.writeBytes(data, length);
	printf("<-------sendSerialFrame END---------> %i / %i\n",result,length);

	return result > 0;//_port.writeBytes(data, length) > 0;
//	return _port.writeBytes(data, length);
}

//----------------------------------------------------------------
int arduinoRaw::readBytes(unsigned char * buffer, int length){
	return _port.readBytes(buffer,length);
}

void arduinoRaw::processData(unsigned char inputData){

	if(!_isConnected) return;

	if((Byte)inputData==FIRMATA_END_SYSEX){
//		printf(" Message : %s \n",_storedInputData);
		arduinoRawEventArgs eventArgs;
        eventArgs.msg.assign(_storedInputData);
		eventArgs.msg = eventArgs.msg.substr(0,_dataLength);
        ofNotifyEvent( newMessageEvent, eventArgs, this );
	}else if((Byte)inputData==FIRMATA_START_SYSEX){
		_dataLength = 0;
	}else{
		_storedInputData[_dataLength] = inputData;
		_dataLength++;
		_dataLength = min(_dataLength,(int)sizeof(_storedInputData)-1);
	}

}
