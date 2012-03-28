#include "testApp.h"


#define WIDTH_RES 320
#define HEIGHT_RES 320
#define WIDTH 640
#define HEIGHT 480
#define WIDTH_DISPLAY 640
#define HEIGHT_DISPLAY 480

//#define LED_WIDTH 16 //20
//#define LED_HEIGHT 28 //16
#define LED_WIDTH 64 //20
#define LED_HEIGHT 4 //16

#define RECT_SIZE_MIN 16
#define RECT_SIZE_MAX HEIGHT


//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(25);
	
//	//TODO Need a XPlatform solution for this
//#ifdef TARGET_OSX
//	ofDisableDataPath();
//	CFBundleRef mainBundle = CFBundleGetMainBundle();
//	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
//	char path[PATH_MAX];
//	CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX);
//	CFRelease(resourcesURL);
//	chdir(path);
//#endif
	
	sendSerialData = false;
	
	int s = MIN(WIDTH,HEIGHT)*.1;
	rectSizeWidth = s;
	rectSizeHeight = s/5*4;
	imageRect.x = (WIDTH - rectSizeWidth) * .3;
	imageRect.y = (HEIGHT - rectSizeHeight) * .3;	
	imageRect.width = rectSizeWidth;
	imageRect.height = rectSizeHeight;	
	
	resultImage.allocate(LED_WIDTH,LED_HEIGHT, OF_IMAGE_COLOR);
    colorImg.allocate(WIDTH,HEIGHT, OF_IMAGE_COLOR);
	imgOrg.allocate(WIDTH,HEIGHT);
	imgCrop.allocate(imageRect.width, imageRect.height);	
	imgResized.allocate(LED_WIDTH, LED_HEIGHT);
	
	string file;
	
	file = "../../../data/mono.ttf";
	fontMono.loadFont(file,7, true, true);
	
	file = "../../../data/test1.png";
	bitmapImage.loadImage(file);
	
	//	file = "../../../data/colourspot.avi";
	//	vidPlayer.loadMovie(file);
		
//	resultPixels = new unsigned char[LED_WIDTH*LED_HEIGHT*3];
//	resultSend = new unsigned char[LED_WIDTH*LED_HEIGHT*3+13];
	resultPixels = new unsigned char[2000];
	resultSend = new unsigned char[2000];

	// Zeit bis man automatisch den Config-Modus verlaesst, sollte in intervallen aufgerufen werden
	configStartLength = 20;
	configStart  = new unsigned char[configStartLength];
	for (int i=0; i<configStartLength; i++) {
		configStart[i] = 0x00;
	}
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	configValuePositionXlow					= 0;
	configValuePositionXhigh				= 0;
	configValuePositionYlow					= 0;
	configValuePositionYhigh				= 0;
	configValuePositionRotation				= 0;
	configValuePositionSmoothing			= 10;
	configValuePositionPower				= 60;
	configValuePositionPositionChainLow		= 0;
	configValuePositionPositionChainHigh	= 0;
//	configValuePositionBaudrate				= 16;	// 115700
	configValuePositionBaudrate				= 3;	// 500kbaud
//	configValuePositionBaudrate				= 7;	// 250kbaud
	configValuePositionPowerLimit			= 200;
	configValuePositionTemperatureLimit		= 150;
	configValuePositionBlankingTimeout		= 20;
	
//	configNextTileLength = 5;
//	configNextTile  = new unsigned char[configNextTileLength];
//	configNextTile[0] = 0xff;
//	configNextTile[1] = 0x03;
//	configNextTile[2] = 0x00;
//	configNextTile[3] = 0x80;
//	configNextTile[0] = 0x00;
//	configNextTile[1] = 0xFC;
//	configNextTile[2] = 0xFF;
//	configNextTile[3] = 0xF2;
//	configNextTile[4] = 0x00;n
	configNextTileLength = 1;
	configNextTile  = new unsigned char[configNextTileLength];
	configNextTile[0] = 0x80;
	
	
	// Zeit bis man automatisch den Config-Modus verlaesst, sollte in intervallen aufgerufen werden
