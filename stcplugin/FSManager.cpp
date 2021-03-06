#include "stdafx.h"
#include "FSManager.h"
#include "FSShell.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

static pthread_mutex_t mutex;

HINSTANCE hinstLib;
CHAR szDrives[28] = "";
CHAR szIDList[28] = "";
UINT hInitHandle;

int * FSManager::progress = 0;

UINT __stdcall SHUpdateCDROMCallBackFunc(UINT nCustomData, UINT nPos, UINT nMax) {
	//printf("SHUpdateCDROMCallBackFunc");
	if (nPos % 10 == 0) {
		if (FSManager::progress != 0) {
			*FSManager::progress = nPos;
		}
		//printf("SHUpdateCDROMCallBack: nCustomData=%d, nPos=%d nMax=%d! \n", nCustomData, nPos, nMax);
		//if(_callback)_callback->run(nPos);
	}
	else {

	}
	return 0;
}

FSManager::FSManager()
{
	printf("FS?? U?̹???v1.20\n");
	pthread_mutex_init(&mutex, NULL);
	stringResult = new char[32];
}


FSManager::~FSManager()
{
}

bool FSManager::getMountedList() {
	proc = "shGetMountedIDList";
	bool result = shGetMountedIDList(hInitHandle, szIDList);
	if (!result) {
		printError(hInitHandle, proc);
		return false;
	}
	else {
		printf("CSHGetMountedIDList: %s ! \n", szIDList);
	}
	return result;
}

wchar_t* FSManager::charToWchar(char * value) {
	const size_t cSize = strlen(value) + 1;
	wchar_t* wc = new wchar_t[cSize];
	MultiByteToWideChar(CP_ACP, 0, value, strlen(value) + 1, wc, (strlen(value) + 1)*2);
	return wc;
}

DWORD FSManager::printError(UINT id, char * description) {
	DWORD dwLastError = shGetLastError(id);
	printf("%s Error : %.8x(%d) \n", description, dwLastError, dwLastError);
	return dwLastError;
}

bool FSManager::openDevice(char * password) {
	proc = "shOpen";
	bool result = shOpen(hInitHandle, szDrives[0], "ydstc316");
	// bool result = shOpen(hInitHandle, szDrives[0], "123456");
	if (!result) {
		printError(hInitHandle, proc);
	}
	else {
		printf("CSHOpen: success ! \n");
	}
	result = getMountedList();
	return result;
}

