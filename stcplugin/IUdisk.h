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
	/*��ʼ��SDK��ʵ����UDiskManager����������ִ��һ�θ÷���*/
	virtual bool init() = 0;

	/*���豸*/
	virtual bool openDevice(char * password = "123456") = 0;

	/**��¼iso*/
	//bool updateCDROM(char * isopath , CDRomCallback * cb);
	virtual bool updateCDROM(char * isopath, int * p = 0) = 0;

	/*��ȡu���̷�
	*@return ���� "IJ" �������̷��ַ���
	*/
	virtual char* getDriveList() = 0;

	/*���ļ�
	*@param filepath ���ļ�·��
	*@param isCreate �������ļ��ĳ����Ƿ񴴽��ļ�
	*/
	//virtual bool openFile(char * filepath, bool isCreate) = 0;

	/*д�����������
	* @param filepath д���ļ�·��
	* @param content д���ֽ�����ָ��
	* @param length д�볤��
	* @usage
	var b []byte
	stuplugins.WriteFileByBytes(filepath ,
	&b[0] ,   //ָ���ֽ�����ʼλ��
	uint64(len(content))) //ת��c++��unsigned long����
	*/
	virtual bool writeFileByBytes(char * filepath, unsigned char * content, unsigned long  length) = 0;

	virtual bool writeFileContent(char * filepath, unsigned char * content, 
		unsigned long length, long startP = -1, 
		bool isEof = true) { return true; };

	/*��ȡ�ļ�
	* @return ��ȡ������ݵ��ֽ���ָ�� *uint8
	* @usage ����һ���ļ�����д����һ�ļ�
	content := stuplugins.ReadFile("0:\\path.jpg")
	len := stuplugins.GetFileSize("0:\\path.jpg")
	stuplugins.WriteFileByBytes("0:\\target.jpg" , content , len)
	*/
	virtual unsigned char * readFile(char * filepath) = 0;

	/*��ȡ�ļ�
	* @return ��ȡ������ݵ�vector����
	* @usage ����һ���ļ�����д����һ�ļ�
	content := stuplugins.ReadFileWithVector("0:\\path.jpg")
	len := content.Size()
	barr := make([]byte , len)
	for i:=0 ; i<int(len) ; i++{
	barr[i] = content.Get(i)  //����vector��ֵ����byte[]
	}
	stuplugins.WriteFileByBytes("0:\\target.jpg" , &barr[0] , uint64(len))
	*/
	virtual std::vector<unsigned char> readFileWithVector(char * filepath) = 0;

	/*ɾ���ļ�*/
	virtual bool deleteFile(char * filepath) = 0;

	/*��ȡ�ļ���С*/
	virtual unsigned long getFileSize(char * filepath) = 0;

	/*�½��ļ���*/
	virtual bool createFolder(char * path) = 0;

	/*ɾ���ļ���*/
	virtual bool deleteFolder(char * path) = 0;

	/*Ѱ���ļ�
	* @param pattern ƥ��ģʽ����������·�����ļ���
	* @param vec ����������������
	* @usage
	vec := stuplugins.NewStringVector()  //����c++�ַ�����������
	stuplugins.FindFiles("0:\\*.txt" , vec)
	for i:=0 ; i<int(vec.Size()) ; i++{
	fmt.PrintIn("file name = " , vec.Get(int(i)))
	}
	*/
	virtual void findFiles(char * pattern, vector<string> * vec) = 0;

	/*��ȡU����Ϣ(δ���� , �������ݴ���)*/
	virtual char * getDiskInfo() = 0;

	/*��ʽ��(δ���ţ������Դ���)*/
	virtual bool formatDisk() = 0;

	/*������*/
	virtual bool changePassWord(char * pw) = 0;

	/*����ļ�����*/
	virtual bool flush() = 0;

	/*�建��*/
	virtual bool cleanCache() = 0;

	/*�ر��豸*/
	virtual bool close() = 0;

	/*�رչ����ͷ�SDK*/
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

