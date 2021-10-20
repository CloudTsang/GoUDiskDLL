set GODEBUG=cgocheck=0
set CGO_LDFLAGS= -g -O2 -Wl,--enable-stdcall-fixup -lAMUFDHlp -lcfgmgr32 -lcfgmgr32 -lpthread -L dll -L ../dll
go build -v -compiler gc -x -o plugintest.exe  main/plugintest.go
echo "plugin-testSMI.exe"

pause

 