//	configStayAliveLength = 6;
//	configStayAlive  = new unsigned char[configStayAliveLength];
/nnnnnnnnnnnnnnnnnn/	configStayAlive[0] = 0xff;
//	configStayAliven[1] = 0x05;
//	configStayAlive[n2] = 0x00;
//	configStayAlive[3x ] = 0x85;
//	configStayAlive[4] = 0x00;
//	configStayAlive[5] = 0x30; // 0x01 waere ca 1 Sekunde?
	configStayAliveLength = 3;
	configStayAlive  = new unsigned char[configStayAliveLength];
	configStayAlive[0] = 0x85;
	configStayAlive[1] = 0x00;
	configStayAlive[2] = 0x30; // 0x01 waere ca 1 Sekunde?
	
	
	// Verlassen des Config Modus
//	configExitConfigLength = 5;
//	configExitConfig = new unsigned char[4];
//	configExitConfig[0] = 0xff;
//	configExitConfig[1] = 0x03;
//	configExitConfig[2] = 0x00;
//	configExitConfig[3] = 0x82;	
//	configExitConfig[0] = 0x00;
//	configExitConfig[1] = 0xfc;
//	configExitConfig[2] = 0xff;
//	configExitConfig[3] = 0xf6;	
//	configExitConfig[4] = 0x00;
	configExitConfigLength = 1;
	configExitConfig  = new unsigned char[configNextTileLength];
	configExitConfig[0] = 0x82;
	
	
	// Aufleuchten des aktuellen Moduls in blau
//	configEditCurrentLength = 4;
//	configEditCurrent = new unsigned char[4];
//	configEditCurrent[0] = 0xff;
//	configEditCurrent[1] = 0x03;
//	configEditCurrent[2] = 0x00;
//	configEditCurrent[3] = 0x83;	
//	configEditCurrent[0] = 0x00;
//	configEditCurrent[1] = 0xfc;
//	configEditCurrent[2] = 0xff;
//	configEditCurrent[3] = 0xf2;	
//	configEditCurrent[4] = 0x00;
	configEditCurrentLength = 1;
	configEditCurrent = new unsigned char[configEditCurrentLength];
	configEditCurrent[0] = 0x83;	
	
	
	configTileSettingsLength = 31;
	configTileSettings = new unsigned char[31];
	configTileSettings[0] = 0xff;
	configTileSettings[1] = 4+ 2 * 13;
	configTileSettings[2] = 0x00;
	configTileSettings[3] = 0x81;
	configTileSettings[4] = 0x00;
	setCurrentConfigTile();
//	// xpos
///*0*/	configTileSettings[5] = 0x00;
//		configTileSettings[6] = 0x00;
///*1*/	configTileSettings[7] = 0x00;	
//		configTileSettings[8] = 0x00;	
//	// ypos
///*2*/	configTileSettings[9] = 0x00;
//		configTileSettings[10] = 0x00;
///*3*/	configTileSettings[11] = 0x00;
//		configTileSettings[12] = 0x00;
//	// orientation
///*4*/	configTileSettings[13] = 0x00;
//		configTileSettings[14] = 0x00;
//	// default smoothing
///*5*/	configTileSettings[15] = 0;
//		configTileSettings[16] = 10;
//	// default power
///*6*/	configTileSettings[17] = 0;
//		configTileSettings[18] = 10;
//	// default position in chain -> not used
///*7*/	configTileSettings[19] = 0;
//		configTileSettings[20] = 0;
///*8*/	configTileSettings[21] = 0;	
//		configTileSettings[22] = 0;	
//	// baudrate -> 2mbaud/(n+1)
///*9*/	configTileSettings[23] = 0;
//		configTileSettings[24] = 16;
//	// power limit
///*10*/	configTileSettings[25] = 0;
//		configTileSettings[26] = 10;
//	// temperature limit
//	
///*11*/	configTileSettings[27] = 150&0xf0;
//		configTileSettings[28] = 150&0x0f;
//	// blanking tmieout
///*12*/	configTileSettings[29] = 80&0xf0;
//		configTileSettings[30] = 80&0x0f;
	
	resultSend[0] = 0xff;	// beginning of frame
	resultSend[1] = 0x0b;   //0x0b;	// length of message without the initial 0xff
	resultSend[2] = 0x00;	// length high byte, not in use
	resultSend[3] = 0x03;	// command byte:
	//								0x01 = Send per-tile image data frame for one tile.
	//								0x02 = Global Update.
	//								0x03 = Send whole-frame image data.
	//								0x90 = Display temperature
	
	resultSend[4] = (int)LED_WIDTH;//0x14;	// width low byte
	resultSend[5] = 0x00;	// width high byte
	resultSend[6] = (int)LED_HEIGHT;//0x10;	// height low byte
	resultSend[7] = 0x00;	// height high byte
	resultSend[8] = _LEDSmoothing;	// smoothing : 0 default
	resultSend[9] = _LEDPower;	// power : 0 default
	resultSend[10] = 0x00;	// image format options
	resultSend[11] = 0x00;	// nothing
	
