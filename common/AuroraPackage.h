#ifndef __AURORAPACKAGE_H__
#define __AURORAPACKAGE_H__
#include <string>
#include "PlatformDef.h"

#define RECV_BUFFER_SIZE 1024 
#define AURORA_PACKAGE_HEADER_SIZE 9
#define AURORA_START_DATA  0xA5
#define AURORA_PACKAGE_SIZE 8192

enum class AuroraPackageType
{
	AuroraPackage_Msg,
	AuroraPackage_File,
};

class AuroraPackageHeader
{
public:
	AuroraPackageHeader();

	void MappingData(const unsigned char* buffer);
public:
	u8	Start;
	u16 Version;
	u16 Type;
	u32 Size;
};

class AuroraPackage
{
public:
	AuroraPackage();
	~AuroraPackage();

	/*
	* 从缓冲区接受一片数据
	* return bool
	* true  数据包接收完毕
	* false 数据包还未接收完毕
	*/
	bool RecvData(const u8* data, u32 len);

	void Reset();
public:
	bool IsHeaderRecvSuccess;
	std::string UserId;		//不用在写get set函数
	u32 RecvedSize;
	unsigned char* RecvBuffer;	//接收缓冲区，解包过后的实际的数据
	AuroraPackageHeader Header;
};

#endif // !__AURORAPACKAGE_H__
