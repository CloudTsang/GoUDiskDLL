package main

import (
	"log"
	"time"
	"stcplugin"
	"os"
	"io/ioutil"
)

var process int = 0

var timer = time.NewTicker(3 * time.Second)
var leng int = 512
var udisk stcplugin.IUDisk
var mode = stcplugin.UDiskFactoryMODEL_TECLAST_SMI
var ch = make(chan bool, 10)
func main() {

	ty := "w"
	if len(os.Args)>1{
		ty = os.Args[1]
	}

	udisk = stcplugin.UDiskFactoryGetUDisk(mode)


	udisk.Init()
	udisk.OpenDevice("")	

	sNumber := udisk.GetDiskInfo()
	log.Println("U盘序列号:",sNumber)
	// return

	for i:=0; i<10; i++{
		go func(){
			WriteTest("D:\\testfile\\test_1050.jpg", "0:\\test_1050.jpg" , ty ,"1mb")			
		}()
		go func(){
			WriteTest("D:\\testfile\\test_1050.jpg", "0:\\test_1050.jpg" , "r" ,"1mb")
		}()
	}
	<-ch

	// WriteTest("D:\\testfile\\examDetail.db", "0:\\examDetail.db" , ty , "32kb")

	// WriteTest("D:\\testfile\\test_10.txt", "0:\\test_10.txt" , ty , "10kb")

	// WriteTest("D:\\testfile\\test_100.txt", "0:\\test_100.txt" , ty , "100kb")

	// WriteTest("D:\\testfile\\test_512.txt", "0:\\test_512.txt" , ty , "512kb")

	// WriteTest("D:\\testfile\\test_1050.jpg", "0:\\test_1050.jpg" , ty ,"1mb")

	// WriteTest("D:\\testfile\\test_2053.pdf", "0:\\test_2053.pdf" , ty , "2mb")

	// WriteTest("D:\\testfile\\test_3984.mp3", "0:\\test_3984.mp3" , ty ,"4mb")

	// WriteTest("D:\\testfile\\test_5084.gif", "0:\\test_5084.gif" , ty , "5mb")

	// WriteTest("D:\\testfile\\test_10227.mp3", "0:\\test_10227.mp3" , ty , "10mb")

	// WriteTest("D:\\testfile\\test_33400.epub", "0:\\test_33400.epub" , ty , "30mb")

	// WriteTest("D:\\testfile\\test_132416.zip", "0:\\test_132416.zip" , ty , "130mb")
	

	return
}


func WriteTest(src , tgt , ty string , si string){
	if mode == stcplugin.UDiskFactoryMODEL_TECLAST{
		if ty == "w"{
			t1 := time.Now()
			udisk.WriteTest(src, tgt);
			t2 := time.Since(t1)
			log.Println("Write Size : " , si , "   , Totle Time : ",t2)

		}else if ty=="r"{
			t1 := time.Now()
			udisk.ReadTest(tgt , src)
			t2 := time.Since(t1)
			log.Println("Read Size : " , si , "   , Totle Time : ",t2)
		}
	}else{
		if ty == "w"{
			by , _ := ioutil.ReadFile(src)
			size := len(by)

			t1 := time.Now()
			boo := udisk.WriteFileByBytes(tgt , &by[0] , uint64(size))
			t2 := time.Since(t1)
			log.Println("Write Size : " , si , "   , Totle Time : ",t2)

			if !boo{
				log.Println("WriteFailed")
			}
		}else if ty=="r"{
			t1 := time.Now()
			vec := udisk.ReadFileWithVector(tgt)
			t2 := time.Since(t1)
			log.Println("Read Size : " , si , "   , Totle Time : ",t2)

			log.Println(vec.Size() , " bytes read.")
			len := vec.Size()
			barr := make([]byte , len)
			for i:=0 ; i<int(len) ; i++{
				barr[i] = vec.Get(i)  //遍历vector将值赋予byte[]
			}
			err := ioutil.WriteFile(src+".bak" , barr , 0644)
			if err!=nil{
				log.Println(err.Error())
			}

		}

	}

}