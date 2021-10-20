#pragma once
#include "BaseAll.h"
class RSATool : public CBaseAll
{
private:
	CK_OBJECT_HANDLE priKey;
	CK_OBJECT_HANDLE pubKey;
	CK_RV errCode;		
	CK_MECHANISM ckMechanism = { CKM_RSA_PKCS, NULL_PTR, 0 };
	CK_ULONG keyType = CKK_RSA;
	bool inited = false;	
	bool GenerateKey();
public:
	RSATool(char* dll_file_path , char * pw);
	virtual ~RSATool();
	unsigned char * getCertId();
	bool ReadPublicKey();
	bool ReadPrivateKey();
	bool Verify(unsigned char * cert);
	unsigned char * Encrypt(unsigned char * msgToEncrypt, unsigned long len, unsigned long * cipherLen,bool isB64 = true);
	unsigned char * Decrypt(unsigned char * buffer, unsigned long bufferLen, unsigned long * decodedLen , bool isB64 = true);
	
	bool Dispose();
	unsigned long getErrorCode();

};