//	serial.printDevices();
//	serial.listDevices();
	
	_LEDPower = 0x04;
	_LEDSmoothing = 0xD0;
	_currentFrame = 0;

//	gui.addTitle("LED PROTOTYPE SETTINGS");
//	gui.addSlider("smoothing", _LEDSmoothing, 0x10, 0XFD);
//	gui.addSlider("global power", _LEDPower, 0x00, 0xFE);
//	gui.addToggle("SEND SERIAL", sendSerialData);
	
	
	_currentColour = nonoColor::HSB2RGB(_currentFrame,1,1);

	serial.setup();
	isConfigMode = false;
	if(isConfigMode)	openConfigConnection();
	else				openTestConnection();
	
}
	
	
//	char *myName = new char[50];
////	string ip = "195.71.11.67";
//	string ip = "192.168.1.6";
//	string ipSerialServer = "192.168.1.60";	
//	string hostname = "dosava.local";
////	string hostname = "monkmasks.local";
////	string hostname = "br0";
//	struct hostent *hp;
//    long    addr;	
//	
//	
//	IpEndpointName ien = IpEndpointName( hostname.c_str());
//	
//	gethostname(myName, 50);
//	addr = ien.address; //inet_addr(ip.c_str());
//	hp = gethostbyaddr((char *) &addr, sizeof(addr), AF_INET);
//	
//	if (hp != NULL) {
//		printf("Hostname:\t%s\n", hp->h_name);
//		printf("Aliases:\t");
//		while (hp->h_aliases[0])
//			printf("%s ", *hp->h_aliases++);
//		printf("\n");
//		printf("Addresses:\t");
//		while (hp->h_addr_list[0])
//			printf("%s ", inet_ntoa(*(struct in_addr *) * hp->h_addr_list++));
//		printf("\n");
//	}else{
//		printf("NOTHING FOUND\n");
//	}
//	
//	printf("MY HOST NAME : %s\n",myName);
//	
////	IpEndpointName ien = IpEndpointName(addr, 1233);//
////	ien.AddressAsString();
//
//	addr = inet_addr(ip.c_str());
//	printf("IP_ADDRESS         : %il\n",addr);
////	addr = inet_addr(ipSerialServer.c_str());
//	printf("IP_ADDRESS SERIAL  : %il\n",ien.address);
//	
//	ien = IpEndpointName(ien.address, 1233);//
//	ien.AddressAsString(myName);	
//	printf("IP : %s\n",myName);
//
//	ien = IpEndpointName('192.168.1.6', 1233);//
//	ien.AddressAsString(myName);	
//	printf("1 IP : %s\n",myName);
//	
//	char ipnew[] = "192.168.1.60";
//	long long_address = inet_addr (ipnew) ;
//	ien = IpEndpointName(long_address, 1233);//
//	ien.AddressAsString(myName);	
//	printf("2 IP : %s    -  %il / %il\n",myName,ien.address,long_address);
//	
//	int error_num;
//	char **p;
//	char abuf[INET6_ADDRSTRLEN];
//	struct  in_addr addrStruct;
//	addrStruct.s_addr = inet_addr(ipnew);
//	
//	
//	hp = getipnodebyname(ipnew, AF_INET6, AI_ALL | AI_ADDRCONFIG |
//						 AI_V4MAPPED, &error_num);
//	if (hp == NULL) {
//		if (error_num == TRY_AGAIN) {
//			printf("%s: unknown host or invalid literal address (try again later)\n", ipnew);
//				   } else {
//				   printf("%s: unknown host or invalid literal address\n",
//				   ipnew);
//		}
//	}else{
//		for (p = hp->h_addr_list; *p != 0; p++) {
//			struct in6_addr in6;
//			char **q;
//		
//			bcopy(*p, (caddr_t)&in6, hp->h_length);
//			(void) printf("   %s    %s \n", inet_ntop(AF_INET6, (void *)&in6,
//										abuf, sizeof(abuf)), hp->h_name);
//			
////		for (q = hp->h_aliases; *q != 0; q++)
////			(void) printf(" %s", *q);
//			(void) putchar('0');
//		}
//		freehostent(hp);
//	}
//	
	
	
	
	
	
