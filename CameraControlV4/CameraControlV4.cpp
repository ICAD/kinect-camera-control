// CameraControl.V2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "glut.h"
#include <XnOpenNI.h>
#include <XnList.h>
#include <XnCppWrapper.h>
#include <XnCodecIDs.h>
#include <math.h>
#include "CameraState.h"

#define SAMPLE_XML_PATH "CameraControl.xml"



xn::Context g_Context;
XnUserID ActiveUser=1;
xn::DepthGenerator g_DepthGenerator;
xn::UserGenerator g_UserGenerator;
xn::SceneAnalyzer g_SceneAnalyzer;
XnBool cameraCalibrated=false;
XnBool	g_bCalibrate1 = false, g_bCalibrate2 = false;
XnBool g_bShowBG = false;
XnBool g_IsSkeletonEnabled = true;
XnBool drawB=false;
XnPoint3D topLeft,bottomRight;
CannonState camera;
float neckX=341,neckY=263;
float deltaXMove=0,deltaYMove;

float centerX=341.0,oldNeck=0;
int cx=375;
XnSkeletonJoint getJoint(int a);
FILE * pFile;
void DrawBones(XnUserID user, xn::UserGenerator g_UserGenerator );

void initScene() {

	glEnable(GL_DEPTH_TEST);


}



void changeSize(int w, int h)
	{
     float ratio;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	ratio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the clipping volume
	gluPerspective(65,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 0.0f, 
		       0.0f, 0.0f, -1.0f,
			  0.0f,1.0f,0.0f);


	}


XnBool calibrateCameraLocation(XnUserID user,xn::UserGenerator g_UserGenerator)
{
	//Declarations
	static int count=0,count2=0;
	XnBool ret=false;
	XnSkeletonJointPosition pos;
	XnPoint3D lHand,rHand,torso;
	static double lXAvg,lYAvg,rXAvg,rYAvg,sumX=0,sumY=0;
	
	//Init of points
	g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(user, XN_SKEL_LEFT_HAND, pos);
	lHand=pos.position;
	g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(user, XN_SKEL_RIGHT_HAND, pos);
	rHand=pos.position;
	g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(user, XN_SKEL_TORSO, pos);
	torso=pos.position;
	count2++;
	//if left hand is not set set it otherwise check the right if the after right is set change calibrated boolean
	if (topLeft.X==-1)
	{
		
		if (torso.X>rHand.X)
		{
			topLeft=lHand;
			g_DepthGenerator.ConvertRealWorldToProjective(1, &lHand, &lHand);
			//printf("Left %f %f Top:%f %f \n",lHand.X,lHand.Y,topLeft.X,topLeft.Y);
			
			sumX+=lHand.X;
			sumY+=lHand.Y;
			count++;
			if (count<30)
				 topLeft.X=-1;
				
			else 
			{ 
				lXAvg=sumX/count;
				lYAvg=sumY/count;
				printf("Done Left Calibration");
				count=0;
				sumX=0;
				sumY=0;
			}
				
		}
		
	}
	else if (bottomRight.X==-1)
	{
		if (torso.X<lHand.X)
		{
			bottomRight=rHand;
		    g_DepthGenerator.ConvertRealWorldToProjective(1, &rHand, &rHand);
			//printf("Right: %f %f Bottom %f %f \n",rHand.X,rHand.Y,bottomRight.X,bottomRight.Y);
			//
			
			//topLeft.X=-1;
		//	if (count==5)
			
			if (count<30){
				sumX+=rHand.X;
			sumY+=rHand.Y;
				 bottomRight.X=-1;
			}

			count++;
		}
		if (topLeft.X!=-1 && bottomRight.X!=-1){
				rXAvg=sumX/count;
				rYAvg=sumY/count;
				
			
			topLeft.X=-1;
			bottomRight.X=-1;
			count=0;
			camera.initilize((int)(rXAvg+lXAvg)/2,(int)(rYAvg+lYAvg)/2);
			printf("Done Calibration");
			 pFile = fopen ("locations.txt","w");
			fprintf (pFile,"%d %d",(int)(rXAvg+lXAvg)/2,(int)(rYAvg+lYAvg)/2);
			fclose(pFile);
			ret=true;
		}
		
	}
	
	return ret;
}