bool FSManager::init() {

	printf("FSManager::init\n");
	hinstLib = LoadLibrary(TEXT("FSShell"));
	if (hinstLib != NULL) {
		shInit = (PFSHInit)GetProcAddress(hinstLib, "SHInit");
		shFinal = (PFSHFinal)GetProcAddress(hinstLib, "SHFinal");
		shGetLastError = (PFSHGetLastError)GetProcAddress(hinstLib, "SHGetLastError");

		shGetDriveList = (PFSHGetDriveList)GetProcAddress(hinstLib, "SHGetDriveList");
		shOpen = (PFSHOpen)GetProcAddress(hinstLib, "SHOpen");
		shClose = (PFSHClose)GetProcAddress(hinstLib, "SHClose");
		shGetMountedIDList = (PFSHGetMountedIDList)GetProcAddress(hinstLib, "SHGetMountedIDList");
		shUpdateCDROM = (PFSHUpdateCDROM)GetProcAddress(hinstLib, "SHUpdateCDROM");
		shChangePwd = (PFSHChangePwd)GetProcAddress(hinstLib, "SHChangePwd");
		shFormat = (PFSHFormat)GetProcAddress(hinstLib, "SHFormat");
		shFlushBuffers = (PFSHFlushBuffers)GetProcAddress(hinstLib, "SHFlushBuffers");

		shGetDeviceTotalSize = (PFSHGetDeviceTotalSize)GetProcAddress(hinstLib, "SHGetDeviceTotalSize");
		shGetDiskFreeSpace = (PFSHGetDiskFreeSpace)GetProcAddress(hinstLib, "SHGetDiskFreeSpace");
		shDeviceNumber = (PFSHGetDeviceSerialNumber)GetProcAddress(hinstLib, "SHGetDeviceSerialNumber");

		shCreateFile = (PFSHCreateFile)GetProcAddress(hinstLib, "SHCreateFile");
		shWriteFile = (PFSHWriteFile)GetProcAddress(hinstLib, "SHWriteFile");
		shReadFile = (PFSHReadFile)GetProcAddress(hinstLib, "SHReadFile");
		shFlushFileBuffers = (PFSHFlushFileBuffers)GetProcAddress(hinstLib, "SHFlushFileBuffers");
		shCloseFile = (PFSHCloseHandle)GetProcAddress(hinstLib, "SHCloseHandle");
		shDeleteFile = (PFSHDeleteFile)GetProcAddress(hinstLib, "SHDeleteFile");
		shGetFileSize = (PFSHGetFileSize)GetProcAddress(hinstLib, "SHGetFileSize");
		shSetEndOfFile = (PFSHSetEndOfFile)GetProcAddress(hinstLib, "SHSetEndOfFile");
		shSetFilePtr = (PFSHSetFilePointer)GetProcAddress(hinstLib, "SHSetFilePointer");
		shGetFileAttr = (PFSHGetFileAttributes)GetProcAddress(hinstLib, "SHGetFileAttributes");

		shFindFirstFile = (PFSHFindFirstFile)GetProcAddress(hinstLib, "SHFindFirstFile");
		shFindNextFile = (PFSHFindNextFile)GetProcAddress(hinstLib, "SHFindNextFile");
		shFindClose = (PFSHFindClose)GetProcAddress(hinstLib, "SHFindClose");

		shCreateDirectory = (PFSHCreateDirectory)GetProcAddress(hinstLib, "SHCreateDirectory");
		shRemoveDirectory = (PFSHRemoveDirectory)GetProcAddress(hinstLib, "SHRemoveDirectory");

	}
	if (shInit == NULL) {
		PDllGetClassObject pfDllGetClassObject = (PDllGetClassObject)GetProcAddress(hinstLib, "DllGetClassObject");
		if (pfDllGetClassObject != NULL)
		{
			TFSFuncDeviceTable FuncDevice;
			if (pfDllGetClassObject(0, 1, &FuncDevice) == S_OK)
			{
				shInit = (PFSHInit)FuncDevice.SHInit;
				shFinal = (PFSHFinal)FuncDevice.SHFinal;
				shGetDriveList = (PFSHGetDriveList)FuncDevice.SHGetDriveList;
				shGetLastError = (PFSHGetLastError)FuncDevice.SHGetLastError;
			}


			TFSFuncShellSDKTable FuncShellSDK;
			if (pfDllGetClassObject(0, 2, &FuncShellSDK) == S_OK)
			{
				shOpen = (PFSHOpen)FuncShellSDK.SHOpen;
				shClose = (PFSHClose)FuncShellSDK.SHClose;
				shGetMountedIDList = (PFSHGetMountedIDList)FuncShellSDK.SHGetMountedIDList;
				shUpdateCDROM = (PFSHUpdateCDROM)FuncShellSDK.SHUpdateCDROM;
				shFlushBuffers = (PFSHFlushBuffers)FuncShellSDK.SHFlushBuffers;
				shChangePwd = (PFSHChangePwd)FuncShellSDK.SHChangePwd;
				shFormat = (PFSHFormat)FuncShellSDK.SHFormat;
				shGetDeviceTotalSize = (PFSHGetDeviceTotalSize)FuncShellSDK.SHGetDeviceTotalSize;
				shDeviceNumber = (PFSHGetDeviceSerialNumber)FuncShellSDK.SHGetDeviceSerialNumber;
			}

			TFSFuncFileSystemTable FuncFileSystem;
			if (pfDllGetClassObject(0, 5, &FuncFileSystem) == S_OK)
			{
				shGetDiskFreeSpace = (PFSHGetDiskFreeSpace)FuncFileSystem.FIX_FUNC_NAME(SHGetDiskFreeSpace);
				shCreateFile = (PFSHCreateFile)FuncFileSystem.SHCreateFile;
				shWriteFile = (PFSHWriteFile)FuncFileSystem.SHWriteFile;
				shReadFile = (PFSHReadFile)FuncFileSystem.SHReadFile;
				shDeleteFile = (PFSHDeleteFile)FuncFileSystem.SHDeleteFile;
				shCloseFile = (PFSHCloseHandle)FuncFileSystem.SHCloseHandle;
				shFlushFileBuffers = (PFSHFlushFileBuffers)FuncFileSystem.SHFlushFileBuffers;
				shGetFileSize = (PFSHGetFileSize)FuncFileSystem.SHGetFileSize;
				shSetEndOfFile = (PFSHSetEndOfFile)FuncFileSystem.SHSetEndOfFile;
				shSetFilePtr = (PFSHSetFilePointer)FuncFileSystem.SHSetFilePointer;
				shGetFileAttr = (PFSHGetFileAttributes)FuncFileSystem.SHGetFileAttributes;

				shFindFirstFile = (PFSHFindFirstFile)FuncFileSystem.SHFindFirstFile;
				shFindNextFile = (PFSHFindNextFile)FuncFileSystem.SHFindNextFile;
				shFindClose = (PFSHFindClose)FuncFileSystem.SHFindClose;

				shCreateDirectory = (PFSHCreateDirectory)FuncFileSystem.SHCreateDirectory;
				shRemoveDirectory = (PFSHRemoveDirectory)FuncFileSystem.SHRemoveDirectory;
			}
		}
	}

	bool result = false;
	try {
		hInitHandle = (shInit)();
	}
	catch (char * err) {
		cout << err << endl;
		return false;
	}

	if (hInitHandle > 0) {
		printf("getdrivelist\n");
		result = (getDriveList() != '\0');
		if (!result)return false;
	}
	if (!hInitHandle) {
		printf("udisk manager init failed\n");
		return false;
	}
	else {
		printf("udisk manager init success\n");
		return true;
	}
	return false;
}

