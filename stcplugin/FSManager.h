#pragma once
#include "IUdisk.h"
#include "FSShell.h"
#include <string>
#include <vector>

class FSManager :
	public IUDisk
{
private:
	/*��ʼ��*/PFSHInit            shInit;
	/*�ͷ�SDK*/PFSHFinal			shFinal;
	/*����*/PFSHGetLastError    shGetLastError;

	/*��ȡ�豸�б�*/PFSHGetDriveList    shGetDriveList;
	/*���豸*/PFSHOpen            shOpen;
	/*�ر��豸*/PFSHClose           shClose;
	/*��ȡ�ѹ��ڵķ���ID�б�*/PFSHGetMountedIDList        shGetMountedIDList;
	/*����CDROM��*/PFSHUpdateCDROM				shUpdateCDROM;
	/*������*/PFSHChangePwd       shChangePwd;
	/*��ʽ��*/PFSHFormat          shFormat;
	/*�����������*/PFSHFlushBuffers    shFlushBuffers;
	/**��ȡ���к�*/PFSHGetDeviceSerialNumber shDeviceNumber;

	/*��ȡ�����ܿռ�*/PFSHGetDeviceTotalSize		shGetDeviceTotalSize;
	/*��ȡ����ʣ��ռ�*/PFSHGetDiskFreeSpace        shGetDiskFreeSpace;

	/*�½��ļ�*/PFSHCreateFile      shCreateFile;
	/*д���ļ�*/PFSHWriteFile       shWriteFile;
	/**�����ļ���β*/PFSHSetEndOfFile shSetEndOfFile;
	/*��ȡ�ļ�*/PFSHReadFile		shReadFile;
	/*ˢ���ļ�����*/PFSHFlushFileBuffers        shFlushFileBuffers;
	/*�����ļ�д��*/PFSHCloseHandle     shCloseFile;
	/*ɾ���ļ�*/PFSHDeleteFile      shDeleteFile;
	/*��ȡ�ļ���С*/PFSHGetFileSize     shGetFileSize;
	/*�����ļ�ָ��*/PFSHSetFilePointer  shSetFilePtr;
	/**��ȡ�ļ�����*/PFSHGetFileAttributes shGetFileAttr;

	/*�����ļ�*/PFSHFindFirstFile   shFindFirstFile;
	/*������һ���ļ�*/PFSHFindNextFile    shFindNextFile;
	/*��������*/PFSHFindClose       shFindClose;

	/*�½��ļ���*/PFSHCreateDirectory shCreateDirectory;
	/*ɾ���ļ���*/PFSHRemoveDirectory shRemoveDirectory;

	CHAR * proc = "";
	DWORD printError(UINT id, char * description);
	wchar_t*  charToWchar(char * value);
	bool getMountedList();
	bool checkIfFileDirectoryExist(char * path);
	TFSFileHandle openFileHandle(char * filepath, bool isCreate);
	unsigned long getFileSizeWithHandle(char * filepath, TFSHandle file);
	char * brand = "Seczure";

	char * stringResult;

public:
	FSManager();
	~FSManager();
	static int * progress;
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
};

