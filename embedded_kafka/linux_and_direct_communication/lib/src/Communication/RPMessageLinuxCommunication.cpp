#ifdef __unix__

#include "RPMessageLinuxCommunication.hpp"

RPMessageLinuxCommunication::RPMessageLinuxCommunication(const RPMessageEndpoint &ep, const Logger &l) : Communication(l), endpoint(ep)
{
    int ret, status, c;

    /* Use auto-detection for SoC */
    ret = rpmsg_char_init(NULL);
    if (ret)
    {
        logger.logError("rpmsg_char_init failed, ret = %d", ret);
    }
}

RPMessageLinuxCommunication::~RPMessageLinuxCommunication()
{
    close_devs();
    rpmsg_char_exit();
}

int RPMessageLinuxCommunication::write(const char *message, size_t messageSize, const Endpoint &destination)
{
    rpmsg_char_dev_t *rcdev;
    const RPMessageEndpoint &rpMessageDestination = static_cast<const RPMessageEndpoint &>(destination);
    char eptdev_name[32] = {0};
    int flags = 0, ret = 0;
    int fd;

    sprintf(eptdev_name, "rpmsg-char-%d-%d", rpMessageDestination.getCoreId(), getpid());

    if (endpointMap.find(rpMessageDestination) == endpointMap.end())
    {
        rcdev = rpmsg_char_open(static_cast<rproc_id>(rpMessageDestination.getCoreId()), NULL, RPMSG_ADDR_ANY, rpMessageDestination.getServiceEndpoint(),
                                eptdev_name, flags);
        endpointMap.insert(std::make_pair(rpMessageDestination, rcdev));
        fds.push_back(rcdev->fd);
    }

    rcdev = endpointMap[rpMessageDestination];

    ret = send_msg(rcdev->fd, message, messageSize);

    return ret;
}

int RPMessageLinuxCommunication::read(char *buffer, size_t bufferSize, Endpoint &source)
{
    int packet_len;
    int maxfd = *std::max_element(fds.begin(), fds.end());

    fd_set read_fds;
    FD_ZERO(&read_fds);

    for (const auto &fd : fds)
    {
        FD_SET(fd, &read_fds);
    }

    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    int num_ready = select(maxfd + 1, &read_fds, nullptr, nullptr, NULL);

    if (num_ready == -1)
    {
        logger.log("Error in select");
    }
    else if (num_ready == 0)
    {
        logger.log("Timeout occured");
    }
    else
    {
        for (const auto &fd : fds)
        {
            if (FD_ISSET(fd, &read_fds))
            {
                setSourceEndpoint(fd, source);
                int bytesRead = recv_msg(fd, MAX_MESSAGE_SIZE_LINUX, buffer, &packet_len);
                if (packet_len > 0)
                {
                    buffer[packet_len] = '\0';
                }
                return bytesRead;
            }
        }
    }

    return 0;
}

int RPMessageLinuxCommunication::send_msg(int fd, const char *msg, int len)
{
    int ret = 0;

    ret = ::write(fd, msg, len);
    if (ret < 0)
    {
        logger.logError("Can't write to rpmsg endpt device");
        return -1;
    }

    return ret;
}

int RPMessageLinuxCommunication::recv_msg(int fd, int len, char *reply_msg, int *reply_len)
{
    int ret = 0;

    /* Note: len should be max length of response expected */
    ret = ::read(fd, reply_msg, len);
    if (ret < 0)
    {
        logger.logError("Can't read to rpmsg endpt device");
        return -1;
    }
    else
    {
        *reply_len = ret;
    }

    return 0;
}

void RPMessageLinuxCommunication::close_devs()
{
    for (auto iter = endpointMap.begin(); iter != endpointMap.end(); ++iter)
    {
        rpmsg_char_close(iter->second);
    }
}

void RPMessageLinuxCommunication::setSourceEndpoint(int fd, Endpoint &source)
{
    for (const auto &pair : endpointMap)
    {
        const rpmsg_char_dev_t &value = *pair.second;
        if (value.fd == fd)
        {
            const RPMessageEndpoint &endpoint = pair.first;
            static_cast<RPMessageEndpoint &>(source).setCoreId(endpoint.getCoreId());
            static_cast<RPMessageEndpoint &>(source).setServiceEndpoint(endpoint.getServiceEndpoint());
        }
    }
}

#endif
