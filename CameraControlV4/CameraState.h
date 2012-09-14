
#include <stdio.h>
class zoomState
{
private:
	int zoomLocation;
	int findLocation(float loc)
	{
		int retVal;
		if (loc>3200) // the reason for the ors here is that if the person 
			retVal=90;
		else if (loc>2600 )
			retVal=60;
		else if (loc > 1600 )
			retVal=30;
		else
			retVal=0;
		return retVal;
	}

public:
	zoomState()
	{
		zoomLocation=0;
	}
	bool updatePosition(float loc)
	{
		int newZP;
		newZP=findLocation(loc);
		if (newZP!=zoomLocation)
		{
			zoomLocation=newZP;
			return true;
		}
		else
			return false;
	}
	int getLocation()
	{
		return zoomLocation;
	}

};
class CannonState
{
private:
	//States
	BOOL idle;
	BOOL panningLeft;
	BOOL panningRight;
	BOOL checkedAngle;
	BOOL toMoveToExactLocation;
	BOOL move;
	HWND a; 
	zoomState zoom;
	int pan;
	int nextLocation;
	int nextYLocation;
	int tilt;
	double ratio;
	int watchDogCounter;
	//center of screen at home posistion
	int centerPixelX;
	int centerPixelY;
	int currentCenter;
	int currentAngle;
	//Number of commands issued
	int commandNumber;

	//byte to recieve message
	BYTE recivedMessage [20];

	void PanMove(int location,int);
	bool checkStatus();
	int messageToAngleConversion(void);
	void checkAngle(void);
	int pixelToAngleConversionX (int pixel);
	int pixelToAngleConversionY (int pixel);
	void watchDogFA();

public:
	CannonState();
	void initilize(int center,int);
	void updateAll(int currentLocation,int,float,int state); //state will be 0= not moving,1=moved left, 2=moved right
	int printMessages();
	void watchdog(int location,int ylocation);
	void zoomUpdate();
	void commandWait(int num);


};


	
