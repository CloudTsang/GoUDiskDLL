Golang操作读写U盘测试工程   
使用SWIG桥接？golang和c++，golang调c++接口再调dll方法实现定制加密u盘的读写。本repo删去了很多项目文件不能正常运行，只是个代码存档和参考。   
研发过程中一共接入了5、6款不同厂家主控的u盘，考虑厂方提供的工具完备程度、读写速度稳定性选择适合产品的u盘。   
实际SWIG由于厂方u盘dll会使用不同的visual studio/visual c++的版本或配置打包，会出各种问题，我都是把新建dll工程把原dll重新封装一遍（顺便筛选下不需要的接口）再使用。而golang通过`import "C"` 和 `syscall.GetProcAddress` 的方法直接调用dll里的方法似乎没有该问题，但是厂方基本都提供了c++工程里面就有可用代码，golang直接调dll也会有数据类型转换的困惑而没有使用。

    //c++读取u盘dll方法
    hinstLib = LoadLibrary(TEXT("FSShell"));
	if (hinstLib != NULL) {
		shInit = (PFSHInit)GetProcAddress(hinstLib, "SHInit");
		shFinal = (PFSHFinal)GetProcAddress(hinstLib, "SHFinal");
		shGetLastError = (PFSHGetLastError)GetProcAddress(hinstLib, "SHGetLastError");
    }
    //读写数据的代码由于接口参数类型等，每家u盘有些差别。


    //golang调c++初始化U盘
    udisk := stcplugin.NewFSManager()
	udisk.Init()
	udisk.OpenDevice()
    
    //golang调c++读写U盘
    content := []byte("11111111111")
	udisk.WriteFileByBytes(filePath , &content[0] , uint64(len(content)))	
	readResult := udisk.ReadFileWithVector(filePath)
	file_by := make([]byte, readResult.Size())
	for i := 0; i < int(readResult.Size()); i++ {
		file_by[i] = readResult.Get(i)
	}
	log.Println("ReadFile : " , string(file_by[:]))


此外为了减少频繁读写u盘，使用了boltdb，u盘里其实是boltdb数据库文件，使用时写出到本地再读写产品业务数据。


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