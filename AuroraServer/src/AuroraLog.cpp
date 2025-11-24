#include "AuroraLog.h"
#include <cstdio>
#include <ctime>
#include <cstdio>

std::string g_strLogFileName = "AuroraServer.log";
FILE* g_pLogFile;


AuroraLog::AuroraLog()
{
}

AuroraLog::~AuroraLog()
{
}

int AuroraLog::Println(const std::string& log)
{
	time_t nowtime;
	time(&nowtime); //获取1970年1月1日0点0分0秒到现在经过的秒数
	tm* p = localtime(&nowtime); //将秒数转换为本地时间,年从1900算起,需要+1900,月为0-11,所以要+1
	//printf("%04d:%02d:%02d %02d:%02d:%02d\n", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
    return printf("[%d-%02d-%02d %02d:%02d:%02d] %s\n", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, log.c_str());
}

int AuroraLog::Println(int type, std::string& log)
{
	int ret = 0;
	if (type == LOG_WARNING)
	{
		ret = Println(log);
	}
	else if(type == LOG_ERROR)
	{
		_WriteToFile(log);
	}
	else if ((type & LOG_WARNING) && (type & LOG_ERROR))
	{
#ifdef _DEBUG
		ret = Println(log);
#else
		ret = _WriteToFile(log);
#endif
	}
	return ret;
}

void AuroraLog::PrintToFile(const std::string& log)
{
	time_t nowtime;
	time(&nowtime);
	tm* p = localtime(&nowtime);
	FILE* pFile = fopen(g_strLogFileName.c_str(), "a+");
	if (!pFile)
	{
		return;
	}
	fprintf(pFile, "[%d-%02d-%02d %02d:%02d:%02d] %s\n", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec,
		log.c_str());
	fclose(pFile);
}

int AuroraLog::_WriteToFile(const std::string& log)
{
	int ret = 0;
	time_t nowtime;
	time(&nowtime);
	tm* p = localtime(&nowtime);
	FILE* pFile = fopen(g_strLogFileName.c_str(), "a+");
	if (!pFile)
	{
		return ret;
	}
	ret = fprintf(pFile, "[%d-%02d-%02d %02d:%02d:%02d] %s\n", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec,
		log.c_str());
	fclose(pFile);
	return ret;
}

