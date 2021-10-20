package main

import (
	"log"
	"time"
	"stcplugin"
	"os"
)

var process int = 0

var timer = time.NewTicker(3 * time.Second)

func main() {
	
	isoPath := "D:\\test.iso"

	args := os.Args
	if len(args) > 1{
		isoPath = args[1]
	}
	udisk := stcplugin.UDiskFactoryGetUDisk(stcplugin.UDiskFactoryMODEL_TECLAST_SMI)

	udisk.Init()
	udisk.OpenDevice()
	return

	sNumber := udisk.GetDiskInfo()
	log.Println("---------------------------")
	log.Println("        U盘序列号          ")
	log.Println(sNumber)
	log.Println("---------------------------\n\n")

	return

	log.Println("开始刷入iso，请稍候...")
	log.Println("iso路径 ： " , isoPath)
	go checkIsoProcess()
	r := udisk.UpdateCDROM(isoPath , &process)
	if timer != nil {
		timer.Stop()
	}

	if !r{
		log.Println("刷入iso失败，请确认U盘状态并重新尝试")
	}else{
		log.Println("刷入iso成功，请拔出该U盘，插入另一U盘再运行本程序")
	}

}

func checkIsoProcess(){
	for range timer.C {
		log.Println("刷入进度 ： " , process)
	}
}