//	printf(" NEW name : %s\n",hhh->h_name);
//	printf(" NEW name : %i\n",hhh->h_length);
//	printf(" NEW name : %s\n",hhh->h_aliases[0]);
//	printf(" NEW name : %i\n",hhh->h_addrtype);	
	
	
//	// print binary with leading zeros
//	int n = CSTOPB;
//	cout << "binary : ";
//	for (int i=31; i>=0; i--) {
//		int bit = ((n >> i) & 1);
//		cout << bit;
//	}
//	cout << endl;
//
////	
//	n = 51968;
//	n = 52992;
//	cout << "binary : ";
//	for (int i=31; i>=0; i--) {
//		int bit = ((n >> i) & 1);
//		cout << bit;
//	}
//	cout << endl;
//	
//	n = n & CSTOPB;
//
//	cout << "binary : ";
//	for (int i=31; i>=0; i--) {
//		int bit = ((n >> i) & 1);
//		cout << bit;
//	}
//	cout << endl;
//	
//	bool isOn = n > 0;
//	printf("CSTOPB is : %s\n",isOn?"ON":"OFF");

void testApp::exit(){
	serial.close();
	
	printf("ON EXIT\n");
}

void testApp::openTestConnection(){
	
	//	_isSerialConnected = serial.connect("/dev/tty.usbserial-DUTHNWQ5", B600);//115200);
	//	_isSerialConnected = serial.connect("/dev/tty.PL2303-003014FA", B115200);//115200);	
	//	_isSerialConnected = serial.connect("/dev/tty.PL2303-000013FD", B115200);	
	//	_isSerialConnected = serial.connect("/dev/tty.usbserial-FTB6APB5", B115200);
	
	//	_isSerialConnected = serial.connect("/dev/tty.usbserial-FTF4AMO2", B57600);	
	//	_isSerialConnected = serial.connect("/dev/tty.usbserial-DUTHNWQ5", B115200);
	
	//	serial.connect("/dev/tty.usbserial-DUTHNWQ5", 57600);//115200);
	
	serial.connectTestConnection();
//	serial.close();
//	_isSerialConnected = serial.setup("/dev/tty.usbserial-FTF4AMO2", B115200);
	
//	printf("openTestConnection : serial : %s\n", serial.getCurrentDeviceIDString().c_str());
}

void testApp::openConfigConnection(){
	if(serial.connectConfigConnection()){
		enterConfigTiles();
	}
//	serial.close();
//	_isSerialConnected = serial.setup("/dev/tty.usbserial-FTF4AMO2", B57600);
//	if(_isSerialConnected){
//		printf("openConfigConnection : %i \n",configStartLength);
//		enterConfigTiles();
//		printf("starting config mode\n");
//	}
}



