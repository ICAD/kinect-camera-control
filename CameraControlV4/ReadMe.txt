========================================================================
    CONSOLE APPLICATION : CameraControlV4 Project Overview
========================================================================

Hello User, 

	Okay so versions 2 and 3 only differ with less functionality and probably less reliability
of the actuall tracking, so I would reccomend using this version. That being said, if you do
not want to zoom functionality then use Version 3.

So what you need is a Cannon VC-C4 camera and a kinect, my files, and the drivers for each installed.

After that you can just run the exe and it will work, although not perferctly, if this project
goes any further then I will put more effort into making this a non coders dream till then
roll up your sleeves and get ready to do some coding.

Okay so to initilize the camera what your going to want to do is comment out any code relatiing
to the Cannon Camera class in the cameracontrolV4 code. Set up the cannon and the kinect so that 
the kinect is on top of the cannon and they are centered horzontaly. 

Un comment this line 	//cameraCalibrated=calibrateCameraLocation(user, g_UserGenerator);
start up whatever viewer you are using to view the canon feed. Compile and run my program.

Get your skeleton calibrated AND NOTE!!!! do not let your arms cross your chest!!! 

Okay your set, now position your left hand so that the tips of fingers can not be seen in the 
top left corner,keeping that hand there move your right rand so it is past the middle of your 
body. You should see a bunch of lines spew onto the console and then stop. 

Next step move your right hand to the bottom right corner of the screen you may have to get on your 
knees SORRY. Swipe left hand across your body. 

you can repeat this a few times if you want more accurate numbers, once your done. open the locations 
text file in the directory and then find the horizontal and vertical center (Left X + right X)/2

enter these numbers into 	camera.initilize(306,260); this line, instead of 306,206.

ALMOST THERE!! un comment the camera lines, recomment the calibrate line and your done! recompile and run

SEE I told you a little bit of work but this project is not supposed to be commercial just prrof of concept

ENJOY

Sincerly
RATTLE RIV

AKA
Wyatt Rivers