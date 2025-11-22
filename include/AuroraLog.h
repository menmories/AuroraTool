#ifndef __AURORALOG_H__
#define __AURORALOG_H__

#include <string>

#define LOG_WARNING 0x01
#define LOG_ERROR   0x02

class AuroraLog
{
public:
    AuroraLog();
    ~AuroraLog();

    static int Println(const std::string& log);

    static int Println(int type, std::string& log);

    static void PrintToFile(const std::string& log);

private:
    static int _WriteToFile(const std::string& log);
};

#endif  //__AURORALOG_H__