bool FSManager::updateCDROM(char * isopath, int * p) {
	proc = "shUpdateCDROM";
	progress = 0;
	progress = p;
	void* ptr = (void*)&SHUpdateCDROMCallBackFunc;

	/*CHAR* tmpPath = (CHAR*)malloc(256);
	memset(tmpPath, 0, MAX_PATH);
	strcpy(tmpPath, isopath);*/
	Char * tmpPath = "G:\\ydxyTD.iso";
	
	printf("tmpPath = %s\n" , tmpPath);
	printf("hInitHandle = %d\n" , hInitHandle);
	printf("ptr = %d\n" , ptr);

	bool result = shUpdateCDROM(hInitHandle, tmpPath, ptr, 1234);	
	printf("tmpPath = %s\n" , tmpPath);
	if (!result) {
		printError(hInitHandle, proc);
		return false;
	}
	printf("update cdrom finish\n");
	progress = 0;
	return result;
}

char* FSManager::getDriveList() {
	proc = "shGetDriveList";
	bool result = shGetDriveList(hInitHandle, szDrives);
	if (!result) {
		printError(hInitHandle, proc);
		return false;
	}
	printf("CSHGetDriveList: %s ! \n", szDrives);
	return szDrives;
}

TFSFileHandle FSManager::openFileHandle(char * filepath, bool isCreate) {
	checkIfFileDirectoryExist(filepath);
	wchar_t* wc = charToWchar(filepath);
	proc = "shopenFileHandle";
	TFSFileHandle fileHandle = shCreateFile(
		hInitHandle, wc,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0
	);
	if (fileHandle != (TFSFileHandle)INVALID_HANDLE_VALUE) {
		// printf("CSHopenFileHandle Success! hFileHandle:%.8x \n", fileHandle);
		return fileHandle;
	}
	else {
		proc = "shCreateFile";
		DWORD err = printError(hInitHandle, proc);
		if (err == 404 && isCreate) {
			// printf("file to open doesn't exist , creating a new file...\n");
			fileHandle = shCreateFile(
				hInitHandle, wc,
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				0
			);
			if (fileHandle != (TFSFileHandle)INVALID_HANDLE_VALUE) {
				printf("CSHCreateFile Success! hFileHandle:%.8x \n", fileHandle);
				return fileHandle;
			}
		}
	}
	return fileHandle;
}

