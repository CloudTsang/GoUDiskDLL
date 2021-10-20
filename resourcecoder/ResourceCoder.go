package resourcecoder

import (
	//"archive/zip"
	//	"encoding/base64"
	"errors"
	"fmt"
	//"fmt"
	"boltdb"
	"crypt/des"
	//"crypt/rsa"
	//"crypto"
	//"yondor.com/stc/local/stcc"
	//"encoding/json"
	"encoding/binary"
	"math/rand"
	"stcplugin"
	"time"
)

const (
	bufferLen  int  = 256
	ISBASE64   bool = true
	CRYPT_RES  int  = 1
	CRYPT_DES  int  = 2
	CRYPT_3DES int  = 3
	CRYPT_AES  int  = 4
)

type ResourceCoder struct {
	con        *boltdb.BoltDbConnection
	udisk      stcplugin.IUDisk //u盘链接
	desKey     []byte
	BucketName string
}

func GetDecoder(db *boltdb.BoltDbConnection) (*ResourceCoder, error) {
	if db.Udisk() == nil {
		return nil, errors.New("没有u盘链接")
	}
	k, err := db.ReadData("key", "des")
	var k2 []byte
	if err != nil {
		fmt.Println(err)
		k2 = nil
	} else {
		dk := db.Udisk().RsaDecryptKey(k, uint64(binary.Size(k)), ISBASE64)
		dkstr := make([]byte, dk.Size())
		for i := 0; i < int(dk.Size()); i++ {
			dkstr[i] = dk.Get(i)
		}
		k2 = dkstr
	}
	coder := &ResourceCoder{db, db.Udisk(), k2, "resource"}
	return coder, nil
}

/**解密**/
func (coder *ResourceCoder) Decrypt(resId string) ([]byte, error) {
	if coder.desKey == nil {
		return nil, errors.New("没有储存到des密钥")
	}
	res, err := coder.con.ReadData(coder.BucketName, resId)
	if err != nil {
		return nil, errors.New("读取加密资源失败")
	}
	dres, err2 := des.DesDecrypt(res, coder.desKey)
	if err2 != nil {
		return nil, err2
	}
	return dres, nil

}

func (coder *ResourceCoder) DecryptWithKey(resId string, key []byte) ([]byte, error) {
	if coder.udisk == nil {
		return nil, errors.New("没有u盘链接")
	}
	dk := coder.udisk.RsaDecryptKey(&key[0], uint64(binary.Size(key)), ISBASE64)
	if dk.Size() == 0 {
		return nil, errors.New("解密对称密钥失败")
	}
	dkstr := make([]byte, dk.Size())
	for i := 0; i < int(dk.Size()); i++ {
		dkstr[i] = dk.Get(i)
	}
	fmt.Println("rsa decrypted des key : ", string(dkstr[:]))
	res, err := coder.con.ReadData(coder.BucketName, resId)
	if err != nil {
		return nil, errors.New("读取加密资源失败")
	}
	dres, err2 := des.DesDecrypt(res, dkstr)
	if err2 != nil {
		return nil, err2
	}
	return dres, nil
}

func (coder *ResourceCoder) SetLocalDesKey(key []byte) error {
	if coder.udisk == nil {
		return errors.New("没有u盘链接")
	}
	if coder.con == nil {
		return errors.New("没有boltdb文件")
	}
	if key == nil {
		key = gen8BitDesKey()
	}
	fmt.Println("generate local des key : ", string(key[:]))
	coder.desKey = key
	ekvec := coder.udisk.RsaEncryptKey(&key[0], uint64(binary.Size(key)), ISBASE64)
	ek := make([]byte, ekvec.Size())
	for i := 0; i < int(ekvec.Size()); i++ {
		ek[i] = ekvec.Get(i)
	}
	fmt.Println("rsa encrypted des key : ", string(ek[:]))
	w_err := coder.con.WriteData("key", "des", ek)
	return w_err
}

func gen8BitDesKey() []byte {
	str := "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
	bytes := []byte(str)
	result := []byte{}
	r := rand.New(rand.NewSource(time.Now().UnixNano()))
	for i := 0; i < 8; i++ {
		result = append(result, bytes[r.Intn(len(bytes))])
	}
	return result
}
