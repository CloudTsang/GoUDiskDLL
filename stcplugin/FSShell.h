#ifndef     FSSHELL_H
#define     FSSHELL_H

#include <windows.h> 

#define FS_MAX_PATH         260         // �ļ�����󳤶� 
#define FS_RW_MAX_SECTOR    128         // ���ζ�д��������� 
#define INVALID_DATA_FLAG   (DWORD)-1   // ��Ч���ݱ�ʶ 
//#define INVALID_DATA_FLAG   (DWORD)-1  // ��Ч���ݱ�ʶ 
//#define INVALID_HANDLE_VALUE    INVALID_DATA_FLAG   // ��Ч��� 
//#define TRUE  (Byte)-1 
//#define FALSE  0 

// typedef unsigned __int64     ULONGLONG; 
// typedef __int64              LONGLONG; 
// typedef unsigned long        DWORD; 
typedef long                 INTEGER; 
// typedef unsigned short       WORD; 
// typedef short                SHORT; 
// typedef unsigned char        BYTE; 
// typedef char                 CHAR; 
// typedef wchar_t              WCHAR; 
// // typedef void                 VOID; 
// // typedef void                 *PVOID; 
// // typedef void                 *PVOID64; 
// typedef BYTE                 BOOLEAN;
// 
typedef BOOLEAN              Boolean; 
typedef WCHAR                WideChar; 
typedef CHAR                 Char; 
typedef Char                 AnsiChar; 
typedef Char                 Shortint; 
typedef SHORT                Smallint; 
typedef INTEGER              Integer; 
typedef BYTE                 Byte; 
typedef WORD                 Word; 
typedef VOID                 Untyped; 
typedef PVOID                Pointer; 
typedef Integer              Longint; 
typedef DWORD                Cardinal; 
typedef Cardinal             LongWord; 
typedef LongWord             UInt32;
typedef LONGLONG             Int64; 
typedef ULONGLONG            UInt64; 
typedef Int64                NativeInt; 
typedef UInt64               NativeUInt;
// 
typedef Char                 *PAnsiChar; 
typedef PAnsiChar            PChar; 
typedef WideChar             *PWideChar; 
typedef Byte                 ByteBool; 
typedef Word                 WordBool; 
typedef LongWord             LongBool; 
typedef WCHAR                TChar; 
typedef TChar                *PTChar;
// 
// //typedef Char              INT8; 
// typedef Smallint          INT16; 
// typedef Longint           INT32 
// typedef Int64             INT64 
// typedef Byte              UINT8; 
// typedef Word              UINT16; 
// typedef LongWord          UINT32; 
// typedef UInt64            UINT64; 
//============================= 
//��ǰ 32λ�� SDK�����ͨ������ 
//============================= 
// typedef UINT64            UINT_BIG;        //������� 
// typedef UINT8             UINT_SMALL;      //��С���� 
// typedef INT32             INT;             //��׼����Ϊ 32λ 
// typedef UINT32            UINT;            //��׼����Ϊ 32λ 
// typedef LongBool          BOOL;            //��׼��������Ϊ 32λ��0:False,�� 0:True 
// typedef INT32             FS_RESULT;       //��׼�ļ�ϵͳ����ֵ

typedef	INT32				HResult;

//������Ͷ��� 
typedef UINT				THandle;                    //ͨ�þ�� 
typedef THandle				TFileHandle;                //ͨ���ļ���� 
typedef THandle				TFSHandle;                  //�ļ�ϵͳͨ�þ�� 
typedef THandle				TFSFileHandle;              //�ļ�ϵͳͨ���ļ���� 


//����ͨ������ 
typedef UINT				*PUINT;                     //��׼����ָ�� 
typedef INT64				TLargeInteger;              //������ 
typedef TLargeInteger		*PLargeInteger;             //������ָ��

typedef struct _FSFILEMTIME {  // st   
    Word wYear;  
    Word wMonth;  
    Word wDayOfWeek;  
    Word wDay;  
    Word wHour;  
    Word wMinute;  
    Word wSecond;  
    Word wMilliseconds;  
} FSFILEMTIME; 
//�������ƣ� 
typedef FSFILEMTIME TFSFileTime; 
typedef TFSFileTime *PFSFileTime;