void testApp::sendSerialFrame(){
	
	if(!serial.isConnected()) return;
	
//	printf(" update :: testMode : %i\n",(int)serial.isTestMode());
	
	int w = LED_WIDTH;
	int h = LED_HEIGHT;
	
//	int l = w * h * 3 + 12;
//	int bytesSend = w * h * 3 + 12 + 2;
//	
//	unsigned char sendBytes[bytesSend];// = new unsigned char[bytesSend];
//	
//	sendBytes[0] = 0xff;	// beginning of frame
//	sendBytes[1] = 0x0b;   //0x0b;	// length of message without the initial 0xff
//	sendBytes[2] = 0x00;	// length high byte, not in use
//	sendBytes[3] = 0x03;	// command byte:
//	//								0x01 = Send per-tile image data frame for one tile.
//	//								0x02 = Global Update.
//	//								0x03 = Send whole-frame image data.
//	//								0x90 = Display temperature
//	
//	sendBytes[4] = (int)w;//0x14;	// width low byte
//	sendBytes[5] = 0x00;	// width high byte
//	sendBytes[6] = (int)h;//0x10;	// height low byte
//	sendBytes[7] = 0x00;	// height high byte
//	sendBytes[8] = _LEDSmoothing;	// smoothing : 0 default
//	sendBytes[9] = _LEDPower;	// power : 0 default
//	sendBytes[10] = 0x00;	// image format options
//	sendBytes[11] = 0x00;	// nothing
	
	int bytesSend = w * h * 3 + 12;// + 2;
	
//	unsigned char sendBytes[bytesSend];// = new unsigned char[bytesSend];
	unsigned char* sendBytes = new unsigned char[bytesSend];
	
	
//	int r,g,b;
//	int cntClr = 0;
//	r = MIN((_currentColour>>16)&0xFF			,0xFE);
//	g = MIN((_currentColour>>8)&0xFF			,0xFE);
//	b = MIN((_currentColour>>0)&0xFF			,0xFE);	
//	for (int i = 0; i < bytesSend; i+=3) {
//		//		sendBytes[i] = resultSend[i%3];		
//		sendBytes[i] = r;	
//		sendBytes[i+1] = g;	
//		sendBytes[i+2] = b;	
//	}
	
	
	sendBytes[0] = 0xff;	// beginning of frame
	sendBytes[1] = 0x0b;   //0x0b;	// length of message without the initial 0xff
	sendBytes[2] = 0x00;	// length high byte, not in use
	sendBytes[3] = 0x03;	// command byte:
	//								0x01 = Send per-tile image data frame for one tile.
	//								0x02 = Global Update.
	//								0x03 = Send whole-frame image data.
	//								0x90 = Display temperature
	
	sendBytes[4] = (int)w;//0x14;	// width low byte
	sendBytes[5] = 0x00;	// width high byte
	sendBytes[6] = (int)h;//0x10;	// height low byte
	sendBytes[7] = 0x00;	// height high byte
	sendBytes[8] = _LEDSmoothing;	// smoothing : 0 default
	sendBytes[9] = _LEDPower;	// power : 0 default
	sendBytes[10] = 0x00;	// image format options
	sendBytes[11] = 0x00;	// nothing
	
	
	int r,g,b;
	int cntClr = 0;
	r = MIN((_currentColour>>16)&0xFF			,0xFE);
	g = MIN((_currentColour>>8)&0xFF			,0xFE);
	b = MIN((_currentColour>>0)&0xFF			,0xFE);
		
	for (int i = 12; i < bytesSend; i+=3) {
//		sendBytes[i] = resultSend[i%3];		
		sendBytes[i] = r;	
		sendBytes[i+1] = g;	
		sendBytes[i+2] = b;	
	}
	
	bool sendEntireFrame = true;
	
	int res = 0;
	int l1 = 512;
	int l2 = bytesSend - l1;
	writeBytesRaw(sendBytes,bytesSend);
	
//	writeBytes(sendBytes,bytesSend);
//	res = serial.sendData(sendBytes,bytesSend);
//	serial.sendData(sendBytes,bytesSend);
}


//--------------------------------------------------------------
	
void testApp::update(){
	
	if(serial.isTestMode()){
		
		_currentFrame++;
		
		imgOrg.setROI(0, 0, colorImg.getWidth(),colorImg.getHeight());
		imgOrg.setRoiFromPixels(colorImg.getPixels(), colorImg.getWidth(),colorImg.getHeight());
		imgOrg.setROI(imageRect);
		imgCrop.setFromPixels(imgOrg.getRoiPixels(), imageRect.width, imageRect.height);
		imgResized.resize(imageRect.width, imageRect.height);
		imgResized.setFromPixels(imgOrg.getRoiPixels(), imageRect.width, imageRect.height);
		imgResized.resize(LED_WIDTH, LED_HEIGHT);
		imgOrg.resetROI();
		
		resultImage = colorImg;
		resultImage.resize(LED_WIDTH,LED_HEIGHT);
		
		unsigned char* pixels = imgResized.getPixels();
		int l = LED_WIDTH*LED_HEIGHT*3;
		unsigned char pixel;
		int clr = nonoColor::HSB2RGB(_currentFrame,1,1);
		int offsetByte = 12;
		int r,g,b;
		int cntClr = 0;
		for (int i=0; i<l; i+=3) {
			r = MIN((clr>>16)&0xFF			,0xFE);
			g = MIN((clr>>8)&0xFF			,0xFE);
			b = MIN((clr>>0)&0xFF			,0xFE);
			
			cntClr ++;
//			r = cntClr%250;
//			g = cntClr%250;
//			b = cntClr%250;
			
//			if(cntClr %5 == 0){
//				r = 200;
//			}
//			if((int)(cntClr/LED_WIDTH)%3==0) g = 20;
			
			resultPixels[i]			= resultSend[i+offsetByte+0] = r;
			resultPixels[i+1]		= resultSend[i+offsetByte+1] = g;
			resultPixels[i+1]		= resultSend[i+offsetByte+2] = b;
			
		}
//		printf("   [ %X %X %X ]\n",r,g,b);
		_currentColour = clr;
// TODO		
		sendSerialFrame();
	}else{
		
		serial.read();
		
	}
	
}