bool FSManager::checkIfFileDirectoryExist(char * path) {
	const char * sp = "\\";
	long spl = strlen(sp);

	char * tmp = (char *)malloc(256);
	memset(tmp, 0, 256);
	memcpy(tmp, path, strlen(path));

	char * p = strtok(tmp, sp); //??·???ֽ?Ϊÿ??Ŀ¼??
	

	char lastP[MAX_PATH] = { 0 };
	memset(lastP, 0, MAX_PATH);
	strcpy(lastP, "0:\\");

	char tmpstr[MAX_PATH] = { 0 };
	
	bool  isRoot = true;
	p = strtok(NULL, sp);
	if (p != NULL) {
		strcpy(tmpstr, p);
	}		
	p = strtok(NULL, sp);
	while (p != NULL) {		
		strcat(lastP, tmpstr);
		//strcat(lastP, "\\");
		//printf("%s\n", lastP);
		wchar_t* wc = charToWchar(lastP);
		DWORD attr = shGetFileAttr(hInitHandle,wc);
		//printf("attr of %s is %d\n", lastP, attr);
		if (attr == FILE_ATTRIBUTE_DIRECTORY) {
			//folder
		}
		else if (attr == -1) {
			//not exist		
			bool result = shCreateDirectory(hInitHandle, wc, NULL);
			if (!result) {
				printError(hInitHandle, "create directory");
			}
		}	
		strcat(lastP, "\\");
		strcpy(tmpstr, p);				
		p = strtok(NULL, sp);
	}
	return true;
}


bool FSManager::writeFileByBytes(char * filepath, unsigned char * content, unsigned long  length) {
	pthread_mutex_lock(&mutex);
	TFSFileHandle fileHandle = openFileHandle(filepath, true);	
	bool result = false;
	if (fileHandle) {
		DWORD dwNumOfBytesWritten;
		bool result = shWriteFile(hInitHandle, fileHandle, content, length, &dwNumOfBytesWritten, NULL);		
		proc = "shWriteFile";
		if (result) {
			/*long ptr = shSetFilePtr(hInitHandle, fileHandle, length, NULL, FILE_BEGIN);
			if (ptr == INVALID_SET_FILE_POINTER) {
				printError(hInitHandle, proc);
			}*/
			result = shSetEndOfFile(hInitHandle, fileHandle);
		}
		else {
			printError(hInitHandle, proc);
		}
	}

	result = shFlushFileBuffers(hInitHandle, fileHandle);
	result = shCloseFile(hInitHandle, fileHandle);
	pthread_mutex_unlock(&mutex);
	if (result) {
		return true;
	}
	return false;
}

bool FSManager::writeFileContent(char * filepath, unsigned char * content, unsigned long length,  long startP, bool isEof) {
	pthread_mutex_lock(&mutex);
	TFSFileHandle filePtr = openFileHandle(filepath, true);
	bool result = false;
	if (filePtr == 0) {
		pthread_mutex_unlock(&mutex);
		return false;
	}
	DWORD dwNumOfBytesWritten;
	unsigned long fsize = getFileSizeWithHandle(filepath, filePtr);
	if (startP == -1) {
		startP = fsize;
	}
	else if (startP > fsize) {
		startP = fsize;
	}
	long ptr = shSetFilePtr(hInitHandle , filePtr, startP , NULL , FILE_BEGIN);
	proc = "shSetFilePtr";
	if (ptr == INVALID_SET_FILE_POINTER) {
		printError(hInitHandle, proc);
		pthread_mutex_unlock(&mutex);
		return false;
	}
	unsigned long BytesWritten = 0;
	proc = "shWriteFile";
	result = shWriteFile(hInitHandle, filePtr, content, length, &BytesWritten, NULL);
	if (!result)
	{
		printError(hInitHandle, proc);
		pthread_mutex_unlock(&mutex);
		return false;
	}
	if (isEof) {
		proc = "shSetFilePtr";
		ptr = shSetFilePtr(hInitHandle , filePtr, startP + length , NULL , FILE_BEGIN);
		if (ptr == INVALID_SET_FILE_POINTER) {
			printError(hInitHandle, proc);
		}
		proc = "shSetEndOfFile";		
		result = shSetEndOfFile(hInitHandle, filePtr);
		if (!result) {
			printError(hInitHandle, proc);
		}
	}
	result = shCloseFile(hInitHandle, filePtr);
	pthread_mutex_unlock(&mutex);
	if (result) {
		//printf("file closed\n");
		return true;
	}
	return false;

}

