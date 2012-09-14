#include "stdafx.h"
#include "CameraState.h"
#include "VCC4.h"
#include <math.h>
// this is just for token sake
float zoomLoc;
int TAMPAX=1;
CannonState::CannonState()
{
	
	zoomState zoom; 
	
	BOOL ok;
	idle=true;
	 panningLeft=false;
	 panningRight=false;
	 checkedAngle=false;
	 toMoveToExactLocation=false;
	 move=false;
	 ratio=1.2;
		 tilt=0x8070;
		 nextLocation=-1;
		 	ok=V4Open(a, 1L, 9600L,8L, 0L,0L);
	ok=ok && V4SetCmdMode(0L,0L);
    ok=ok && V4SetNotify(0,1);
	commandWait(2);
	ok=ok && V4SetCameraPower(0L,1L);
	commandWait(4);
	ok=ok && V4SetMotorSpeed(0L,0L,150L);
	commandWait(5);
	ok=ok && V4PedestalInitialize(0,0);
	commandWait(7);
	commandNumber=7;
	printMessages();

	
	
}
void CannonState::initilize(int centerX,int centerY)
{

	centerPixelX=centerX;
	centerPixelY=centerY;
	//V4GotoHomePosition(0);
	//commandNumber++;
	//commandNumber++;
	//currentCenter=currentLocation;
	
	//commandNumber=6;


}
int CannonState::printMessages()
{
	int x=1,y,z;
	for (x=1;(y=V4GetMsg(x, recivedMessage))!=0;x++ )
	{
	
		printf("%d: ",x);
		for (z=0;z<y;z++)
		{
			printf("%x " ,recivedMessage [z]);
		}
		printf("\n");
	}
	printf("C : %d \n",commandNumber);
	return x;
}
void CannonState::updateAll(int currentLocation,int yLocation,float zLocation,int state)
{
	static int count=0;
	static int zoomIndicator=0;
	int numBytes;
	zoomLoc=zLocation;

	if (zoom.getLocation()==90)
	{
		//yLocation-=20;
	}
	if (checkStatus())
		{//ready

		if (state==1)
		{
			PanMove(currentLocation,yLocation);
			count=0;
			nextLocation=-1;
			nextYLocation=-1;
			zoomIndicator=0;
		}
		else if (nextLocation!=-1)
		{
			PanMove(nextLocation,nextYLocation);
			nextLocation=-1;
			nextYLocation=-1;
			zoomIndicator=0;
		}
		else if (count>30 && zoomIndicator==1)
		{
			PanMove(currentLocation,yLocation);
			zoomIndicator=2;
		}
		else if (state==0)
		{
			count++;
			if (count>30 && zoomIndicator==0)
			{
				zoomUpdate();
				zoomIndicator=1;
			}
		}
	}
	else
	{
		if (state==1)
		{
			nextLocation=currentLocation;
			nextYLocation=yLocation;
			count=0;
		}
		else
		{
			count++;
		}	
	}
	
	

	
}
bool CannonState::checkStatus()
{
		int x,y,z;
		static bool p=false;
	for (x=TAMPAX;(y=V4GetMsg(x, recivedMessage))!=0;x++ )
	{
	;
	}
	TAMPAX=x-1;
	
	return (commandNumber==TAMPAX);
}
void CannonState::zoomUpdate () {
		bool change;
		change=zoom.updatePosition(zoomLoc);
		if (change==true)
		{
			V4SetZoomPosition(0,zoom.getLocation());
			commandNumber++;
			commandNumber++;
			//printMessages();
		}
}
void CannonState::watchdog(int location,int ylocation)
{
		int i=1,j=1;
		printf("WatchDog Initiated\n");
		PanMove(location,ylocation);
		idle=true;
		
		nextLocation=-1;
			int x=1,y,z;
	for (x=1;(y=V4GetMsg(x, recivedMessage))!=0;x++ )
	{
	
		printf("%d: ",x);
		for (z=0;z<y;z++)
		{
			printf("%x " ,recivedMessage [z]);
		}
		printf("\n");
	}
	commandNumber=x;
		//watchDogCounter=0;
}
void CannonState::PanMove(int location,int ylocation)
{

	pan= pixelToAngleConversionX(location);
	tilt= pixelToAngleConversionY(ylocation);
	V4SetPTAngle2(0,pan,tilt);
	commandNumber++;
	commandNumber++;
	idle=false;
}

void CannonState::checkAngle (void)
{
	V4GetPTAngle2(0);
	commandNumber++;
	checkedAngle=true;
}

int CannonState::pixelToAngleConversionX (int pixel)
{
	int ret;
	ret=floor((centerPixelX-pixel)/ratio);
	ret+=0x8000;
	return ret;
}
int CannonState::pixelToAngleConversionY (int pixel)
{
	int ret;
	ret=floor((centerPixelY-pixel)/ratio);
	ret+=0x8000;
	return ret;
}
int CannonState::messageToAngleConversion (void)
{
	int angle=0;
	for (int g=0;g<4;g++)
		{
			if (recivedMessage [5+g]>0x40)
			{
				angle+=(recivedMessage[5+g]-0x37)*pow(16.0,3.0-g);
			}
			else
			{
				angle+=(recivedMessage[5+g]-0x30)*pow(16.0,3.0-g);
			}
				
		}
	return angle;
}
void CannonState::commandWait(int Number)
{
	BYTE  bb[20];
	int i;
	for (;;)
	{
		i=V4GetMsg(Number,bb);
		if (i>0)
			break;
	
	}
	printf("Done");
}
		
	