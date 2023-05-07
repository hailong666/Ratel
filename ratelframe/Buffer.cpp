#include <errno.h>
#include <sys/uio.h>
#include <unistd.h>
#include "Buffer.h"

/**
 * 从fd上读取数据，  Poller工作在LT模式
 * Buffer缓冲区是有大小的  但是从fd上读取数据的时候，却不知道tcp数据最终的大小
*/
ssize_t Buffer::readFd(int fd, int* saveErrno)
{
    char extrabuf[65536] = {0}; //栈上的内存空间
    struct iovec vec[2];
    const size_t writable  = writableBytes(); // 这是Buffer底层缓冲区剩余可写的大小
    vec[0].iov_base = begin() + writeIndex_;
    vec[0].iov_len = writable;

    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof extrabuf;

    const int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
    const ssize_t n = ::readv(fd, vec, iovcnt);
    if (n < 0)
    {
        *saveErrno = errno;
    }
    else if (n <= writable) // Buffer的可写缓冲区已经够存储读出来的数据了
    {
        writeIndex_ += n;
    }
    else // extrabuf 里面也写入了数据
    {
        writeIndex_ = buffer_.size();
        append(extrabuf, n - writable);  // writeIndex_开始写n - writeable大小的数据
    }
    return n;
}

ssize_t Buffer::writeFd(int fd, int* saveErrno)
{
    ssize_t n = ::write(fd, peek(), readableBytes());
    if (n < 0)
    {
        *saveErrno = errno;
    }

    return n;
}