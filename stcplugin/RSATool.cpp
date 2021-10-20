#include "RSATool.h"
#include<windows.h>  
#include "common.h"
#include "Base64.h"

RSATool::RSATool(char* dll_file_path, char * pw ) :CBaseAll(dll_file_path)
{
	errCode = 0;
	
	if (CKR_OK != BaseAllStart(pw))
		return;	
	ReadPrivateKey();
	ReadPublicKey();
}

RSATool::~RSATool()
{
}

unsigned char * RSATool::getCertId()
{
	CK_OBJECT_CLASS certClass = CKO_CERTIFICATE;
	CK_ATTRIBUTE temp1[] = {
		{ CKA_CLASS  , &certClass  , sizeof(certClass)},
	};
	CK_RV rv = m_gToken->C_FindObjectsInit(hSession, temp1, countof(temp1));
	if (rv != CKR_OK) {
		errCode = rv;
		return 0;
	}
	CK_OBJECT_HANDLE cert = 0;
	CK_ULONG num = 0;
	rv = m_gToken->C_FindObjects(hSession, &cert, 1, &num);
	if (rv != CKR_OK) {
		errCode = rv;
		m_gToken->C_FindObjectsFinal(hSession);
		return 0;
	}
	CK_BYTE id[128] = {0};
	CK_BYTE label[128] = {0};
	CK_ATTRIBUTE temp2[] = {		
		{CKA_ID , id , sizeof(id)},
		{CKA_LABEL , label , sizeof(label)}
	};	
	m_gToken->C_GetAttributeValue(hSession, cert, temp2, countof(temp2));
	//printf("get attribute type : %d   ,   value : %s\n", temp2[0].type, temp2[0].pValue);
	//printf("get attribute type : %d   ,   value : %s\n", temp2[1].type, temp2[1].pValue);
	m_gToken->C_FindObjectsFinal(hSession);
	unsigned long len = temp2[1].ulValueLen + 1;
	unsigned char * ret = (unsigned char *)malloc(len);
	memset(ret, 0, len);
	memcpy(ret, temp2[1].pValue, len);
	printf("udisk id : %s\n", ret);
	return ret;
}

bool RSATool::ReadPublicKey()
{
	CK_BBOOL bTrue = TRUE;
	CK_OBJECT_CLASS pubClass = CKO_PUBLIC_KEY;
	CK_ULONG ulModulusBits = MODULUS_BIT_LENGTH;
	CK_ATTRIBUTE pubTemplate[] =
	{
		{ CKA_CLASS,		&pubClass,		sizeof(pubClass) },
		{ CKA_KEY_TYPE,		&keyType,		sizeof(keyType) },
		{ CKA_MODULUS_BITS,	&ulModulusBits, sizeof(ulModulusBits) },
		{ CKA_ENCRYPT,		&bTrue,			sizeof(bTrue) },
		{ CKA_TOKEN,		&bTrue,			sizeof(bTrue) },
	};
	CK_RV rv = m_gToken->C_FindObjectsInit(hSession, pubTemplate, countof(pubTemplate));
	if (rv != CKR_OK) {
		errCode = rv;
		return false;
	}
	while (1) {
		CK_ULONG pulObjectCount = 0;
		rv = m_gToken->C_FindObjects(hSession, &pubKey, 1, &pulObjectCount);
		if (rv != CKR_OK || pulObjectCount == 0) {
			break;
		}
	}
	if (pubKey == 0) {
		printf("Public key object not found!\n");
		errCode = rv;
	}
	rv = m_gToken->C_FindObjectsFinal(hSession);
	return false;
}

bool RSATool::ReadPrivateKey()
{		
	CK_BBOOL bTrue = TRUE;
	CK_OBJECT_CLASS priClass = CKO_PRIVATE_KEY;	
	CK_ATTRIBUTE priTemplate[] = {
		{ CKA_CLASS,			&priClass,	sizeof(priClass) },
		{ CKA_KEY_TYPE,		&keyType,	sizeof(keyType) },
		{ CKA_DECRYPT,		&bTrue,		sizeof(bTrue) },
		{ CKA_PRIVATE,		&bTrue,		sizeof(bTrue) },
		{ CKA_SENSITIVE,		&bTrue,		sizeof(bTrue) },
		{ CKA_TOKEN,			&bTrue,		sizeof(bTrue) },
	};

	CK_RV rv = m_gToken->C_FindObjectsInit(hSession, priTemplate, countof(priTemplate));
	if (rv != CKR_OK) {
		errCode = rv;
		return false;
	}
	while (1) {
		CK_ULONG         pulObjectCount = 0;
		rv = m_gToken->C_FindObjects(hSession, &priKey, 1, &pulObjectCount);
		if (rv != CKR_OK || pulObjectCount == 0) {			
			break;
		}			
	}
	if (priKey == 0) {
		printf("Private key object not found!\n");
		errCode = rv;
	}
	rv = m_gToken->C_FindObjectsFinal(hSession);
	errCode = 0;
	return true;
}

bool RSATool::Verify(unsigned char * cert)
{
	return true;
}