void testApp::tempSendData(){

	
	unsigned char startData[] = {	0x0,0xFC,0xFF ,0xF6 ,0x0 ,0xFE ,0xFE ,0xFE ,0xFE ,0xFE ,0xFE ,0xFE ,0xFE ,0xFE ,0xFE ,0xFE ,0xFE ,0xFE, 0xFE ,0xFE ,0xFE ,0xFE ,0xFE ,0xFE ,0x0 ,0x0 ,0xFC ,0xFF ,0xF2 ,0x0 ,0x0 ,0xFA ,0xFF ,0xEA ,0xFC ,0xB2 ,0x0};
	

	unsigned char rotateData[] = {0x0 ,0xDF ,0xFF ,0xFA ,0xFC ,0x3E ,0xFE ,0xFE ,0xFE ,0xFE ,0xFE ,0xFE ,0xFE ,0xFE ,0xF6 ,0xFE ,0x78 ,0xFE ,0xFC ,0xFE ,0xFE ,0xFE ,0xFE ,0xFE ,0xF2 ,0x3E ,0x18 ,0xBE ,0xCC ,0xBE ,0x9C ,0xFE ,0xFE ,0x0 ,0x0 ,0xFA ,0xFF ,0xEA ,0xFC ,0xB2 ,0x0};
//	Received (33):
//	0 F3 FA FC 3E FE FE FE FE FE FE FE FE F2 FE 78 FE FC FE FE FE FE FE F2 3E 18 BE CC BE 9C FE FE 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
//	Received (7):
//	0 FA FF EA FC B2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
//	Received (41):
//	0 DF FF FA FC 3E FE FE FE FE FE FE FE FE FE FE 7C FE FC FE FE FE FE FE F2 3E 18 BE CC BE 9C FE FE 0 0 FA FF EA FC B2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
//	Received (5):
	unsigned char endData[] = {	0x0 ,0xFC ,0xFF ,0xF6 ,0x00 };
	
//	printf("size : %i\n",sizeof(startData));
//	printf("size : %i\n",sizeof(rotateData));
//	printf("size : %i\n",sizeof(endData));

	int waittime = 500;
	writeBytes(startData, (int)sizeof(startData));
	wait(&waittime);
	writeBytes(rotateData, (int)sizeof(rotateData));
	wait(&waittime);
	writeBytes(endData, (int)sizeof(endData));
//	int waittime = 500;
//	serial.sendData(startData, (int)sizeof(startData));
//	wait(&waittime);
//	serial.sendData(rotateData, (int)sizeof(rotateData));
//	wait(&waittime);
//	serial.sendData(endData, (int)sizeof(endData));
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(30, 30, 30);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	ofSetHexColor(0xffffff);
	//	if(useLiveVideo){
	ofPushMatrix();
	ofTranslate(20, 20, 0);
	double s = WIDTH_DISPLAY/WIDTH;
	ofScale(s, s, 1);
	ofFill();
	colorImg.draw(0,0);
	
	ofEnableAlphaBlending();
	glColor4f(0,0,0, .25);   
	
	int px = (imageRect.x+imageRect.width);
	int py = (imageRect.y+imageRect.height);
	ofRect(0,0,WIDTH,imageRect.y);
	ofRect(0,py,WIDTH,HEIGHT - py);
	ofRect(0,imageRect.y,imageRect.x,imageRect.height);
	ofRect(px,imageRect.y,WIDTH-px,imageRect.height);
	
	ofDisableAlphaBlending();
	ofNoFill();
	ofSetHexColor(0x909090);
	ofSetLineWidth(.5);
	
	ofRect(imageRect.x, imageRect.y, imageRect.width, imageRect.height);		
	ofPopMatrix();
	
	ofPushMatrix();
	ofTranslate(WIDTH_DISPLAY + 40, 20, 0);
	ofSetHexColor(0xffffff);
	imgCrop.draw(0, WIDTH_RES+10,100,100);
	imgResized.draw(110, WIDTH_RES+10,100,100);
	bitmapImage.draw(220, WIDTH_RES+10, 100, 100);
	ofPopMatrix();
	
	ofPushMatrix();
	ofPushStyle();
	ofTranslate(20, 20+480+20, 0);
	ofSetHexColor(0x666666);
	ofFill();
	ofRect(0, 0, WIDTH_DISPLAY, 14);
	ofSetHexColor(0xcccccc);
	ofRect(0, 0, WIDTH_DISPLAY * ((double)rectSizeWidth/(double)RECT_SIZE_MAX), 14);

	ofSetColor(230, 20, 20);
	char str[50];
	sprintf(str, "Size %i x %i   [%i,%i]",(int)imageRect.width,(int)imageRect.height,(int)imageRect.x,(int)imageRect.y);
	fontMono.drawString(str,10, 8);
	ofPopStyle();
	ofPopMatrix();
	
	//		vidGrabber.draw(20, 20,WIDTH*2,HEIGHT*2);
	//		colorImg.draw(WIDTH*2 + 40, 20);
	//	}
	ofPushMatrix();
	ofFill();
	ofSetLineWidth(0);
	ofTranslate(WIDTH_DISPLAY + 40, 20, 0);
	
	int l = resultImage.width * resultImage.height*3;
	int step = WIDTH_RES / LED_WIDTH;
	int cnt = 0;
	for (int i=0; i<l; i+=3) {
		ofSetColor((_currentColour>>16)&0xff, (_currentColour>>8)&0xff, _currentColour&0xff);
		ofRect((cnt%LED_WIDTH)*step, ((int)(cnt/LED_WIDTH))*step, step, step);
		cnt++;
	}
	
	
	
	ofPopMatrix();
//	if(gui.isOn()){
//		ofEnableAlphaBlending();
//		ofSetColor(0, 0, 0,150);
//		ofFill();
//		ofRect(0, 0, ofGetWidth(), ofGetHeight());
//		gui.draw();
//		ofDisableAlphaBlending();
//	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if(key == ' '){
//		isPaused = !isPaused;
//		sendSerialFrame();		
		isConfigMode = !isConfigMode;
		if(isConfigMode) openConfigConnection();
		else openTestConnection();
		
	}else if(key == OF_KEY_RETURN){
//		if(gui.isOn()) gui.hide();
//		else gui.show();

		
	}else if(key == 'e'){
		printf(" e tiles : %i\n",configEditCurrentLength);
		writeBytes(configEditCurrent,configEditCurrentLength);
		writeBytes(configStayAlive,configStayAliveLength);	
//		serial.sendData(configEditCurrent,configEditCurrentLength);
//		serial.sendData(configStayAlive,configStayAliveLength);		
	}else if(key == 'n'){
		printf(" n tiles : %i\n",configNextTileLength);
		editNextTile();
		
	}else if(key == 'x'){
		exitConfigTiles();
	}else if(key == 'c'){
		enterConfigTiles();		
	}else if(key == OF_KEY_DOWN){
		rotateCurrentTile();
	}else if(key == 's'){
		openConfigConnection();
		
	}else if(key == 't'){
		tempSendData();
	}
	
	
	
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------

