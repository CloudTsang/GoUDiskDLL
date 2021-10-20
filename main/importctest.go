package main

import (
	"log"
	"stcplugin"
	"os"
	"time"
)
import "C"

var process int = 0



func main() {
	
	isoPath := "D:\\ydxy.iso"

	args := os.Args
	if len(args) > 1{
		isoPath = args[1]
	}
	udisk := stcplugin.NewTDManagerSMI()//
	// udisk := stcplugin.NewFSManager()
	// udisk := stcplugin.UDiskFactoryGetUDisk(stcplugin.UDiskFactoryMODEL_FS)
	udisk.Init()
	udisk.OpenDevice()

	// udisk.CreateFolder("0:\\folder2")
	// udisk.DeleteFolder("folder1")
	// return	

	sNumber := udisk.GetDiskInfo()
	log.Println("---------------------------")
	log.Println("        U盘序列号          ")
	log.Println(sNumber)
	log.Println("---------------------------\n\n")

	t1 := time.Now()
	fSize := udisk.GetFileSize("stcData\\examQuesList.json");
	fSize = udisk.GetFileSize("stcData\\examQuesList.json");
	fSize = udisk.GetFileSize("stcData\\examQuesList.json");
	fSize = udisk.GetFileSize("stcData\\examQuesList.json");
	fSize = udisk.GetFileSize("stcData\\examQuesList.json");	
	t2 := time.Since(t1)
	log.Println("fSize = ", fSize, ",  time : ", t2);

	// t1 = time.Now()
	// fSize = udisk.GetFileSize("stcData\\examQuesListA.json");
	// t2 = time.Since(t1)
	// log.Println("fSize = ", fSize, ",  time : ", t2);

	return

	log.Println("开始刷入iso，请稍候...")
	log.Println("iso路径 ： " , isoPath)
	r := udisk.UpdateCDROM(isoPath , &process)

	if !r{
		log.Println("刷入iso失败，请确认U盘状态并重新尝试")
	}else{
		log.Println("刷入iso成功，请拔出该U盘，插入另一U盘再运行本程序")
	}

}
