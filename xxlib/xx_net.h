#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <csignal>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/signalfd.h>
#include <sys/inotify.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "xx_string.h"

namespace xx {

    // 适配 sockaddr_in6
    template<>
    struct StringFuncs<sockaddr_in6, void> {
        static inline void Append(std::string &s, sockaddr_in6 const &in) {
            char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
            if (!getnameinfo((sockaddr * ) & in,
                             ((sockaddr * ) & in)->sa_family == AF_INET6 ? INET6_ADDRSTRLEN : INET_ADDRSTRLEN, hbuf,
                             sizeof hbuf, sbuf, sizeof sbuf, NI_NUMERICHOST | NI_NUMERICSERV)) {
                s.append(hbuf);
                s.push_back(':');
                s.append(sbuf);
            }
        }
    };

}

namespace xx::Net {

    // sockType == SOCK_STREAM || SOCK_DGRAM
    inline int MakeSocketFD(int const &port, int const &sockType = SOCK_DGRAM, char const *const &hostName = nullptr,
                            bool const &nonblock = false, bool const &reusePort = true,
                            size_t const &rmem_max = 1784 * 3000, size_t const &wmem_max = 1784 * 3000) {
        char portStr[20];
        snprintf(portStr, sizeof(portStr), "%d", port);

        addrinfo hints;
        memset(&hints, 0, sizeof(addrinfo));
        hints.ai_family = AF_UNSPEC;                                            // ipv4 / 6
        hints.ai_socktype = sockType;                                           // SOCK_STREAM / SOCK_DGRAM
        hints.ai_flags = AI_PASSIVE;                                            // all interfaces

        addrinfo *ai_, *ai;
        if (getaddrinfo(hostName, portStr, &hints, &ai_)) return -1;

        int fd;
        for (ai = ai_; ai != nullptr; ai = ai->ai_next) {
            fd = socket(ai->ai_family, ai->ai_socktype | (nonblock ? SOCK_NONBLOCK : 0), ai->ai_protocol);
            if (fd == -1) continue;

            int enable = 1;
            if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
                close(fd);
                continue;
            }
            if (reusePort) {
                if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
                    close(fd);
                    continue;
                }
            }
            if (rmem_max) {
                if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char *) &rmem_max, sizeof(rmem_max)) < 0) {
                    close(fd);
                    continue;
                }
            }
            if (wmem_max) {
                if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char *) &wmem_max, sizeof(wmem_max)) < 0) {
                    close(fd);
                    continue;
                }
            }
            if (!bind(fd, ai->ai_addr, ai->ai_addrlen)) break;                  // success

            close(fd);
        }
        freeaddrinfo(ai_);
        if (!ai) return -2;
        return fd;
    }


}