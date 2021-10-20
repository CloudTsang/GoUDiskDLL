package main

import (
	"callback"
	"log"
	"time"
	"stcplugin"
	"os"
	"strconv"
)

func main() {

	args := os.Args
	delay , _:= strconv.Atoi(args[1])
	log.Println("delay set to " , delay)
	ctest := stcplugin.UDiskFactoryGetUDisk(stcplugin.UDiskFactoryMODEL_FS)
	// ctest := stcplugin.UDiskFactoryGetUDisk(stcplugin.UDiskFactoryMODEL_SMARTGM)
	ctest.Init()
	ctest.OpenDevice()

	dev := ctest.GetDiskInfo()
	log.Println("Udisk serial number : ", dev)
	return
	ch := make(chan bool)

	filePath:="0:\\testfile.json"

	log.Println("写入文件")		
	content := []byte("11111111111")
	ctest.WriteFileByBytes(filePath , &content[0] , uint64(len(content)))	
	readResult := ctest.ReadFileWithVector(filePath)
	file_by := make([]byte, readResult.Size())
	for i := 0; i < int(readResult.Size()); i++ {
		file_by[i] = readResult.Get(i)
	}
	log.Println("ReadFile0 : " , string(file_by[:]))
	log.Println("\r\n")		



	go func(){
		log.Println("写入测试")		
		content2 :=[]byte("22222222222222222222222222222")
		ctest.WriteFileByBytes(filePath , &content2[0] , uint64(len(content2)))
		readResult = ctest.ReadFileWithVector(filePath)
		file_by = make([]byte, readResult.Size())
		for i := 0; i < int(readResult.Size()); i++ {
			file_by[i] = readResult.Get(i)
		}
		log.Println("ReadFile1 : " , string(file_by[:]))
		log.Println("\r\n")			
		ch <- true
	}()
	time.Sleep(time.Duration(delay)*time.Second)

	go func(){
		log.Println("读取测试")		
		// content3 := []byte("22222222222222222222222222222222222222222222222222222222222222")
		// ctest.WriteFileByBytes(filePath , &content3[0] , uint64(len(content3)))
		readResult = ctest.ReadFileWithVector(filePath)
		file_by = make([]byte, readResult.Size())
		for i := 0; i < int(readResult.Size()); i++ {
			file_by[i] = readResult.Get(i)
		}
		log.Println("ReadFile2 : " , string(file_by[:]))
		ch <- true
	}()



	<- ch
	<- ch

}

func TestCall() {
	c := stcplugin.NewCaller()
	cb := stcplugin.NewCallback()

	c.SetCallback(cb)
	s := c.Call()
	if s != "Callback::run" {
		log.Println("unexpected string from Call: ", s)
	}
	c.DelCallback()
}

func TestCallback() {
	c := stcplugin.NewCaller()
	cb := stcplugin.NewDirectorCallback(&callback.GoCallback{})
	c.SetCallback(cb)
	s := c.Call()
	if s != "GoCallback.Run" {
		log.Println("unexpected string from Call with callback:", s)
	}
	c.DelCallback()
	stcplugin.DeleteDirectorCallback(cb)
}
