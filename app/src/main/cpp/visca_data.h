//
// Created by zhangxd on 2019/3/12.
//
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <android/log.h>
#include <sys/shm.h>
#include <time.h>

#define LOG_TAG "VISCA"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
int socketfd;
extern "C"
int connectMyServer(const char *ip, uint16_t port) {
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ip);
    if (connect(socketfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        LOGE("connect failure");
        return VISCA_FAILURE;
    }
    LOGD("connect success");
    return VISCA_SUCCESS;
}

char *sendData(unsigned char *data, uint8_t length) {
    LOGD("sendData size:%d", length);
    uint8_t sendbuffer[length];
    memset(sendbuffer, 0, length);
    memcpy(sendbuffer, data, length);
    if (send(socketfd, sendbuffer, length, 0) < 0) {
        LOGE("sendData failure");
        return NULL;
    };
    int i = 0;
    while (i < sizeof(sendbuffer)) {
        LOGD("send data: %x", sendbuffer[i]);
        i++;
    }
    char *result = new char[20];
    clock_t start = clock();
    LOGD("enter1 %d", start / CLOCKS_PER_SEC);
    while (((clock() - start) / CLOCKS_PER_SEC) < 5) {
        int size = recv(socketfd, result, 20, 0);
        int num = 0;
        while (num < size) {
            LOGD("receive data %x", result[num]);
            num++;
        }
        if (size > 0) {
            return result;
        }
    }

    return result;

}

int closeSocket() {
    close(socketfd);
}

