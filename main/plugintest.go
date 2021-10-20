package main

import (
	"stcplugin"

)

func main() {
	udisk := stcplugin.NewFSManager()
	udisk.Init()
	udisk.OpenDevice()
	udisk.UpdateCDROM("G:\\TD.iso")

}