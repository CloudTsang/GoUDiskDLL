#include "TDManagerSMI.h"
#include "stdafx.h"

#include <string>
#include <vector>
#include <iostream>
#include<windows.h>  
#include<time.h>  

#include <stdlib.h>
#include <stdio.h>
#include <direct.h>   
#include <io.h>  

static pthread_mutex_t smi_mutex;

void TDManagerSMI::GetFileTmpPath() {
	char * appdata = getenv("APPDATA");	
	strcat(teclastFileTmp, appdata);
	strcat(teclastFileTmp, "\\STC\\Teclast");
	if (access(teclastFileTmp, 0) != 0) {
		mkdir(teclastFileTmp);
	}
	strcat(teclastFileTmp, "\\");
	printf("文件中转路径 ： %s\n", teclastFileTmp);
}

char* TDManagerSMI::TranTmpFilePath(char * fn) {
	char * tmp = (char *)malloc(256);
	memset(tmp, 0, 256);
	strcat(tmp, teclastFileTmp);
	strcat(tmp, fn);
	return tmp;
}

char * TDManagerSMI::GetFileNameFromPath(char* path) {
	char szDrive[_MAX_DRIVE];   //磁盘名  
	char szDir[_MAX_DIR];       //路径名  
	char szFname[_MAX_FNAME];   //文件名  
	char szExt[_MAX_EXT];       //后缀名  
	_splitpath(path, szDrive, szDir, szFname, szExt); //分解路径  

	int len = strlen(szDir);
	for (int i = 0; i < len; i++) {
		if (szDir[i]=='\\' || szDir[i]=='/') {
			szDir[i] = '_';
		}
	}
	
	char * tmp = (char *)malloc(256);
	memset(tmp, 0, 256);
	strcat(tmp, szDir);
	strcat(tmp, "_");
	strcat(tmp, szFname);
	strcat(tmp, szExt);
	return tmp;
}


wchar_t* TDManagerSMI::charToWchar(char * value) {

	const size_t cSize = strlen(value) + 1;
	wchar_t* wc = new wchar_t[cSize];
	MultiByteToWideChar(CP_ACP, 0, value, strlen(value) + 1, wc, (strlen(value) + 1)*2);
	return wc;
}

//台电牌smi方案文件路径不以0:打头 
char * TDManagerSMI::pathSplit0(char * oripath)
{
	return &oripath[3];// 0:
}

TDManagerSMI::TDManagerSMI()
{
	printf("台电U盘SMI方案管理工具v1.0\n");
	pthread_mutex_init(&smi_mutex, NULL);
}

TDManagerSMI::~TDManagerSMI()
{
}

bool TDManagerSMI::init()
{
	char dllpath[MAX_PATH];
    memset(dllpath,0,MAX_PATH);
    GetModuleFileNameA(NULL,dllpath,MAX_PATH);
    (_tcsrchr(dllpath, _T('\\')))[1] = 0;
    strcat(dllpath , "udiskapi.dll");
    printf("dll path = %s\n" , dllpath);
	smidll = LoadLibrary(TEXT(dllpath));
	// smidll = LoadLibrary("udiskapi.dll");
	
	if (smidll == NULL) {
		DWORD ercode = GetLastError();
		printf("读取SMI DLL失败 : %d\n" , ercode);
		return false;
	}

	OpenSMIDisk = (OpenSMIDiskDLL)GetProcAddress(smidll, "YDOpenSMIDiskDLL");
	CloseSMIDisk = (CloseSMIDiskDLL)GetProcAddress(smidll , "YDSMISDKCloseDLL");

	ReadSerialNumber = (ReadSerialNumberDLL)GetProcAddress(smidll, "YDGetSerialNumberDLL");

	GetChipName = (GetChipNameDLL)GetProcAddress(smidll, "YDGetChipNameDLL");
	
	GetCDROMLetter = (GetCDROMLetterDLL)GetProcAddress(smidll, "YDGetCDROMLetterDLL");

	WriteToCDROM = (WriteToCDROMDLL)GetProcAddress(smidll, "YDWriteToCDROMDLL");
	
	FSCreateDirectory = (FSCreateDirectoryDLL)GetProcAddress(smidll, "YDFSCreateDirectoryDLL");
	DeleteDataInHidden = (DeleteDataInHiddenDLL)GetProcAddress(smidll, "YDDeleteDataInHiddenDLL");

	FSInit = (FSInitDLL)GetProcAddress(smidll, "YDFSInitDLL");
	FSGetListLength = (FSGetListLengthDLL)GetProcAddress(smidll , "YDFSGetListLengthDLL");
	FSOpen = (FSOpenDLL)GetProcAddress(smidll, "YDFSOpenDLL");
	FSClose = (FSCloseDLL)GetProcAddress(smidll, "YDFSCloseDLL");
	FSRead = (FSReadDLL)GetProcAddress(smidll, "YDFSReadDLL");
	FSReadBuffer = (FSReadBufferDLL)GetProcAddress(smidll , "YDReadFileFromHiddenDLL");
	FSWrite = (FSWriteDLL)GetProcAddress(smidll, "YDFSWriteDLL");
	FSWriteBuffer = (FSWriteBufferDLL)GetProcAddress(smidll , "YDWriteFileToHiddenDLL");
	FSCopyFileToLocal = (FSCopyFileToLocalDLL)GetProcAddress(smidll , "YDCopyFileToLocalDLL");
	FSTell = (FSTellDLL)GetProcAddress(smidll, "YDFSTellDLL");
	FSGetFileSize = (FSGetFileSizeDLL)GetProcAddress(smidll , "YDFSGetFileSizeDLL");
	FSSeek = (FSSeekDLL)GetProcAddress(smidll, "YDFSSeekDLL");
	FSCheckFileExist = (FSCheckFileExistDLL)GetProcAddress(smidll, "YDFSCheckFileExistDLL");

	GetFileTmpPath();

	OSVERSIONINFO   osver;
	osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osver);
	if (osver.dwPlatformId == 2 && osver.dwMajorVersion == 5) {
		isXP = true;
	}else{
		isXP = false;
	}
	isXP = true;

	return true;
}