typedef struct _FSOVERLAPPED { // o   
    DWORD    Internal; 
    DWORD    InternalHigh; 
    DWORD    Offset; 
    DWORD    OffsetHigh; 
    THandle  hEvent; 
} FSOVERLAPPED; 
//�������ƣ� 
typedef FSOVERLAPPED   TFSOverlapped; 
typedef TFSOverlapped  *PFSOverlapped; 

typedef struct _FSSECURITY_ATTRIBUTES { // sa   
    DWORD    nLength;  
    Pointer  lpSecurityDescriptor;  
    BOOL     bInheritHandle;  
} FSSECURITY_ATTRIBUTES; 
//�������ƣ� 
typedef FSSECURITY_ATTRIBUTES   TFSSecurityAttributes; 
typedef TFSSecurityAttributes   *PFSSecurityAttributes;

typedef struct _FS_FIND_DATA { // wfd   
    UINT        iAttribute;                   //�ļ����� 
    TFSFileTime ftCreationTime;               //����ʱ�� 
    TFSFileTime ftLastAccessTime;             //����ʱ�� 
    TFSFileTime ftLastWriteTime;              //�޸�ʱ�� 
    DWORD       nFileSizeHigh;                //�ļ���С���ֽ� 
    DWORD       nFileSizeLow;                 //�ļ���С���ֽ� 
    //TCHAR       sName[FS_MAX_PATH];           //�ļ����� 
    TChar       sName[FS_MAX_PATH];           //�ļ����� 
} FS_FIND_DATA; 
//�������ƣ� 
typedef FS_FIND_DATA   TFSFindData; 
typedef TFSFindData    *PFSFindData;

typedef HResult    (__stdcall *PDllGetClassObject)(UInt32 CLSID, UInt32 IID, PVOID Obj);
typedef HResult    (__stdcall *PDllCanUnloadNow)();
typedef HResult    (__stdcall *PDllRegisterServer)();
typedef HResult    (__stdcall *PDllUnregisterServer)();

typedef UINT    (__stdcall *PFSHInit)();
typedef BOOL    (__stdcall *PFSHFinal)(UINT hHandle);
typedef DWORD   (__stdcall *PFSHGetLastError)(UINT hHandle);
typedef BOOL    (__stdcall *PFSHGetDriveList)(UINT hHandle,Char* sDriveList);
typedef BOOL    (__stdcall *PFSHSetDriveSpeedDetect)(UINT hHandle,BOOL bEnable);
typedef BOOL    (__stdcall *PFSHOpen)(UINT hHandle, Char cDrive, Char* sPwd);
typedef BOOL    (__stdcall *PFSHClose)(UINT hHandle);
typedef BOOL    (__stdcall *PFSHFlushBuffers)(UINT hHandle);
typedef BOOL    (__stdcall *PFSHChangePwd)(UINT hHandle, Char* sNewPwd);
typedef BOOL    (__stdcall *PFSHGetMountedIDList)(UINT hHandle, Char* sIDList); 
typedef BOOL    (__stdcall *PFSHFormat)(UINT hHandle, Char* sID, BOOL bQuick);
typedef BOOL    (__stdcall *PFSHGetDeviceSerialNumber)(UINT hHandle, Char* sDeviceSerialNumber); 
typedef BOOL    (__stdcall *PFSHUpdateCDROM)(UINT hHandle, Char* szISOFile, Pointer pCallBack, UINT nCustomData); 
typedef UINT32  (__stdcall *PFSHGetDeviceTotalSize)(UINT hHandle, PINT32 pSizeHigh); 

typedef TFSHandle (__stdcall *PFSHFindFirstFile)(UINT hHandle, TChar* lpFileName, TFSFindData* lpFindFileData);
typedef BOOL    (__stdcall *PFSHFindNextFile)(UINT hHandle, TFSHandle hFindFile, TFSFindData* lpFindFileData);
typedef BOOL    (__stdcall *PFSHFindClose)(UINT hHandle, TFSHandle hFindFile); 
typedef BOOL    (__stdcall *PFSHGetDiskFreeSpace)(
    UINT    hHandle, 
    TChar*  lpRootPathName, 
    DWORD*  lpSectorsPerCluster, 
    DWORD*  lpBytesPerSector, 
    DWORD*  lpNumberOfFreeClusters, 
    DWORD*  lpTotalNumberOfClusters);
