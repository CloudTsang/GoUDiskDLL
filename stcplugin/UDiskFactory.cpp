#include "stdafx.h"
#include "UDiskFactory.h"
//#include "UDiskManager.h"
#include "FSManager.h"
#include "TDManagerSMI.h"
IUDisk * UDiskFactory::getUDisk(int model) {
	switch (model) {
	case MODEL_FS:
		return new FSManager;	
	case MODEL_TECLAST_SMI:
		return new TDManagerSMI();
	default:
		return 0;
	}
}