bool TDManagerSMI::openDevice(char * password)
{
	printf("%d\n" , OpenSMIDisk);
	OpenSMIDisk();
	printf("TDManagerSMI::openDevice1 %s\n");
	memset(letter, 0, 8);
	GetCDROMLetter(letter);
	printf("盘符 %s\n" , letter);

	
	/*memset(sChipName, 0, 64);
	GetChipName(sChipName, letter[0]);
	printf("台电SMI芯片型号 : %s\n", sChipName);
	*/
	// ULONG ret = FSGetListLength(letter[0] , NULL);
	// printf("File List Length = %d\n" , ret);
	
	return true;
}

bool TDManagerSMI::updateCDROM(char * isopath, int * progress)
{
	HWND hd = FindWindow(NULL, "ISO更新");//GetDesktopWindow();
	if (hd == NULL) {
		hd = GetDesktopWindow();
	}
	printf("iso path : %s\n", isopath);
	BOOL ret = WriteToCDROM(isopath, false, letter[0], hd);
	if (ret) {
		printf("刷入iso完毕\n");
	}
	else {
		printf("刷入iso失败\n");
		return false;
	}
	return true;
}

char * TDManagerSMI::getDriveList()
{
	return NULL;
}

bool TDManagerSMI::writeFileByBytes(char * filepath0, unsigned char * content, unsigned long length)
{
	char * filepath = pathSplit0(filepath0);
	pthread_mutex_lock(&smi_mutex);
	BOOL isExist = FSCheckFileExist(letter[0], filepath);
	if (isExist) {
		printf("file %s is already existed.\n", filepath0);
		DeleteDataInHidden(filepath, false, letter[0], NULL, "");
	}


	BOOL ret = FSWriteBuffer(filepath , content , length , true , letter[0], NULL , "");
	pthread_mutex_unlock(&smi_mutex);
	if(!ret){
		printf("write file failed\n");
		return false;
	}

	/*void* fileHandle = FSOpen(letter[0], filepath, "wb");
	if (fileHandle == NULL) {
		return false;
	}
	unsigned char * tmpBuffer = content;
	DWORD bytesToWrite = length;
	DWORD bytesWritten = 0;
	FSSeek(fileHandle, 0, SEEK_SET);	
	do {
		printf("here1\n");
		bytesWritten = FSWrite(tmpBuffer, 1, length, fileHandle);
		printf("here2\n");
		bytesToWrite -= bytesWritten;
		tmpBuffer += bytesWritten;
	} while (bytesToWrite > 0);	
	FSClose(fileHandle);*/
	return true;
}

bool TDManagerSMI::writeFileContent(char * filepath, unsigned char * content, unsigned long length, long startP, bool isEof)
{
	return false;
}

unsigned char * TDManagerSMI::readFile(char * filepath0)
{
	return 0;	
}

