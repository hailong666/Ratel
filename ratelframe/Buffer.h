#pragma once

#include<vector>
#include <cstdlib>
#include <string>
#include <algorithm>
// 网络库底层的缓冲区类型定义
class Buffer
{
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;
    explicit Buffer(size_t initialSize = kInitialSize)
            : buffer_(kCheapPrepend + initialSize)
            , readerIndex_(kCheapPrepend)
            , writeIndex_(kCheapPrepend)
    {

    }
    size_t readableBytes() const
    {
        return writeIndex_ - readerIndex_;
    }
    size_t writableBytes() const
    {
        return buffer_.size() - writeIndex_;
    }
    size_t prependableBytes() const
    {
        return readerIndex_;
    }

    // 返回缓冲区中刻度数据的起始地址
    const char* peek() const
    {
        return begin() + readerIndex_;
    }
    // onMessage string  <- Buffer
    void retrieve(size_t len)
    {
        if (len < readableBytes())
        {
            readerIndex_ += len; //应用只读取了可读缓冲区数据的一部分 即len长度
        }
        else
        {
            retrieveAll();
        }
    }
    void retrieveAll()
    {
        readerIndex_ = writeIndex_ = kCheapPrepend;

    }
    // 把onMessage函数上报的Buffer数据，转换成string类型
    std::string retrieveAllString()
    {
        return retrieveAsString(readableBytes());
    }
    std::string retrieveAsString(size_t len)
    {
        std::string result(peek(), len);
        retrieve(len); //缓冲区中可读的数据已经读了，所以要进行复位
        return result;
    }
    ~Buffer()
    {

    }  
    // buffer_.size - writeIndex_   和   len  大小不一致
    void ensureWriteableBytes(size_t len)  
    {
        if (writableBytes() < len)
        {
            makeSpace(len); // 扩容函数
        }
    }
    // 把[data , data + len ]内存上的数据添加到writeable缓冲区中
    void append(const char *data, size_t len)
    {
        ensureWriteableBytes(len);
        std::copy(data, data + len, beginWrite());
        writeIndex_ += len;
    }
    char* beginWrite()
    {
        return begin() + writeIndex_;
    }
    const char* beginWrite() const
    {
        return begin() + writeIndex_;
    } 
    // 从fd上读取数据
    ssize_t readFd(int fd, int* saveErrno);
    // 从fd上写数据
    ssize_t writeFd(int fd, int* saveErrno);   
private:
    char* begin()
    {
        return &*buffer_.begin(); //底层数组首元素地址，也就是数组的起始地址
    }
    const char* begin() const
    {
        return &*buffer_.begin(); //底层数组首元素地址，也就是数组的起始地址
    } 
    void makeSpace(size_t len)
    {
        if (writableBytes() + prependableBytes() < len + kCheapPrepend)
        {
            buffer_.resize(writeIndex_ + len);
        }
        else
        {
            size_t readable = readableBytes();
            std::copy(begin() + readerIndex_,
                     begin() + writeIndex_,
                     begin() + kCheapPrepend);
            readerIndex_ = kCheapPrepend;
            writeIndex_ = readerIndex_ + readable;
        }
    }
    std::vector<char> buffer_;
    size_t readerIndex_;
    size_t writeIndex_;

};

