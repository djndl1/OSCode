#include "file_desc.h"
#include "data_buffer.h"
#include "test_utils.h"
#include "utest.h"

#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <assert.h>

int sock = -1;
static void sigio_handler(int sig, siginfo_t *info, void *ucontext)
{
    if (sig != SIGIO) {
        return;
    }
    printf("signal sig %d = %d\n", sig, SIGIO);

    int fd = info->si_fd;
    printf("fd in signal: %d, sockfd: %d\n", fd, sock);
    //assert(sock == fd);
    long band = info->si_band;
    int code = info->si_code;

    switch (code) {
        case POLL_IN:
            printf("Incoming data\n");
            break;
        case POLL_MSG:
            printf("Incoming message\n");
            break;
        case POLL_HUP:
            printf("device disconnected\n");
            break;
        case POLL_ERR:
            printf("I/O error\n");
            break;
        case POLL_OUT:
            printf("output ready\n");
            break;
        case POLL_PRI:
            printf("High priority input available\n");
            break;
        default:
            printf("%d signal\n", code);
            break;
    }
}

void interrupt_handler(int sig)
{
    if (sock > 0)
    {
        shutdown(sock, SHUT_RDWR);
        close(sock);
        unlink("/var/tmp/fileio.unix");
    }
    exit(0);
}

static bool install_interrupt_handler()
{
    struct sigaction sigact = { 0 };
    sigemptyset(&sigact.sa_mask);
    sigact.sa_handler = interrupt_handler;

    return sigaction(SIGINT, &sigact, NULL) != -1;
}

static bool install_sigio_handler()
{
    struct sigaction sigact = { 0 };
    sigemptyset(&sigact.sa_mask);
    sigact.sa_sigaction = sigio_handler;
    sigact.sa_flags = SA_SIGINFO | SA_RESTART;

    return sigaction(SIGIO, &sigact, NULL) != -1;
}

UTEST(FILEIO, SIG_OASYNC)
{
    install_interrupt_handler();
    if (!install_sigio_handler()) {
        print_error(errno, "Failed to install a handler for SIGIO\n");
        ASSERT_TRUE(false);
    }

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1) {
        print_error(errno, "Failed to create a unix socket\n");
        ASSERT_TRUE(false);
    }

    int r0 = fcntl(sock, F_SETOWN, getpid());
    if (r0 == -1) {
        print_error(errno, "Failed to set the owner of SIGIO\n");
        ASSERT_TRUE(false);
    }

    int fl = fcntl(sock, F_GETFL);
    if (fl == -1) {
        print_error(errno, "Failed to get the file flags of the socket\n");
        ASSERT_TRUE(false);
    }

    r0 = fcntl(sock, F_SETFL, fl | O_ASYNC);
    if (r0 == -1) {
        print_error(errno, "Failed to set the file flags of the socket to O_ASYNC\n");
        ASSERT_TRUE(false);
    }

    struct sockaddr_un addr = { 0 };
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/var/tmp/fileio.unix");

    int r = bind(sock, (const struct sockaddr*)&addr, sizeof(addr));
    if (r == -1) {
        print_error(errno, "Failed to bind a unix socket\n");
        ASSERT_TRUE(false);
    }

    int r2 = listen(sock, 0);
    if (r2 == -1) {
        print_error(errno, "Failed to make a unix socket listen\n");
        ASSERT_TRUE(false);
    }

    bool continuing = true;
    printf("Listening on /var/tmp/fileio.unix\n");
    while (continuing) {
        struct sockaddr_un peer_addr = { 0 };
        socklen_t addr_len = sizeof(struct sockaddr_un);

        int peer = 0;
        if ((peer = accept(sock, (struct sockaddr*)&peer_addr, &addr_len)) == -1) {
            print_error(errno, "Failed to accept a unix socket connection\n");
        }
        printf("connection from %s\n", peer_addr.sun_path);

        send(peer, "ABC", 3, 0);
        close(peer);
    }

close_socket:
    close(sock);
}

UTEST_MAIN();
