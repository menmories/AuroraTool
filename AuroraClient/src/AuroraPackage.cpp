#include "AuroraPackage.h"
#include "AuroraLog.h"

AuroraPackage::AuroraPackage()
    : IsHeaderRecvSuccess(false)
    , RecvedSize(0)
{
	RecvBuffer = (u8*)malloc(AURORA_PACKAGE_SIZE);
}

AuroraPackage::~AuroraPackage()
{
	free(RecvBuffer);
}

bool AuroraPackage::RecvData(const u8* data, size_t len)
{
    size_t handedLen = 0;
    u32 needSize = 0;
    while (handedLen < len)
    {
        if (IsHeaderRecvSuccess)
        {
            if (Header.Type == (u16)AuroraPackageType::AuroraPackage_Msg)
            {
                //如果是普通聊天消息，发送的总数据长度不能超过10240个字节
                needSize = Header.Size - RecvedSize;
                if (needSize)
                {
                    if (needSize > len)
                    {
                        memcpy(RecvBuffer + RecvedSize, data + handedLen, len);
                        handedLen += len;
                        RecvedSize += len;
                    }
                    else
                    {
                        memcpy(RecvBuffer + RecvedSize, data + handedLen, needSize);
                        handedLen += len;
                        RecvedSize += len;
                    }
                }
                if (RecvedSize >= Header.Size)
                {
                    //接收完毕
                    std::string msg((char*)RecvBuffer, Header.Size);
                    AuroraLog::Println("recv:" + msg);
                    Reset();
                    return true;
                }
            }
            else if (Header.Type == (u16)AuroraPackageType::AuroraPackage_File)
            {
                //发送文件采用分块发送

            }
        }
        else
        {
            needSize = AURORA_PACKAGE_HEADER_SIZE - RecvedSize;
            if (needSize > 0)
            {
                if (needSize > len)
                {
                    memcpy(RecvBuffer + RecvedSize, data, len);
                    handedLen += len;
                    RecvedSize += len;
                }
                else
                {
                    memcpy(RecvBuffer + RecvedSize, data, needSize);
                    handedLen += needSize;
                    RecvedSize += needSize;
                }
            }
            if (RecvedSize >= AURORA_PACKAGE_HEADER_SIZE)       //数据头接收完毕
            {
                IsHeaderRecvSuccess = true;
                Header.MappingData(RecvBuffer);
                RecvedSize = 0;
            }
        }
    }
    return false;
}

void AuroraPackage::Reset()
{
    IsHeaderRecvSuccess = 0;
    UserId = "";
    RecvedSize = 0;
    memset(&Header, 0, sizeof(AuroraPackageHeader));
}

AuroraPackageHeader::AuroraPackageHeader()
    : Start(0)
    , Type(0)
    , Size(0)
    , Version(0)
{
}

void AuroraPackageHeader::MappingData(const unsigned char* buffer)
{
    Start = buffer[0];
    buffer += 1;
    memcpy(&Version, buffer, 2);
    buffer += 2;
    memcpy(&Type, buffer, 2);
    buffer += 2;
    memcpy(&Size, buffer, 4);
}
