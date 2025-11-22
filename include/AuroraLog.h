#ifndef __AURORALOG_H__
#define __AURORALOG_H__

#include <string>
class AuroraLog
{
public:
    AuroraLog();
    ~AuroraLog();

    static int Println(const std::string& log);
};

#endif  //__AURORALOG_H__