void testApp::editNextTile(){
	
	writeBytes(configStayAlive,configStayAliveLength);	
	writeBytes(configNextTile,configNextTileLength);
	writeBytes(configStayAlive,configStayAliveLength);
	writeBytes(configEditCurrent,configEditCurrentLength);
//	serial.sendData(configStayAlive,configStayAliveLength);	
//	serial.sendData(configNextTile,configNextTileLength);
//	serial.sendData(configStayAlive,configStayAliveLength);
//	serial.sendData(configEditCurrent,configEditCurrentLength);
	configValuePositionXlow += 4;
	sendCurrentConfigTile();
}

void testApp::enterConfigTiles(){
	
	printf("\n  -- testApp::enterConfigTiles --\n");

	writeBytes(configExitConfig,configExitConfigLength);
	writeBytesRaw(configStart,configStartLength);
	writeBytes(configStayAlive,configStayAliveLength);		
	writeBytes(configEditCurrent,configEditCurrentLength);	
	//	serial.sendData(configTileSettings,configTileSettingsLength);
	//	serial.sendData(configNextTile,configNextTileLength);
//	writeBytes(configEditCurrent,configEditCurrentLength);
	
	
//	serial.sendData(configExitConfig,configExitConfigLength);
//	serial.sendData(configStart,configStartLength);
//	serial.sendData(configStayAlive,configStayAliveLength);		
////	serial.sendData(configTileSettings,configTileSettingsLength);
////	serial.sendData(configNextTile,configNextTileLength);
//	serial.sendData(configEditCurrent,configEditCurrentLength);
//	
	configValuePositionXlow = 0;
	configValuePositionXhigh = 0;
	configValuePositionYlow = 0;
	configValuePositionYhigh = 0;
}
	