std::vector<unsigned char> TDManagerSMI::readFileWithVector(char * filepath)
{	
	pthread_mutex_lock(&smi_mutex);
	char * filepath0 = pathSplit0(filepath);	
	ULONG fileSize = FSGetFileSize(letter[0] , filepath0);
	UCHAR * readBuffer =  (UCHAR*)calloc(1,fileSize);
	if (fileSize == 0) {
		pthread_mutex_unlock(&smi_mutex);
		printf("file %s not exist.\n" , filepath0);
		std::vector<unsigned char> v0(0);
		return v0;		
	}

	if(isXP){
		char * fName = GetFileNameFromPath(filepath);
		char * lPath = TranTmpFilePath(fName);
		BOOL ret0 = FSCopyFileToLocal(lPath , filepath0 , true , letter[0] , NULL , NULL);
		if(!ret0){
			pthread_mutex_unlock(&smi_mutex);
			printf("read file %s failed.\n" , filepath0);
			std::vector<unsigned char> v0(0);
			return v0;	
		}

		FILE *fl = fopen(lPath , "a+b");	
		fseek(fl, 0, SEEK_SET);
		fread(readBuffer, 1, fileSize, fl);
		fclose(fl);	
	}else{
		BOOL ret0 = FSReadBuffer(letter[0] , filepath0 , readBuffer , fileSize , "");
		if(!ret0){
			pthread_mutex_unlock(&smi_mutex);
			printf("read file failed\n");
			std::vector<unsigned char> v0(0);
			return v0;		
		}
	}
	
	std::vector<unsigned char> v(fileSize);
	if (fileSize > 0) {
		memcpy(&v[0], readBuffer, fileSize);
	}
	pthread_mutex_unlock(&smi_mutex);
	return v;
}

std::vector<unsigned char> TDManagerSMI::readFileContent(char * filepath, unsigned long start, unsigned long length)
{
	return std::vector<unsigned char>();
}

bool TDManagerSMI::deleteFile(char * filepath)
{	
	printf("delete file %s\n" , filepath);
	char * dir0 = pathSplit0(filepath);	
	pthread_mutex_lock(&smi_mutex);
	ULONG fileSize = FSGetFileSize(letter[0] , dir0);
	if(fileSize==0){
		pthread_mutex_unlock(&smi_mutex);
		return true;
	}
	BOOL ret = DeleteDataInHidden(dir0, true, letter[0], NULL, "");
	pthread_mutex_unlock(&smi_mutex);
	if (!ret) {
		printf("delete file %s failed.\n", filepath);
		return false;
	}
	return true;
}

unsigned long TDManagerSMI::getFileSize(char * filepath)
{
	char * dir0 = pathSplit0(filepath);
	BOOL isExist = FSCheckFileExist(letter[0], filepath);
	if (isExist) {
		return 1;
	}else{
		return 0;
	}
	ULONG size = FSGetFileSize(letter[0] , dir0);
	return size;
}

unsigned long TDManagerSMI::getFileSizeWithHandle(char * filepath, void * fileHandle) {
	//FSSeek(fileHandle, 0, SEEK_END);
	unsigned long size = FSTell(fileHandle);
	return size;
}

bool TDManagerSMI::createFolder(char * path)
{	
	CHAR dir[256];
	memset(dir, 0, 256);	
	strcat(dir, path);
	strcat(dir, "\\");
	char * dir0 = pathSplit0(dir);		
	pthread_mutex_lock(&smi_mutex);
	BOOL ret = FSCreateDirectory(letter[0], dir0);
	pthread_mutex_unlock(&smi_mutex);
	// BOOL ret = FSCreateDirectory('I', dir0);
	if (!ret) {
		printf("create folder failed.\n");
		return false;
	}
	return true;
}

bool TDManagerSMI::deleteFolder(char * path)
{
	// CHAR dir[256];
	pthread_mutex_lock(&smi_mutex);
	printf("delete file %s\n" , path);
	char * dir = (char *)malloc(256);
	memset(dir, 0, 256);
	strcat(dir, path);
	strcat(dir, "\\");
	char * dir0 = pathSplit0(dir);
	// HWND hd = GetDesktopWindow();
	
	BOOL ret = DeleteDataInHidden(dir0, true, letter[0], NULL, "");
	pthread_mutex_unlock(&smi_mutex);
	// BOOL ret = DeleteDataInHidden(dir0, true, 'I', NULL, "");	
	return true;
}

void TDManagerSMI::findFiles(char * pattern, vector<string>* vec)
{
}

char * TDManagerSMI::getDiskInfo()
{
	
	memset(sn, 0, 32);
	ReadSerialNumber(sn, letter[0]);	
	printf("serial number : %s\n", sn);
	return sn;
}

bool TDManagerSMI::formatDisk()
{
	return false;
}

bool TDManagerSMI::changePassWord(char * pw)
{
	return false;
}

bool TDManagerSMI::flush()
{
	return false;
}

bool TDManagerSMI::cleanCache()
{
	return false;
}

bool TDManagerSMI::close()
{
	return false;
}

bool TDManagerSMI::dispose()
{
	CloseSMIDiskDLL();
	return false;
}

bool TDManagerSMI::formatHiddenArea()
{
	BOOL ret = FSInit(0, letter[0]);
	if (!ret) {
		printf("Failed initializing hidden area file system.\n");
	}
	return false;
}