typedef TFSFileHandle   (__stdcall *PFSHCreateFile)( 
    UINT                    hHandle,
    TChar*                  lpFileName,   
    DWORD                   dwDesiredAccess, 
    DWORD                   dwShareMode, 
    TFSSecurityAttributes*  lpSecurityAttributes, 
    DWORD                   dwCreationDisposition, 
    DWORD                   dwFlagsAndAttributes, 
    TFSFileHandle           hTemplateFile 
    ); 
typedef BOOL    (__stdcall *PFSHDeleteFile)(UINT hHandle, TChar*  lpFileName); 
typedef BOOL    (__stdcall *PFSHMoveFile)(UINT hHandle, TChar* lpExistingFileName, TChar* lpNewFileName);
typedef BOOL    (__stdcall *PFSHWriteFile)( 
    UINT    hHandle, 
    TFSFileHandle   hFile, 
    Byte*   Buffer, 
    DWORD   nNumberOfBytesToWrite, 
    DWORD*  lpNumberOfBytesWritten, 
    TFSOverlapped*  lpOverlapped 
    );
typedef BOOL    (__stdcall *PFSHReadFile)( 
    UINT    hHandle, 
    TFSFileHandle   hFile, 
    Byte*   Buffer, 
    DWORD   nNumberOfBytesToRead, 
    DWORD*  lpNumberOfBytesRead, 
    TFSOverlapped*  lpOverlapped 
    );
typedef BOOL    (__stdcall *PFSHFlushFileBuffers)( 
    UINT            hHandle, 
    TFSFileHandle   hFile 
    );
typedef DWORD   (__stdcall *PFSHGetFileAttributes)(UINT hHandle, TChar* lpFileName); 
typedef BOOL    (__stdcall *PFSHSetFileAttributes)(UINT hHandle, TChar* lpFileName, DWORD dwFileAttributes);
typedef DWORD   (__stdcall *PFSHGetFileSize)(UINT hHandle, TFSFileHandle hFile, DWORD* lpFileSizeHigh);
typedef BOOL    (__stdcall *PFSHGetFileTime)( 
    UINT            hHandle, 
    TFSFileHandle   hFile, 
    TFSFileTime*    lpCreationTime, 
    TFSFileTime*    lpLastAccessTime, 
    TFSFileTime*    lpLastWriteTime 
    );
typedef BOOL    (__stdcall *PFSHSetFileTime)( 
    UINT            hHandle, 
    TFSFileHandle   hFile, 
    TFSFileTime*    lpCreationTime, 
    TFSFileTime*    lpLastAccessTime, 
    TFSFileTime*    lpLastWriteTime 
    );
typedef BOOL    (__stdcall *PFSHSetEndOfFile)(UINT hHandle, TFSFileHandle hFile);
typedef DWORD   (__stdcall *PFSHSetFilePointer)( 
    UINT            hHandle, 
    TFSFileHandle   hFile,
    Longint         lDistanceToMove, 
    Longint*        lpDistanceToMoveHigh, 
    DWORD           dwMoveMethod 
    );
typedef BOOL    (__stdcall *PFSHCloseHandle)(UINT hHandle, TFSHandle hObject);
typedef BOOL    (__stdcall *PFSHCreateDirectory)(UINT hHandle, TChar* lpPathName, TFSSecurityAttributes*  lpSecurityAttributes);
typedef BOOL    (__stdcall *PFSHRemoveDirectory)(UINT hHandle, TChar* lpPathName);

/**********************************************
Function Table MODE
**********************************************/

typedef Pointer TFSFunction;