unsigned char * FSManager::readFile(char * filepath) {
	pthread_mutex_lock(&mutex);
	DWORD fileSize;
	unsigned char * tmpBuffer;
	unsigned char * readBuffer;
	DWORD bytesRead, bytesToRead;
	//bool result = openFileHandle(filepath, true);	
	TFSFileHandle fileHandle = openFileHandle(filepath, true);
	if (fileHandle == (TFSFileHandle)INVALID_HANDLE_VALUE) {
		pthread_mutex_unlock(&mutex);
		return 0;
	}	
	if (fileHandle) {
		fileSize = getFileSizeWithHandle(filepath, fileHandle);
		readBuffer = (unsigned char *)malloc(fileSize);
		ZeroMemory(readBuffer, fileSize);
		bytesToRead = fileSize;
		bytesRead = 0;
		tmpBuffer = readBuffer;
		do {
			shReadFile(hInitHandle, fileHandle, (Byte*)tmpBuffer, bytesToRead, &bytesRead, NULL);
			if (bytesRead == 0) {
				break;
			}
			bytesToRead -= bytesRead;
			tmpBuffer += bytesRead;
		} while (bytesRead > 0);
	}
	bool result = shCloseFile(hInitHandle, fileHandle);
	pthread_mutex_unlock(&mutex);
	return readBuffer;
}

std::vector<unsigned char> FSManager::readFileWithVector(char * filepath) {
	unsigned char * readBuffer = readFile(filepath);	
	DWORD fileSize = getFileSize(filepath);	
	std::vector<unsigned char> v(fileSize);
	if (fileSize > 0) {
		memcpy(&v[0], readBuffer, fileSize);
	}
	return v;
}

std::vector<unsigned char> FSManager::readFileContent(char * filepath, unsigned long start, unsigned long length) {
	pthread_mutex_lock(&mutex);
	DWORD fileSize;
	unsigned char * tmpBuffer;
	unsigned char * readBuffer;
	DWORD bytesRead, bytesToRead;
	TFSFileHandle fileHandle = openFileHandle(filepath, true);

	fileSize = getFileSizeWithHandle(filepath, fileHandle);
	readBuffer = (unsigned char *)malloc(fileSize);
	ZeroMemory(readBuffer, fileSize);
	if (fileHandle) {
		bytesToRead = fileSize;
		bytesRead = 0;
		tmpBuffer = readBuffer;
		long nRet = shSetFilePtr(hInitHandle, fileHandle, start, NULL, FILE_BEGIN);
		proc = "shSetFilePtr";
		if (nRet!=INVALID_SET_FILE_POINTER) {
			printError(hInitHandle, proc);
			std::vector<unsigned char> v(0);
			pthread_mutex_unlock(&mutex);
			return v;
		}

		do {
			shReadFile(hInitHandle, fileHandle, (Byte*)tmpBuffer, bytesToRead, &bytesRead, NULL);
			if (bytesRead == 0) {
				break;
			}
			bytesToRead -= bytesRead;
			tmpBuffer += bytesRead;
		} while (bytesRead > 0);
	}
	bool result = shCloseFile(hInitHandle, fileHandle);	
	std::vector<unsigned char> v(fileSize);
	if (fileSize > 0) {
		memcpy(&v[0], readBuffer, fileSize);
	}
	pthread_mutex_unlock(&mutex);
	return v;
}

bool FSManager::deleteFile(char * filepath) {
	wchar_t* wc = charToWchar(filepath);
	bool result = shDeleteFile(hInitHandle, wc);
	proc = "shDeleteFile";	
	if (!result && shGetLastError(hInitHandle) != 404) {		
		TFileHandle filePtr = openFileHandle(filepath, false);
		if (filePtr != (TFSFileHandle)INVALID_HANDLE_VALUE && filePtr != 0) {
			//?ļ???ռ???У??????ر???ɾ??
			result = shCloseFile(hInitHandle, filePtr);
			if (result) {
				result = shDeleteFile(hInitHandle, wc);
				if (result) {
					return true;
				}
			}
		}
		printError(hInitHandle, proc);
	}	
	return result;
}