void drawJoint(XnUserID user, xn::UserGenerator g_UserGenerator,XnSkeletonJoint eJoint)
{
	static int i=0;
	//camera.printMessages();
	XnStatus rc = XN_STATUS_OK;
	XnSkeletonJointPosition p,p2;
	XnPoint3D point,point2;
	int joint=eJoint;
	int state=0;
	float X,Y;

		
		glPushMatrix();
		g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(user, eJoint, p);
		g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(user,XN_SKEL_HEAD	, p2);
		point2=p2.position;
		point=p.position;
		g_DepthGenerator.ConvertRealWorldToProjective(1, &point, &point);
		g_DepthGenerator.ConvertRealWorldToProjective(1, &point2, &point2);
		if (eJoint==15 || eJoint==9)
		{
			if (point.Y<point2.Y)
				{
					//printf("User has changed %d to %d\n",ActiveUser,user);
					ActiveUser=user;
				}
				
		}
		//Sees that it is the neck and begins the movment check
	if (eJoint==2 && user==ActiveUser ){
		
		
			//Averaging out the head and the neck for a better point to follow on the Cannon
			
			deltaXMove=point.X-neckX;
			deltaYMove=point.Y-neckY;
			// Check to see if the last Static postion has moved more then 20 pixels in the left right plane
			if (deltaXMove>20 || deltaXMove < -20)
			{
				
				//printf("x\n");
			neckX=point.X;
			i=0;
			state=1;
		//Yes it has so state is 1 for moved and neckX is changed
			}
		
		
				
		if (deltaYMove>20 || deltaYMove < -20)
			{
				// see above but this is the Y movement
			
			neckY=point.Y;
			i=0;
			state=1;
		
			}
		
		
			//sends the relative data to the camera class to do its thing
		 if(cameraCalibrated==true)
			camera.updateAll(point2.X,point2.Y,point2.Z,state);
			
			}
				
			
	
	//This is just drawing the joints on the screen
				
		X=(point.X-320)/320; 
		Y=-1*(point.Y-240)/240;
	glPushMatrix();
	glTranslatef(X, Y,0);
	if (eJoint==1){
		glutSolidSphere(0.05f,20,20);
	}
	
	glutSolidSphere(.05f,20,20);
	glPopMatrix();
	if (eJoint==2 &&  cameraCalibrated==false)
	{
		 
		//cameraCalibrated && to be used for calibration
		cameraCalibrated=calibrateCameraLocation(user, g_UserGenerator);

	}
	
}

void DrawSingleUser(XnUserID user, xn::UserGenerator g_UserGenerator)
{
	static int i=0;
	i++;
	 int count=0;
	 XnSkeletonJoint j;

		for (count=1;count<25;count++)
		{
			
			glColor3f(count/24.0f, count/48.0f, 1.0f);
			j=getJoint(count);
			if (!g_UserGenerator.GetSkeletonCap().IsJointAvailable(j))continue;
			drawJoint(user,g_UserGenerator,j);
		}
		
	}
void draw_stickfigure()
{
	
	float fNewColor = 0;
	float fMaxDepth;
	xn::DepthMetaData dm;
	g_DepthGenerator.GetMetaData(dm);
	XnPoint3D corner = xnCreatePoint3D(dm.XRes(), dm.YRes(), dm.ZRes());
	XnUserID users[10];
	XnUInt16 nUsers = 10;
	glPushMatrix();
	g_UserGenerator.GetUsers(users, nUsers);
	for (int i = 0; i < nUsers; ++i)
	{
		
		if (g_UserGenerator.GetSkeletonCap().IsTracking(users[i]))
		{
			DrawSingleUser(users[i], g_UserGenerator);
			if(drawB)
			DrawBones(users[i],g_UserGenerator);
		
		
		}
	}

		glPopMatrix();
		

	
}
DWORD WINAPI EEThreadProc(LPVOID lpThreadParameter)
{
	{
		g_Context.WaitAndUpdateAll();
	
	}
	return 0;
}


void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	EEThreadProc(NULL);
		glBegin(GL_QUADS);
		glVertex3f(-10.0f,-0.01f, -1.0f);
		glVertex3f(-10.0f,-0.01f,  -1.0f);
		glVertex3f( 10.0f, 0.01f,  -1.0f);
		glVertex3f( 10.0f, 0.01f, -1.0f);
		glEnd();

	glBegin(GL_QUADS);
		glVertex3f( 0.01,-10.0f, -1.0f);
		glVertex3f(0.01f,-10.0f,  -1.0f);
		glVertex3f( 0.01f,10.0f,   -1.0f);
		glVertex3f( 0.01f,10.0f,  -1.0f);
		glEnd();
	draw_stickfigure();
	glutSwapBuffers();
}

void XN_CALLBACK_TYPE NewUser(xn::UserGenerator& generator, XnUserID user, void* pCookie)
{
	printf("New user identified: %d\n", user);
	g_UserGenerator.GetPoseDetectionCap().StartPoseDetection("Psi", user);
}

