#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
//#include "ofxFileDialog.h"
#include "arduinoRaw.h"
//#include "ofxSimpleGuiToo.h"
#include "ofxOpenCv.h"
#include "nonoColor.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

#include "SerialManager.h"


class testApp : public ofBaseApp{

	public:
		void setup();
		void exit();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		ofTrueTypeFont fontMono;
	
	
private:
	
		int				_currentColour;
		float			_currentFrame;
		bool			sendSerialData;
		bool			isPaused;
		bool			isConfigMode;
		bool			_isSerialConnected;
		int				mouseOverID;
		int				mouseDraggingID;
		int				displayModeID;
		int				_LEDPower;
		int				_LEDSmoothing;
		int				rectSizeWidth;
		int				rectSizeHeight;
		ofPoint			dragOffset;
	
		ofRectangle		imageRect;
		ofRectangle		imageRectOffset;	
		ofxCvColorImage imgOrg,imgCrop,imgResized;

		/*
		0xFF 0xC8 0x00 0x01 <SS> <PP> 0x00 0x00 0x00  <8*8*3 bytes of data  RGBRGB...>
		Note 0xFF is used for framing, so the rest of the data MUST NOT contain 0xFF. 
		RGB and other data values must be clipped to 0xFE
		
		SS is the smoothing value - values around 0xD0 to 0XFD are useful
		PP is the global power value - 0XFE is max
		*/
		
		unsigned char*	resultPixels;
		unsigned char*	resultSend;
	
		unsigned char*	configStart;
		unsigned char*	configEditCurrent;
		unsigned char*	configNextTile;
		unsigned char*	configTileSettings;	
		unsigned char*	configExitConfig;	
		unsigned char*	configStayAlive;
	

		int				configStartLength;
		int				configEditCurrentLength;
		int				configNextTileLength;
		int				configTileSettingsLength;	
		int				configExitConfigLength;	
		int				configStayAliveLength;
	
	
		int				configValuePositionXlow;
		int				configValuePositionXhigh;
		int				configValuePositionYlow;
		int				configValuePositionYhigh;
		int				configValuePositionRotation;
		int				configValuePositionSmoothing;
		int				configValuePositionPower;
		int				configValuePositionPositionChainLow;
		int				configValuePositionPositionChainHigh;
		int				configValuePositionBaudrate;
		int				configValuePositionPowerLimit;
		int				configValuePositionTemperatureLimit;
		int				configValuePositionBlankingTimeout;
	
	
		ofImage			previewImage;
		ofImage			bitmapImage;
		ofImage			movieImage;
		ofImage			movieImg;
		ofImage			colorImg;
		ofImage			resultImage;
	
		int				tilesPosition;
	
//		int			nBytesRead;	
	
//		ofSerial		serial;
		SerialManager	serial;
		
		void			sendSerialFrame();
		void			openTestConnection();
		void			openConfigConnection();
	
	
		void			editNextTile();
		void			enterConfigTiles();
		void			exitConfigTiles();
		void			rotateCurrentTile();
		void			setCurrentConfigTile();
		void			sendCurrentConfigTile();
		void			printCurrentConfigTile();
	
		void			tempSendData();
	
		void			writeBytes(unsigned char* data,int length);
		void			writeBytesRaw(unsigned char* data,int length);
	
	
};

#endif
