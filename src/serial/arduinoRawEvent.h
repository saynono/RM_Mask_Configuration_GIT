/*
 *  arduinoRawEvent.h
 *  futbolin09cpp
 *
 *  Created by Vincent R. on 13.11.09.
 *  Copyright 2009 www.say-nono.com. All rights reserved.
 *
 */


#ifndef ARDUINORAW_H_
#define ARDUINORAW_H_

#include "ofMain.h"
#include <map>



class arduinoRawEventArgs: public ofEventArgs{
public:
	int 	port;
	string	msg;
};

typedef ofEvent <arduinoRawEventArgs> arduinoRawEvent ;


class arduinoRawListener {
public:
	arduinoRawListener(){}
	virtual ~arduinoRawListener(){}

	virtual void newArduinoRawMessage(arduinoRawEventArgs& eventArgs)=0;

};


#endif /* ARDUINORAW_H_ */