void testApp::exitConfigTiles(){
	writeBytes(configExitConfig,configExitConfigLength);
//	serial.sendData(configExitConfig,configExitConfigLength);
}

void testApp::rotateCurrentTile(){
	configValuePositionRotation			= (configValuePositionRotation+1)%4;
	sendCurrentConfigTile();
}


void testApp::setCurrentConfigTile(){
	
	// xpos
	/*0*/	configTileSettings[5] = configValuePositionXlow&0xf0;
			configTileSettings[6] = configValuePositionXlow&0x0f;
	/*1*/	configTileSettings[7] = configValuePositionXhigh&0xf0;	
			configTileSettings[8] = configValuePositionXhigh&0x0f;	
	// ypos
	/*2*/	configTileSettings[9] = configValuePositionYlow&0xf0;
			configTileSettings[10] = configValuePositionYlow&0x0f;
	/*3*/	configTileSettings[11] = configValuePositionYhigh&0xf0;
			configTileSettings[12] = configValuePositionYhigh&0x0f;
	// orientation
	/*4*/	configTileSettings[13] = configValuePositionRotation&0xf0;
			configTileSettings[14] = configValuePositionRotation&0x0f;
	// default smoothing
	/*5*/	configTileSettings[15] = configValuePositionSmoothing&0xf0;
			configTileSettings[16] = configValuePositionSmoothing&0x0f;
	// default power
	/*6*/	configTileSettings[17] = configValuePositionPower&0xf0;
			configTileSettings[18] = configValuePositionPower&0x0f;
	// default position in chain -> not used
	/*7*/	configTileSettings[19] = configValuePositionPositionChainLow&0xf0;
			configTileSettings[20] = configValuePositionPositionChainLow&0x0f;
	/*8*/	configTileSettings[21] = configValuePositionPositionChainHigh&0xf0;	
			configTileSettings[22] = configValuePositionPositionChainHigh&0x0f;	
	// baudrate -> 2mbaud/(n+1)
	/*9*/	configTileSettings[23] = configValuePositionBaudrate&0xf0;
			configTileSettings[24] = configValuePositionBaudrate&0x0f;
	// power limit
	/*10*/	configTileSettings[25] = configValuePositionPowerLimit&0xf0;
			configTileSettings[26] = configValuePositionPowerLimit&0x0f;
	// temperature limit
	
	/*11*/	configTileSettings[27] = configValuePositionTemperatureLimit&0xf0;
			configTileSettings[28] = configValuePositionTemperatureLimit&0x0f;
	// blanking tmieout
	/*12*/	configTileSettings[29] = configValuePositionBlankingTimeout&0xf0;
			configTileSettings[30] = configValuePositionBlankingTimeout&0x0f;
	
}

void testApp::sendCurrentConfigTile(){	
	setCurrentConfigTile();
	printCurrentConfigTile();
	writeBytes(configStayAlive,configStayAliveLength);		
	writeBytes(configTileSettings,configTileSettingsLength);
//	serial.sendData(configStayAlive,configStayAliveLength);		
//	serial.sendData(configTileSettings,configTileSettingsLength);
}

void testApp::printCurrentConfigTile(){
	string propTitles[] = {"xl","xh","yl","yh","rot","smoothing","power","posInChain","posInChain","baudrate","powerlimit","temperature limit","blanking timeout"};
	printf(" CONFIG TILE : ");
	for (int i=0; i<configTileSettingsLength; i++) {
		int pos = (int)((i-4)/2);
		if(i > 3 && i%2==1) printf("\n  ||  %i (%18s)	:",pos,propTitles[pos].c_str());
		printf(" %2X ",	configTileSettings[i]);
	}
	printf("  <<");
}



void testApp::writeBytesRaw(unsigned char* data,int length){
	serial.sendBytes(data, length);
}

void testApp::writeBytes(unsigned char* data,int length){
	
	int newLength = length+3;
	unsigned char* dataConv = new unsigned char[newLength];
	dataConv[0] = 0xff;
	dataConv[1] = length+2;
	dataConv[2] = 0x00;
	for(int i=3;i<newLength;i++){
		dataConv[i] = data[i-3];
	}
	
	serial.sendBytes(dataConv, newLength);
}


//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	mouseMoved(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

