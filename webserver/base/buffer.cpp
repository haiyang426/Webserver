#include "Buffer.h"

#include <errno.h>
#include <sys/uio.h>
#include <unistd.h>

ssize_t Buffer::readFd(int fd, int* saveErrno)
{
    char extrabuf[65536] = {0};
    struct iovec vec[2];
    const size_t writableSpace = writableBytes();

    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_len = writableSpace;

    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);

    const int iovcnt = (writableSpace < sizeof(extrabuf) ? 2 : 1);

    const ssize_t n = readv(fd, vec, iovcnt);
    if(n < 0){
        *saveErrno = errno;
    }
    else if(n <= writableSpace){
        writerIndex_ += n;
    }
    else{
        writerIndex_ = buffer_.size();
        append(extrabuf, n-writableSpace);
    }
}

ssize_t Buffer::writeFd(int fd, int* saveErrno)
{
    const size_t readableSpace = readableBytes();
    ssize_t n = write(fd, peek(), readableSpace);
    if(n < 0)
    {   
        *saveErrno = errno;
    }
    return n;
}