void XN_CALLBACK_TYPE LostUser(xn::UserGenerator& generator, XnUserID user, void* pCookie)
{
	printf("User %d lost\n", user);
}
void XN_CALLBACK_TYPE CalibrationStart(xn::SkeletonCapability& skeleton, XnUserID user, void* pCookie)
{
	printf("Calibration start for user %d\n", user);
}
void XN_CALLBACK_TYPE CalibrationEnd(xn::SkeletonCapability& skeleton, XnUserID user, XnBool bSuccess, void* pCookie)
{
	printf("Calibration complete for user %d: %s\n", user, bSuccess?"Success":"Failure");
	if (bSuccess)
	{
		skeleton.StartTracking(user);
	}
	else
	{
		g_UserGenerator.GetPoseDetectionCap().StartPoseDetection("Psi", user);
	}
}
void XN_CALLBACK_TYPE PoseDetected(xn::PoseDetectionCapability& poseDetection, const XnChar* strPose, XnUserID nId, void* pCookie)
{
	printf("Pose '%s' detected for user %d\n", strPose, nId);
	g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, FALSE);
	g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
}
#define CHECK_RC(rc, what)											\
	if (rc != XN_STATUS_OK)											\
	{																\
		printf("%s failed: %s\n", what, xnGetStatusString(rc));		\
		return rc;													\
	}
int main(int argc,  char **argv)
{
	int mposx,mposy;
	char ans;
	
	 pFile = fopen ("locations.txt","r");
	 if (pfile)
	 fscanf(pFile,"%d %d",&mposx,&mposy);
	 printf("%d %d",mposx,mposy);
	 fclose(pFile);
	 printf("Do you need to calibrate (Help ON calibration see the read me) (y or n)");
	 scanf("%c",&ans);
	 if (ans=='y')
		 cameraCalibrated=false;
	 else
	 {
		 camera.initilize(mposx,mposy);
		 cameraCalibrated=true;
	 }
	XnStatus rc = XN_STATUS_OK;
	 topLeft.X=-1;
	 bottomRight.X=-1;
	rc = g_Context.InitFromXmlFile(SAMPLE_XML_PATH);
	CHECK_RC(rc, "InitFromXml");

	rc = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
	CHECK_RC(rc, "Find depth generator");
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_USER, g_UserGenerator);
	CHECK_RC(rc, "Find user generator");

	XnCallbackHandle h;

	g_UserGenerator.RegisterUserCallbacks(NewUser, LostUser, NULL, h);
	g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

	XnCallbackHandle hCalib;
	XnCallbackHandle hPose;
	g_UserGenerator.GetSkeletonCap().RegisterCalibrationCallbacks(&CalibrationStart, &CalibrationEnd, NULL, hCalib);
	g_UserGenerator.GetPoseDetectionCap().RegisterToPoseCallbacks(&PoseDetected, NULL, NULL, hPose);

	rc = g_Context.StartGeneratingAll();
	CHECK_RC(rc, "StartGenerating");

	xn::DepthMetaData depthMD;
	g_DepthGenerator.GetMetaData(depthMD);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,480);
	glutCreateWindow("Wyatt-Skeleton Tracking");

	initScene();



	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	//camera.printMessages();


	//glutReshapeFunc(changeSize);
	//camera.printMessages();
	//camera.printMessages();
	glutMainLoop();

	return(0);
}
XnSkeletonJoint getJoint(int a)
{

	switch (a){
	case 1:
		return XN_SKEL_HEAD;
			break;
	case 2:
		return XN_SKEL_NECK	;
			break;
			
	case 3:
		return XN_SKEL_TORSO;	
		break;
	case 4:
		return XN_SKEL_WAIST;	
		break;
	case 5:
		return XN_SKEL_LEFT_COLLAR;	
			break;
	case 6:
		return XN_SKEL_LEFT_SHOULDER;	
			break;
	case 7:
		return XN_SKEL_LEFT_ELBOW	;
			break;
	case 8:	
		return XN_SKEL_LEFT_WRIST	;	
			break;
	case 9:
		return XN_SKEL_LEFT_HAND	;	
			break;
	case 10:
		return XN_SKEL_LEFT_FINGERTIP	;
			break;
	case 11:
		return XN_SKEL_RIGHT_COLLAR	;
			break;
	case 12:
		return XN_SKEL_RIGHT_SHOULDER	;
			break;
	case 13:
		return XN_SKEL_RIGHT_ELBOW	;	
			break;
	case 14:
		return XN_SKEL_RIGHT_WRIST	;	
			break;
	case 15:
		return XN_SKEL_RIGHT_HAND	;
			break;
	case 16:
		return XN_SKEL_RIGHT_FINGERTIP	;
			break;
	case 17:
		return XN_SKEL_LEFT_HIP	;
			break;
	case 18:	
		return XN_SKEL_LEFT_KNEE	;
			break;
	case 19:
		return XN_SKEL_LEFT_ANKLE	;	
			break;
	case 20:
		return XN_SKEL_LEFT_FOOT	;
			break;
	case 21:
		return XN_SKEL_RIGHT_HIP	;
			break;
	case 22:	
		return XN_SKEL_RIGHT_KNEE	;
			break;
	case 23:
		return XN_SKEL_RIGHT_ANKLE	;
			break;
	case 24:
		return XN_SKEL_RIGHT_FOOT	;
			break;
	}
}