unsigned char * RSATool::Encrypt(unsigned char * msgToEncrypt  , unsigned long len, unsigned long * cipherLen, bool isB64) {
	if (hSession == 0 || pubKey == 0) {
		errCode = -1;
		return 0;
	}
	unsigned long encryptLen = 0;
	CK_RV rv = m_gToken->C_EncryptInit(hSession, &ckMechanism, pubKey);
	if (rv != CKR_OK) {
		errCode = rv;
		return 0;
	}
	rv = m_gToken->C_Encrypt(hSession, msgToEncrypt, len, NULL_PTR, &encryptLen);
	if (rv != CKR_OK) {
		errCode = rv;
		return 0;
	}
	unsigned char * cipherBuffer = (unsigned char *)malloc(encryptLen);
	rv = m_gToken->C_Encrypt(hSession, msgToEncrypt, len, cipherBuffer, &encryptLen);
	if (rv != CKR_OK) {
		errCode = rv;
		return 0;
	}
	if (isB64) {
		string b64 = base64_encode(cipherBuffer, encryptLen);
	//	printf("¼ÓÃÜÃÜÎÄ£º%s\n", b64.c_str());
		*cipherLen = (b64.length() + 1) * sizeof(char);
		char * ret = (char*)malloc(*cipherLen);
		memset(ret, 0, *cipherLen);
		b64.copy(ret, b64.length());
		//free(cipherBuffer);
		return (unsigned char *)ret;
	}	
	return cipherBuffer;
}

unsigned char * RSATool::Decrypt(unsigned char * buffer , unsigned long bufferLen , unsigned long * decodedLen , bool isB64 )
{
	if (hSession == 0 || priKey == 0) {
		errCode = -1;
		return 0;
	}
	unsigned char * result;
	CK_RV rv = m_gToken->C_DecryptInit(hSession,&ckMechanism, priKey);
	if (rv != CKR_OK) {
		errCode = rv;
		return 0;
	}
	unsigned char * cipher;
	unsigned long cipherLen = 0;
	if (isB64) {
		std::vector<unsigned char> vec = base64_decode((char * )buffer);
		cipher = &vec[0];
		cipherLen = vec.size();
	}
	else {
		cipher = buffer;
		cipherLen = bufferLen;
	}
	unsigned long dl = 0;	
	rv = m_gToken->C_Decrypt(hSession, cipher,
		cipherLen, NULL_PTR, &dl);
	if (rv != CKR_OK) {
		errCode = rv;
		//free(cipher);
		return 0;
	}
	result = (unsigned char *)malloc(dl +1);
	memset(result, 0, dl + 1);
	rv = m_gToken->C_Decrypt(hSession, cipher, cipherLen,
		result, &dl);
	if (rv != CKR_OK) {
		errCode = rv;
		return 0;
	}
	*decodedLen = dl;
	//free(cipher);
	return result;
}

bool RSATool::GenerateKey() {
	CK_OBJECT_HANDLE hPriKey = 0;
	CK_OBJECT_HANDLE hPubKey = 0;
	CK_RV rv = CKR_OK;
	CK_BBOOL bTrue = TRUE;
	CK_ULONG ulModulusBits = MODULUS_BIT_LENGTH;
	CK_ULONG keyType = CKK_RSA;
	CK_OBJECT_CLASS pubClass = CKO_PUBLIC_KEY;
	CK_ATTRIBUTE pubTemplate[] =
	{
		{ CKA_CLASS,			&pubClass,		sizeof(pubClass) },
		{ CKA_KEY_TYPE,		&keyType,		sizeof(keyType) },
		{ CKA_MODULUS_BITS,	&ulModulusBits, sizeof(ulModulusBits) },
		{ CKA_ENCRYPT,		&bTrue,			sizeof(bTrue) },
		{ CKA_TOKEN,			&bTrue,			sizeof(bTrue) },
		{ CKA_WRAP,			&bTrue,			sizeof(bTrue) },
	};
	CK_OBJECT_CLASS priClass = CKO_PRIVATE_KEY;
	CK_ATTRIBUTE priTemplate[] = {
		{ CKA_CLASS,			&priClass,	sizeof(priClass) },
		{ CKA_KEY_TYPE,		&keyType,	sizeof(keyType) },
		{ CKA_DECRYPT,		&bTrue,		sizeof(bTrue) },
		{ CKA_PRIVATE,		&bTrue,		sizeof(bTrue) },
		{ CKA_SENSITIVE,		&bTrue,		sizeof(bTrue) },
		{ CKA_TOKEN,			&bTrue,		sizeof(bTrue) },
		{ CKA_EXTRACTABLE,	&bTrue,		sizeof(bTrue) },
		{ CKA_UNWRAP,		&bTrue,		sizeof(bTrue) },
	};
	CK_MECHANISM keyGenMechanism = { CKM_RSA_PKCS_KEY_PAIR_GEN, NULL_PTR, 0 };
	CK_MECHANISM ckMechanism = { CKM_RSA_PKCS, NULL_PTR, 0 };
	rv = m_gToken->C_GenerateKeyPair(hSession, &keyGenMechanism,
		pubTemplate, countof(pubTemplate),
		priTemplate, countof(priTemplate),
		&hPubKey, &hPriKey);
	return rv == CKR_OK;
}


bool RSATool::Dispose()
{	
	CK_RV rv = m_gToken->C_DestroyObject(hSession, pubKey);
	rv = m_gToken->C_DestroyObject(hSession, priKey);
	rv = m_gToken->C_Logout(hSession);
	return true;
}
unsigned long RSATool::getErrorCode()
{
	return errCode;
}
;