void FSManager::findFiles(char * pattern, vector<string> * vec) {
	TFSFindData lpFindFileData;
	wchar_t* wc = charToWchar(pattern)	;
	proc = "shFindFile";
	TFSHandle hFind = shFindFirstFile(hInitHandle, wc, &lpFindFileData);
	if (hFind == (TFSHandle)INVALID_HANDLE_VALUE) {
		printError(hInitHandle, proc);
	}
	else if (hFind != (TFSHandle)INVALID_HANDLE_VALUE) {
		char szFileName[MAX_PATH] = { 0 };
		WideCharToMultiByte(CP_ACP, NULL, (wchar_t*)lpFindFileData.sName, wcslen((wchar_t*)lpFindFileData.sName) + 1, szFileName, MAX_PATH, 0, 0);
		vec->push_back(szFileName);
		while (shFindNextFile(hInitHandle, hFind, &lpFindFileData)) {						
			if (lpFindFileData.iAttribute != FILE_ATTRIBUTE_DIRECTORY) {			
				char szFileName[MAX_PATH] = { 0 };
				WideCharToMultiByte(CP_ACP, NULL, (wchar_t*)lpFindFileData.sName, wcslen((wchar_t*)lpFindFileData.sName) + 1, szFileName, MAX_PATH, 0, 0);
				printf("find file : %s\n", szFileName);
				vec->push_back(szFileName);
			}			
		}
		bool result = shFindClose(hInitHandle, hFind);
		proc = "shFindClose";
		if (!result) {
			printError(hInitHandle, proc);
		}
	}
}


bool FSManager::createFolder(char * path) {
	printf("FSManager createFolder : %d\n", path);
	wchar_t* wc = charToWchar(path);
	proc = "shCreateDirectory";
	bool result = shCreateDirectory(hInitHandle, wc, NULL);
	if (!result) {
		DWORD dwLastError = shGetLastError(hInitHandle);
		if (dwLastError == 403) {
			return true;
		}
		printError(hInitHandle, proc);
	}
	return result;
}

bool FSManager::deleteFolder(char * path) {
	vector<char*> folderVec;
	folderVec.push_back(path);
	/*??ǰĿ¼?Ƿ???Ŀ¼*/
	bool isPushed = false;
	/*??ǰĿ¼?Ƿ?ɾ???ɹ?*/
	bool isDeleted = false;
	proc = "shRemoveDirectory";
	TFSHandle hFind = 0;
	bool result = false;
	while (folderVec.size() > 0) {
		if (isDeleted && isPushed) {
			folderVec.pop_back();
			isDeleted = false;
			isPushed = false;
			continue;
		}
		char curFolder[256] = { 0 };
		for (int i = 0; i < folderVec.size(); i++) {
			//ƴ????Ŀ¼·??
			strcat(curFolder, folderVec[i]);
		}
		//printf("scan folder : %s\n", curFolder);
		wchar_t* wc = charToWchar(curFolder);			
		result = shRemoveDirectory(hInitHandle, wc);		
		//printf("delete directory %ls  ,err code = %d\n",wc , shGetLastError(hInitHandle));
		if (result || shGetLastError(hInitHandle) == 404) {
			//Ŀ¼??Ϊ??/?????ڵĳ??ϣ?ɾ???ɹ???ɾ????һ??Ŀ¼??
			isPushed = false;
			isDeleted = false;
			folderVec.pop_back();
			continue;			
		}
		TFSFindData lpFindFileData;
		char pattern[256] = { 0 };
		strcpy(pattern, curFolder);
		strcat(pattern, "\\*");
		wchar_t* pwc = charToWchar(pattern);
		hFind = shFindFirstFile(hInitHandle, pwc, &lpFindFileData);
		if (hFind == (TFSHandle)INVALID_HANDLE_VALUE) {
			printError(hInitHandle, proc);
		}
		else {		
			do {
				char szFileName[MAX_PATH] = { 0 };
				WideCharToMultiByte(CP_ACP, NULL, (wchar_t*)lpFindFileData.sName, wcslen((wchar_t*)lpFindFileData.sName) + 1, szFileName, MAX_PATH, 0, 0);
				char * tmpstr = new char[MAX_PATH]();
				strcat(tmpstr, "\\");
				strcat(tmpstr, szFileName);
				char fileToDel[MAX_PATH] = { 0 };
				strcpy(fileToDel, curFolder);
				strcat(fileToDel, tmpstr);
				//printf("file %s scanned.\n", fileToDel);

				if (lpFindFileData.iAttribute != FILE_ATTRIBUTE_DIRECTORY) {
					wchar_t* dwc = charToWchar(fileToDel);
					bool tmp = shDeleteFile(hInitHandle, dwc);
					result = tmp && true;
					if (tmp) {
						//printf("file deleted : %s\n", fileToDel);
					}
					else {
						printf("file not deleted : %s\n", fileToDel);
					}
					//shFlushFileBuffers(hInitHandle, hFind);
				}
				else {
					wchar_t* dwc = charToWchar(fileToDel);
					bool tmp = shRemoveDirectory(hInitHandle, dwc);
					if (tmp) {
						isPushed = false;
						isDeleted = true;
					}
					else {
						//printf("delete folder %s failed , error code = %d\n", tmpstr, shGetLastError(hInitHandle));
						folderVec.push_back(tmpstr);
						isPushed = true;
						isDeleted = false;		
						//shFindClose(hInitHandle, hFind);
						break;
					}
				} 
			} while (shFindNextFile(hInitHandle, hFind, &lpFindFileData));
			if (hFind != 0) {
				result = shFindClose(hInitHandle, hFind);
				if (!result) {
					printf("find close failed , error code = %d\n", shGetLastError(hInitHandle));
				}
			}
			//cleanCache();
		}
		
	}
	printf("folder %s deleted.\n", path);
	return true;
}