void drawStickPoint(XnPoint3D point)								
{	g_DepthGenerator.ConvertRealWorldToProjective(1, &point, &point);																	
	float fX = ((point.X-320)/ 320);								
	float fY = -((point.Y-240) /240);								
	float fU = fX + 0.5;												
	float fV = fY + 0.5;												
	glVertex3f(fX, fY, 0.0f);	
}
void drawStickLine(XnUserID user, xn::UserGenerator userGenerator, XnSkeletonJoint joint1, XnSkeletonJoint joint2)
{
	XnSkeletonJointPosition pos1, pos2;
	userGenerator.GetSkeletonCap().GetSkeletonJointPosition(user, joint1, pos1);
	userGenerator.GetSkeletonCap().GetSkeletonJointPosition(user, joint2, pos2);
	if (pos1.fConfidence == 1 &&
		pos2.fConfidence == 1)
	{
		glColor3f(1.0f,0.5f,0.3f);
	
	}
	else
	{
		glColor3f(0.5,0.5,0.5);

		if ((pos1.position.X == 0 && pos1.position.Y == 0 && pos1.position.Z == 0) ||
			(pos2.position.X == 0 && pos2.position.Y == 0 && pos2.position.Z == 0))
		{
			return;
		}
	}
		drawStickPoint(pos1.position);
	drawStickPoint(pos2.position);
		
}
void DrawBones(XnUserID user, xn::UserGenerator g_UserGenerator ){
	glPushMatrix();
		glLineWidth(3.0);
	glBegin(GL_LINES);
	drawStickLine(user, g_UserGenerator, XN_SKEL_LEFT_HAND, XN_SKEL_LEFT_ELBOW );
 	drawStickLine(user, g_UserGenerator, XN_SKEL_LEFT_ELBOW, XN_SKEL_LEFT_SHOULDER);
 	drawStickLine(user, g_UserGenerator, XN_SKEL_LEFT_SHOULDER, XN_SKEL_TORSO);
 	drawStickLine(user, g_UserGenerator, XN_SKEL_LEFT_SHOULDER, XN_SKEL_RIGHT_SHOULDER);
 	drawStickLine(user, g_UserGenerator, XN_SKEL_TORSO, XN_SKEL_RIGHT_SHOULDER);
 	drawStickLine(user, g_UserGenerator, XN_SKEL_RIGHT_SHOULDER, XN_SKEL_RIGHT_ELBOW);
 	drawStickLine(user, g_UserGenerator, XN_SKEL_RIGHT_ELBOW, XN_SKEL_RIGHT_HAND);
 	drawStickLine(user, g_UserGenerator, XN_SKEL_NECK, XN_SKEL_HEAD);
 
	drawStickLine(user, g_UserGenerator, XN_SKEL_TORSO, XN_SKEL_LEFT_HIP);
	drawStickLine(user, g_UserGenerator, XN_SKEL_TORSO, XN_SKEL_RIGHT_HIP);

	drawStickLine(user, g_UserGenerator, XN_SKEL_LEFT_HIP, XN_SKEL_RIGHT_HIP);

	drawStickLine(user, g_UserGenerator, XN_SKEL_LEFT_HIP, XN_SKEL_LEFT_KNEE);
	drawStickLine(user, g_UserGenerator, XN_SKEL_LEFT_KNEE, XN_SKEL_LEFT_FOOT);
	drawStickLine(user, g_UserGenerator, XN_SKEL_RIGHT_HIP, XN_SKEL_RIGHT_KNEE);
	drawStickLine(user, g_UserGenerator, XN_SKEL_RIGHT_KNEE, XN_SKEL_RIGHT_FOOT);
	glEnd();
	glPopMatrix();
}