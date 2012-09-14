extern "C" {

#ifdef VCC4_EXPORTS
#define VCC4_API __declspec(dllexport)
#else
#define VCC4_API __declspec(dllimport)
#endif

//#define  _stdcall

#define VCC4_NORMALEND  0
#define VCC4_SYSTEM_ERR  -1
#define VCC4_OPEN_ERR  -2
#define VCC4_ALREADY_OPEN_ERR -3  
#define VCC4_PARAM_ERR  0x8000
#define VCC4_UNIT_ERR   0xa000

 
VCC4_API BOOL __stdcall  V4Open( HWND hWnd, DWORD prPort, DWORD prSpeed, 
								  DWORD prBytesize, DWORD prParity, DWORD prStopbit );
VCC4_API BOOL _stdcall   V4Close( void );
VCC4_API LPSTR __stdcall  V4GetCmdstr( int v4cmdno, int unitno, int *cmdlen  );
VCC4_API BOOL __stdcall   V4SendCmd( int v4cmdno, int unitno );
VCC4_API int __stdcall  V4GetMsg( int msgno, BYTE *RcvMsg );
VCC4_API int __stdcall  V4GetLastCmdstr( BYTE *RcvCmd );
VCC4_API BOOL __stdcall   V4SetMotorSpeed( int unit, int iMode , int iSpeed );
VCC4_API BOOL __stdcall   V4GetMotorSpeed( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetPanTilt1( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4GotoHomePosition( int unit );
VCC4_API BOOL __stdcall   V4PedestalInitialize( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4GetLimitSpeed( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4GetLimitAngle( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4GetGearRatio( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetPanTilt2( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetPTAngle2( int unit, int Angle1, int Angle2 );
VCC4_API BOOL __stdcall   V4GetPTAngle2( int unit );
VCC4_API BOOL __stdcall   V4SetPTAngle1( int unit, int iMode , int Angle1, int Angle2  );
VCC4_API BOOL __stdcall   V4GetPTAngle1( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetIRRemote( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4GetExtendedOperateStatus( int unit );
VCC4_API BOOL __stdcall   V4GetOperateStatus( int unit );
VCC4_API BOOL __stdcall   V4GetUnitName( int unit );
VCC4_API BOOL __stdcall   V4GetRomVersion( int unit );
VCC4_API BOOL __stdcall   V4SetPresetMemory( int unit, int Memno );
VCC4_API BOOL __stdcall   V4GoToPreset( int unit, int Memno );
VCC4_API BOOL __stdcall   V4GetExtendPreset( int unit );
VCC4_API BOOL __stdcall   V4GetPresetStatus( int unit );
VCC4_API BOOL __stdcall   V4RemoteCommandPass( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetLed( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetCascade( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetCmdMode( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetOnScreen( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetTitleString( int unit, int x,int y,int code );
VCC4_API BOOL __stdcall   V4GetTitleString( int unit, int x, int y );
VCC4_API BOOL __stdcall   V4SetDate( int unit, int y,int m, int d );
VCC4_API BOOL __stdcall   V4GetDate( int unit );
VCC4_API BOOL __stdcall   V4SetTime( int unit, int h,int m,int s );
VCC4_API BOOL __stdcall   V4GetTime( int unit );
VCC4_API BOOL __stdcall   V4GetCameraTime( int unit );
VCC4_API BOOL __stdcall   V4GetPTTime( int unit );
VCC4_API BOOL __stdcall   V4SetDefault( int unit );
VCC4_API BOOL __stdcall   V4SetNotify( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetGlobalNotify( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetCameraPower( int unit, int iPower   );
VCC4_API BOOL __stdcall   V4SetFocusMode( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetZooming( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetZoomPosition( int unit, int Pos );
VCC4_API BOOL __stdcall   V4GetZoomPosition( int unit );
VCC4_API BOOL __stdcall   V4SetBackLight( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetExposed( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetAELock( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetShutterSpeedx( int unit, int Speed );
VCC4_API BOOL __stdcall   V4GetShutterSpeed( int unit );
VCC4_API BOOL __stdcall   V4SetAGCGain( int unit, int Gain  );
VCC4_API BOOL __stdcall   V4GetAGCGain( int unit  ); 
VCC4_API BOOL __stdcall   V4SetIris( int unit, int Ival );
VCC4_API BOOL __stdcall   V4GetIris( int unit );
VCC4_API BOOL __stdcall   V4SetAEVal( int unit, int AEVal );
VCC4_API BOOL __stdcall   V4GetAEVal( int unit );
VCC4_API BOOL __stdcall   V4SetWhiteBalance( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetWhiteBalanceMx( int unit, int iGain );
VCC4_API BOOL __stdcall   V4GetWhiteBalance( int unit );
VCC4_API BOOL __stdcall   V4SetShutterSpeed( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4SetFading( int unit, int iMode  );
VCC4_API BOOL __stdcall   V4CameraReset( int unit  );
VCC4_API BOOL __stdcall   V4GetZoomRatio( int unit  );
VCC4_API BOOL __stdcall   V4GetCCDSize( int unit  );
VCC4_API BOOL __stdcall   V4SetFocusPosition( int unit, int FocusPos );
VCC4_API BOOL __stdcall   V4GetFocusPosition( int unit  );
VCC4_API BOOL __stdcall   V4SetOnePushAF( int unit  );
VCC4_API BOOL __stdcall   V4GetFocusRange( int unit );
VCC4_API BOOL __stdcall   V4SetZoomPosition4( int unit, int pos );
VCC4_API BOOL __stdcall   V4GetZoomPosition4( int unit );
VCC4_API BOOL __stdcall   V4SetZoomSpeed( int unit, int Zoomnum  );
VCC4_API BOOL __stdcall   V4GetZoomSpeed( int unit );
VCC4_API BOOL __stdcall   V4GetZoomMax( int unit );
VCC4_API DWORD __stdcall  V4GetStatus( void );
VCC4_API int __stdcall  V4GetLastError( DWORD *syserr );
VCC4_API int __stdcall  V4GetMsgVB( int msgno );
VCC4_API int __stdcall  V4GetLastCmdstrVB( void );
VCC4_API BYTE __stdcall  V4PicMsgVB( int pos );
VCC4_API BYTE __stdcall  V4PicLastCmdVB( int pos );
VCC4_API BOOL __stdcall  V4SetManualCommand( LPBYTE command, int len );
VCC4_API BOOL __stdcall  V4GetCameraVersion( int unit ,int iMode);
VCC4_API BOOL __stdcall  V4GetPedestalModel( int unit);
VCC4_API BOOL __stdcall  V4GetCameraModel( int unit);
BYTE V4Msgstr[200];
BYTE V4Sendcmd[200];

}