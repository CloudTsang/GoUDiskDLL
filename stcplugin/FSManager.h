#pragma once
#include "IUdisk.h"
#include "FSShell.h"
#include <string>
#include <vector>

class FSManager :
	public IUDisk
{
private:
	/*初始化*/PFSHInit            shInit;
	/*释放SDK*/PFSHFinal			shFinal;
	/*报错*/PFSHGetLastError    shGetLastError;

	/*获取设备列表*/PFSHGetDriveList    shGetDriveList;
	/*打开设备*/PFSHOpen            shOpen;
	/*关闭设备*/PFSHClose           shClose;
	/*获取已挂在的分区ID列表*/PFSHGetMountedIDList        shGetMountedIDList;
	/*更新CDROM？*/PFSHUpdateCDROM				shUpdateCDROM;
	/*改密码*/PFSHChangePwd       shChangePwd;
	/*格式化*/PFSHFormat          shFormat;
	/*清除缓存数据*/PFSHFlushBuffers    shFlushBuffers;
	/**获取序列号*/PFSHGetDeviceSerialNumber shDeviceNumber;

	/*获取磁盘总空间*/PFSHGetDeviceTotalSize		shGetDeviceTotalSize;
	/*获取磁盘剩余空间*/PFSHGetDiskFreeSpace        shGetDiskFreeSpace;

	/*新建文件*/PFSHCreateFile      shCreateFile;
	/*写入文件*/PFSHWriteFile       shWriteFile;
	/**设置文件结尾*/PFSHSetEndOfFile shSetEndOfFile;
	/*读取文件*/PFSHReadFile		shReadFile;
	/*刷新文件缓存*/PFSHFlushFileBuffers        shFlushFileBuffers;
	/*结束文件写入*/PFSHCloseHandle     shCloseFile;
	/*删除文件*/PFSHDeleteFile      shDeleteFile;
	/*获取文件大小*/PFSHGetFileSize     shGetFileSize;
	/*设置文件指针*/PFSHSetFilePointer  shSetFilePtr;
	/**获取文件属性*/PFSHGetFileAttributes shGetFileAttr;

	/*查找文件*/PFSHFindFirstFile   shFindFirstFile;
	/*查找下一个文件*/PFSHFindNextFile    shFindNextFile;
	/*结束查找*/PFSHFindClose       shFindClose;

	/*新建文件夹*/PFSHCreateDirectory shCreateDirectory;
	/*删除文件夹*/PFSHRemoveDirectory shRemoveDirectory;

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