unsigned long FSManager::getFileSizeWithHandle(char * filepath , TFSHandle file) {	
	DWORD fsize = shGetFileSize(hInitHandle, file, NULL);
	printf("fsize = %d\n", fsize);
	return fsize;
}

unsigned long FSManager::getFileSize(char * filepath) {
	pthread_mutex_lock(&mutex);
	TFSFileHandle fileHandle = openFileHandle(filepath, false);
	if (!fileHandle || fileHandle == (TFSFileHandle)INVALID_HANDLE_VALUE) {
		pthread_mutex_unlock(&mutex);
		return 0;
	}
	DWORD fsize = shGetFileSize(hInitHandle, fileHandle, NULL);
	printf("fsize = %d\n", fsize);
	bool result = shCloseFile(hInitHandle, fileHandle);
	pthread_mutex_unlock(&mutex);
	proc = "shCloseFile";	
	if (!result) {
		printError(hInitHandle, proc);
	}
	return fsize;
}

char* FSManager::getDiskInfo() {
	//return "3D43345ECF56FADC";
	char * sn = new char[17]();
	memset(sn, 0, 17);
	bool result = shDeviceNumber(hInitHandle, sn);
	proc = "shDeviceNumber";
	if (!result) {
		printError(hInitHandle, proc);
		return "";
	}
	else {
		printf("UDisk device serial number : %s\n", sn);
	}
	return sn;
}

bool FSManager::formatDisk() {
	printf("???ͺŵ?u?̲???????һ????");
	return false;
}

bool FSManager::changePassWord(char * pw) {
	bool result = shChangePwd(hInitHandle, pw);
	proc = "shChangePwd";
	if (!result) {
		printError(hInitHandle, proc);
	}
	printf("changePassWord to %s\n",  pw);
	return result;
}

bool FSManager::flush() {
	return true;
}

bool FSManager::cleanCache() {
	bool result = shFlushBuffers(hInitHandle);
	proc = "shFlushBuffers";
	if (!result) {
		printError(hInitHandle, proc);
	}
	return result;
}

bool FSManager::close() {
	bool result = shClose(hInitHandle);
	proc = "shClose";
	if (!result) {
		printError(hInitHandle, proc);
	}
	printf("U???豸?ѹر?");
	return result;
}

bool FSManager::dispose() {
	bool result = close();
	result = shFinal(hInitHandle);
	proc = "shFinal";
	FreeLibrary(hinstLib);
	if (!result) {
		printError(hInitHandle, proc);
	}
	printf("U?̹??????ͷ?");
	return result;
}