typedef struct _FS_FUNC_DEVICE_TABLE { //fdt
    TFSFunction		SHSetKernelFile;
    TFSFunction		SHSetUDiskFile;
    TFSFunction		SHIsHandleValid;
    TFSFunction		SHInit;
    TFSFunction		SHFinal;
    TFSFunction		SHGetLastError;
    TFSFunction		SHGetDriveSpeed;
    //{$IFDEF ANDROID}
    TFSFunction		SHSetDriveList;
    //{$ENDIF}
    TFSFunction		SHGetDriveList;
    TFSFunction		SHGetDriveListEx;
    TFSFunction		SHSetDriveSpeedDetect;
    TFSFunction		SHSetReadSpeed;
    TFSFunction		SHSetWriteSpeed;
	// 2016-11-26 add
	TFSFunction		SHGetMaxTransSiz;
    TFSFunction		SHSetMaxTransSize;
} FS_FUNC_DEVICE_TABLE;
typedef FS_FUNC_DEVICE_TABLE TFSFuncDeviceTable;


typedef struct _FS_FUNC_SHELL_SDK_TABLE { //fsst
    TFSFunction		SHSetSecurityLayer;
    TFSFunction		SHOpen;
    TFSFunction		SHOpenEx;
    TFSFunction		SHOpenEx2;
    TFSFunction		SHLoadDrive;
    TFSFunction		SHLoadDriveEx;
    TFSFunction		SHUnLoadDrive;
    TFSFunction		SHLoadFS;
    TFSFunction		SHUnLoadFS;
    TFSFunction		SHOpenAsUser;
    TFSFunction		SHOpenAsUserEx;
    TFSFunction		SHOpenAsUserEx2;
    TFSFunction		SHClose;
    TFSFunction		SHFlushBuffers;
    TFSFunction		SHChangePwd;
    TFSFunction		SHGetMountedIDList;
    TFSFunction		SHFormat;
    TFSFunction		SHFormatEx;
    TFSFunction		SHGetDeviceSerialNumber;
	TFSFunction		SHUpdateCDROM;
	TFSFunction		SHUpdateCDROM2; // 20161110 add
	TFSFunction		SHGetDeviceTotalSize;

} FS_FUNC_SHELL_SDK_TABLE;
typedef FS_FUNC_SHELL_SDK_TABLE TFSFuncShellSDKTable;

#ifdef WIN32
#define FIX_FUNC_NAME(FuncName) FSSHELL_##FuncName
#else
#define FIX_FUNC_NAME 
#endif

typedef struct _FS_FUNC_FILE_SYSTEM_TABLE { //ffst
      TFSFunction	SHFindFirstFile; 
      TFSFunction	SHFindFirstFileA;
      TFSFunction	SHFindNextFile;
	  TFSFunction	SHFindNextFileA;
      TFSFunction	SHFindClose;

	  TFSFunction	FIX_FUNC_NAME(SHGetDiskFreeSpace);
	  TFSFunction	SHGetDiskFreeSpaceA;
      TFSFunction	FIX_FUNC_NAME(SHGetDiskFreeSpaceEx);
	  TFSFunction	SHGetDiskFreeSpaceExA;

      TFSFunction	SHCreateFile;
	  TFSFunction	SHCreateFileA;
      TFSFunction	SHCopyFile;
	  TFSFunction	SHCopyFileA;
      TFSFunction	SHDeleteFile;
	  TFSFunction	SHDeleteFileA;
      TFSFunction	SHMoveFile;
	  TFSFunction	SHMoveFileA;
      TFSFunction	SHWriteFile;
      TFSFunction	SHReadFile;
      TFSFunction	SHFlushFileBuffers;
      TFSFunction	SHGetFileAttributes;
	  TFSFunction	SHGetFileAttributesA;
      TFSFunction	SHSetFileAttributes;
	  TFSFunction	SHSetFileAttributesA;
      TFSFunction	SHGetFileInformationByHandle;
      TFSFunction	SHGetFileSize;
      TFSFunction	SHGetFileTime;
      TFSFunction	SHSetFileTime;
      TFSFunction	SHSetEndOfFile;
      TFSFunction	SHSetFilePointer;
      TFSFunction	SHCloseHandle;

      TFSFunction	SHCreateDirectory;
	  TFSFunction	SHCreateDirectoryA;
      TFSFunction	SHRemoveDirectory;
	  TFSFunction	SHRemoveDirectoryA;
} FS_FUNC_FILE_SYSTEM_TABLE;

typedef FS_FUNC_FILE_SYSTEM_TABLE TFSFuncFileSystemTable;


#endif  // FSSHELL_H