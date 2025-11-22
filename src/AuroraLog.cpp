#include "AuroraLog.h"
#include <cstdio>
#include <ctime>

AuroraLog::AuroraLog()
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

