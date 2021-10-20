#pragma once

#include "IUdisk.h"
#include <windows.h> 
#include <string>
#include <vector>



typedef struct _MyDiskInfo
{
	BYTE    AdapterID;
	BYTE    TargetID;
	HMODULE hLibrary;
	HANDLE  vwin32;
	HANDLE  hDisk;
	int     nLun;
	CHAR    DiskLetter;
	bool    isCDROM;
	bool    bSM3211;// added on 2009/12/03
	int     IC_Ver;// added for 3255AB on j0329

	int     nStringOffset;	// added on 2009/10/30
	int     nInquiryOffset; // added on 2009/10/30

}MyDiskInfo, *PMyDiskInfo;

typedef BOOL( *OpenSMIDiskDLL)();
typedef BOOL( *CloseSMIDiskDLL)();

typedef BOOL( *GetCDROMLetterDLL)(CHAR *);
typedef BOOL( *ReadSerialNumberDLL)(CHAR *, CHAR);
typedef BOOL( *GetChipNameDLL)(CHAR *, CHAR);


typedef BOOL( *WriteToCDROMDLL)(CHAR * FilePathName, BOOL BlockingMode, CHAR, HWND);


typedef BOOL( *FSInitDLL)(ULONG, CHAR);
typedef ULONG( *FSGetListLengthDLL)(CHAR  cLetter, CHAR *SubDir);

typedef void* ( *FSOpenDLL)(CHAR cLetter, CHAR *pFileName, CHAR *Mode);
typedef INT( *FSCloseDLL)(void *pFileHandle);
typedef DWORD( *FSReadDLL)(void *ptr, DWORD size, DWORD count, void *pFileHandle);
typedef DWORD( *FSReadBufferDLL)(CHAR  cLetter, CHAR  *cFileName, UCHAR *pBuffer, ULONG nBufferLen, CHAR* szPasswd);
typedef BOOL( *FSCopyFileToLocalDLL)(CHAR *TargetPath, CHAR *SrcPath, BOOL BlockingMode, CHAR cLetter, void * pWnd, CHAR *szPasswd);

typedef DWORD( *FSWriteDLL)(void *ptr, DWORD size, DWORD count, void *pFileHandle);
typedef BOOL ( *FSWriteBufferDLL)(CHAR *FileName, UCHAR *pBuffer, DWORD size, BOOL BlockingMode, CHAR cLetter, void *pWnd, CHAR *szPasswd);

typedef INT( *FSSeekDLL)(void *pFileHandle, INT offset, INT origin);
typedef DWORD( *FSTellDLL)(void *pFileHandle);
typedef ULONG (* FSGetFileSizeDLL)(CHAR cLetter, CHAR* szFileName);
typedef BOOL( *FSCheckFileExistDLL)(CHAR cLetter, CHAR*pFileName);

typedef BOOL( *FSCreateDirectoryDLL)(CHAR cLetter, CHAR *Subdir);
typedef BOOL( *DeleteDataInHiddenDLL)(CHAR * FileName, BOOL BlockingMode, CHAR cLetter, void* pWnd, CHAR *szPasswd);




class TDManagerSMI :
	public IUDisk
{
private:
	HINSTANCE smidll;
	HINSTANCE smidll2;

	MyDiskInfo *info;
	CHAR letter[8];
	CHAR sChipName[64];
	CHAR sn[32];
	char teclastFileTmp[256];
	BOOL isXP;


	OpenSMIDiskDLL OpenSMIDisk;	
	CloseSMIDiskDLL CloseSMIDisk;

	GetCDROMLetterDLL GetCDROMLetter;
	GetChipNameDLL GetChipName;
	ReadSerialNumberDLL ReadSerialNumber;

	WriteToCDROMDLL WriteToCDROM;	
	
	FSInitDLL FSInit;
	FSGetListLengthDLL FSGetListLength;
	FSCreateDirectoryDLL FSCreateDirectory;
	DeleteDataInHiddenDLL DeleteDataInHidden;
	FSOpenDLL FSOpen;
	FSCloseDLL FSClose;
	FSReadDLL FSRead;
	FSReadBufferDLL FSReadBuffer;
	FSCopyFileToLocalDLL FSCopyFileToLocal;
	FSWriteDLL FSWrite;
	FSWriteBufferDLL FSWriteBuffer;
	FSSeekDLL FSSeek;
	FSTellDLL FSTell;
	FSGetFileSizeDLL FSGetFileSize;
	FSCheckFileExistDLL FSCheckFileExist;

	unsigned long getFileSizeWithHandle(char * filepath, void * file);
	wchar_t*  charToWchar(char * value);
	
	char* pathSplit0(char * oripath);

	void GetFileTmpPath();
	char* TranTmpFilePath(char * fn) ;
	char * GetFileNameFromPath(char* path);


public:
	TDManagerSMI();
	~TDManagerSMI();
	bool init();

	bool openDevice(char * password = "123456");
	bool updateCDROM(char * isopath, int * progress = 0);
	char* getDriveList();
	//bool openFile(char * filepath, bool isCreate);
	bool writeFileByBytes(char * filepath, unsigned char * content, unsigned long  length);
	bool writeFileContent(char * filepath, unsigned char * content, unsigned long length, long startP, bool isEof);
	unsigned char * readFile(char * filepath);
	std::vector<unsigned char> readFileWithVector(char * filepath);
	std::vector<unsigned char> readFileContent(char * filepath, unsigned long start, unsigned long length);
	bool deleteFile(char * filepath);
	unsigned long getFileSize(char * filepath);
	bool createFolder(char * path);
	bool deleteFolder(char * path);
	void findFiles(char * pattern, vector<string> * vec);
	char * getDiskInfo();
	bool formatDisk();
	bool changePassWord(char * pw);
	bool flush();
	bool cleanCache();
	bool close();
	bool dispose();

	bool formatHiddenArea();
};
