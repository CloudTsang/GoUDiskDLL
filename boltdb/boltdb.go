package boltdb

import (
	"encoding/binary"
	"errors"
	"github.com/boltdb/bolt"
	"io/ioutil"
	"log"
	"os"
	"stcplugin"
)

type BoltDbConnection struct {
	id    string           //db文件id
	path  string           //本地磁盘路径
	uPath string           //u盘路径
	con   *bolt.DB         //boltdb链接
	udisk stcplugin.IUDisk //u盘链接
}

func (db *BoltDbConnection) WriteData(bucket string, key string, value []byte) error {
	if db.con == nil {
		return errors.New("数据库链接不存在")
	}
	err := db.con.Update(func(tx *bolt.Tx) error {
		buc, w_err := tx.CreateBucketIfNotExists([]byte(bucket))
		if w_err != nil {
			return w_err
		}
		w_err = buc.Put([]byte(key), value)
		return w_err
	})
	return err
}

func (db *BoltDbConnection) ReadData(bucket string, key string) ([]byte, error) {
	if db.con == nil {
		return nil, errors.New("数据库链接不存在")
	}
	var res []byte
	err := db.con.View(func(tx *bolt.Tx) error {
		buc := tx.Bucket([]byte(bucket))
		if buc == nil {
			return errors.New("Bucket is not exists")
		}
		res = buc.Get([]byte(key))
		return nil
	})
	return res, err
}

func (db *BoltDbConnection) DeleteData(bucket string, key string) (bool, error) {
	if db.con == nil {
		return false, errors.New("数据库链接不存在")
	}
	err := db.con.Update(func(tx *bolt.Tx) error {
		buc := tx.Bucket([]byte(bucket))
		if buc == nil {
			return errors.New("Bucket不存在")
		}
		w_err := buc.Delete([]byte(key))
		return w_err
	})
	return true, err
}

/**将db文件保存到u盘*/
func (db *BoltDbConnection) Store() error {
	if db.udisk == nil {
		return errors.New("u盘链接不存在")
	}
	dbytes, r_err := ioutil.ReadFile(db.path)
	if r_err != nil {
		return r_err
	}
	//err := stcd.SaveFileDirect(db.uPath, dbytes)
	log.Println("database file size = ", uint64(binary.Size(dbytes)))
	boo := db.udisk.WriteFileByBytes(db.uPath, &dbytes[0], uint64(binary.Size(dbytes)))
	if !boo {
		//if err != nil {
		//return err
		return errors.New("创建的boltDb文件写入u盘失败")
	}
	return nil
}

/**关闭bolt链接
* @param isDel 是否删除本地db文件
 */
func (db *BoltDbConnection) Dispose(isDel bool) error {
	if db.con == nil {
		return errors.New("boltdb链接不存在")
	}
	err := db.con.Close()
	if err != nil {
		return err
	}
	if isDel {
		err = os.Remove(db.path)
	}

	db.con = nil
	return nil
}

/**删除U盘db文件*/
func (db *BoltDbConnection) Remove() error {
	if db.udisk == nil {
		return errors.New("u盘链接不存在")
	}
	boo := db.udisk.DeleteFile(db.uPath)
	if !boo {
		return errors.New("删除U盘db文件失败 或 U盘不存在这个数据库文件")
	}
	return nil
}

/**检查db文件是否已移动到本地*/
func (db *BoltDbConnection) CheckIsLocal() bool {
	_, err := os.Stat(db.path)
	if err != nil {
		return false
	}
	return true
}

func (db *BoltDbConnection) LocalPath() string {
	return db.path
}

func (db *BoltDbConnection) UdiskPath() string {
	return db.uPath
}
func (db *BoltDbConnection) Id() string {
	return db.id
}
func (db *BoltDbConnection) Udisk() stcplugin.IUDisk {
	return db.udisk
}

/**获取数据库连接，该方法从u盘的pathInUdisk路径复制db文件到电脑pathInLocal路径，或直接在pathInLocal创建db文件
* @param ud      				u盘读写对象
* @param pathInUdisk   			db文件的u盘路径
* @param pathInLocal			db文件的本地路径
* @param isCreateIfNotExist		db文件在u盘中不存在的场合是否在本地路径创建
 */
func GetBoltDb(ud stcplugin.IUDisk, pathInUdisk string, pathInLocal string, isCreateIfNotExist bool, id string) (*BoltDbConnection, error) {
	//从u盘读取db文件
	vec := ud.ReadFileWithVector(pathInUdisk)
	var db *bolt.DB
	var err error
	if vec.Size() == 0 {
		if isCreateIfNotExist {
			log.Println("在本地创建boltdb文件")
			//u盘不存在db文件的场合直接在本地创建
			//读取创建的db文件并写入u盘(???)
		} else {
			log.Println("boltdb文件不存在")
			//u盘不存在db文件的场合返回错误
			return nil, errors.New("U盘不存在这个数据库文件")
		}
	} else {
		log.Println("从u盘中获取boltdb文件")
		var barr []byte = make([]byte, vec.Size())
		for i := 0; i < int(vec.Size()); i++ {
			barr[i] = vec.Get(i) //遍历vector将值赋予byte[]
		}
		//读取到db文件的场合写入本地
		err = ioutil.WriteFile(pathInLocal, barr, os.ModePerm)
		if err != nil {
			return nil, err
		}
	}
	db, err = bolt.Open(pathInLocal, 0600, nil)
	if err != nil {
		return nil, err
	}
	con := &BoltDbConnection{id, pathInLocal, pathInUdisk, db, ud}
	return con, nil
}
