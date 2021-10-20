#pragma once
#include "IUdisk.h"
class UDiskFactory
{
public:
	static IUDisk * getUDisk(int model);
	
	static const int MODEL_SIM = 0;
	static const int MODEL_FS = 1;	
	static const int MODEL_SMART = 2;
	static const int MODEL_SMARTGM = 3;
	static const int MODEL_TECLAST = 4;
	static const int MODEL_TECLAST_SMI = 5;
	
};

