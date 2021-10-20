#pragma once
//#define interface class __declspec(novtable)
#include <string>
#include <vector>
#include <fstream>  
using namespace std;

//interface IUDisk {
class IUDisk {
	
private :
	char * deviceid;
	char * brand;
	
public:
	//static int * progress;
	/*初始化SDK，实例化UDiskManager对象后请务必执行一次该方法*/
	virtual bool init() = 0;

	/*打开设备*/
	virtual bool openDevice(char * password = "123456") = 0;

	/**烧录iso*/
	//bool updateCDROM(char * isopath , CDRomCallback * cb);
	virtual bool updateCDROM(char * isopath, int * p = 0) = 0;

	/*获取u盘盘符
	*@return 形如 "IJ" 这样的盘符字符串
	*/
	virtual char* getDriveList() = 0;

	/*打开文件
	*@param filepath 打开文件路径
	*@param isCreate 不存在文件的场合是否创建文件
	*/
	//virtual bool openFile(char * filepath, bool isCreate) = 0;

	/*写入二进制数据
	* @param filepath 写入文件路径
	* @param content 写入字节流的指针
	* @param length 写入长度
	* @usage
	var b []byte
	stuplugins.WriteFileByBytes(filepath ,
	&b[0] ,   //指向字节流起始位置
	uint64(len(content))) //转换c++的unsigned long类型
	*/
	virtual bool writeFileByBytes(char * filepath, unsigned char * content, unsigned long  length) = 0;

	virtual bool writeFileContent(char * filepath, unsigned char * content, 
		unsigned long length, long startP = -1, 
		bool isEof = true) { return true; };

	/*读取文件
	* @return 读取获得数据的字节流指针 *uint8
	* @usage 读出一个文件内容写入另一文件
	content := stuplugins.ReadFile("0:\\path.jpg")
	len := stuplugins.GetFileSize("0:\\path.jpg")
	stuplugins.WriteFileByBytes("0:\\target.jpg" , content , len)
	*/
	virtual unsigned char * readFile(char * filepath) = 0;

	/*读取文件
	* @return 读取获得数据的vector类型
	* @usage 读出一个文件内容写入另一文件
	content := stuplugins.ReadFileWithVector("0:\\path.jpg")
	len := content.Size()
	barr := make([]byte , len)
	for i:=0 ; i<int(len) ; i++{
	barr[i] = content.Get(i)  //遍历vector将值赋予byte[]
	}
	stuplugins.WriteFileByBytes("0:\\target.jpg" , &barr[0] , uint64(len))
	*/
	virtual std::vector<unsigned char> readFileWithVector(char * filepath) = 0;

	/*删除文件*/
	virtual bool deleteFile(char * filepath) = 0;

	/*获取文件大小*/
	virtual unsigned long getFileSize(char * filepath) = 0;

	/*新建文件夹*/
	virtual bool createFolder(char * path) = 0;

	/*删除文件夹*/
	virtual bool deleteFolder(char * path) = 0;

	/*寻找文件
	* @param pattern 匹配模式，包含搜索路径与文件名
	* @param vec 存放搜索结果的数组
	* @usage
	vec := stuplugins.NewStringVector()  //创建c++字符串向量对象
	stuplugins.FindFiles("0:\\*.txt" , vec)
	for i:=0 ; i<int(vec.Size()) ; i++{
	fmt.PrintIn("file name = " , vec.Get(int(i)))
	}
	*/
	virtual void findFiles(char * pattern, vector<string> * vec) = 0;

	/*获取U盘信息(未开放 , 返回数据待定)*/
	virtual char * getDiskInfo() = 0;

	/*格式化(未开放，可用性待定)*/
	virtual bool formatDisk() = 0;

	/*改密码*/
	virtual bool changePassWord(char * pw) = 0;

	/*清除文件缓冲*/
	virtual bool flush() = 0;

	/*清缓存*/
	virtual bool cleanCache() = 0;

	/*关闭设备*/
	virtual bool close() = 0;

	/*关闭工具释放SDK*/
	virtual bool dispose() = 0;

	virtual char * getLastError() { return 0; };
	
	virtual vector<unsigned char> rsaDecryptKey(unsigned char * encrypted, unsigned long length, bool isB64 = true){return vector<unsigned char>();};
	virtual vector<unsigned char> rsaEncryptKey(unsigned char * key, unsigned long length, bool isB64 = true){return vector<unsigned char>();};

	virtual bool waitDev() {return false;};

	virtual bool WriteArea(unsigned long start, unsigned long length, unsigned char * content) { return false; };
	virtual vector<unsigned char>  ReadArea(unsigned long start, unsigned long length) { std::vector<unsigned char> v(0); return v; };

	virtual bool writeTest(char * srcPath, char * tgtPath){return false;};
	virtual bool readTest(char * srcPath, char * tgtPath){return false;};

};

