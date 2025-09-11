#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>

int sock;
void goodbye(int sig) {
    close(sock);
    write(1, "Goodbye!\n", 10);
    _exit(0);
}
const int maxtries = 3;
int tries = maxtries;
int ok = 1;
int i = 0;
uint32_t packetindex = 0;
char outpacket[2052] = {0};
char readbuffer[2048] = {0};
int main() {
    int version = 0;
    int release = 0;
    int subrelease = 1;
    struct sigaction sa;
    sa.sa_handler = goodbye;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    printf("Starting up MeshNet Client version %i.%i.%i...\n", version, release, subrelease);
    printf("Opening socket...\n");
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        printf("Error %i creating socket\n", sock);
        return sock;
    }
    struct timeval tv;
    tv.tv_sec = 1;       // 1 second timeout
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(5960);
    printf("Pulling from UDP port 5960\n");
    socklen_t addr_len = sizeof(server_addr);
    sendto(sock, "GET", 3, 0, (struct sockaddr*)&server_addr, addr_len);
    FILE *fp = fopen("output.txt", "ab");
    if (!fp) { perror("fopen"); return 1; }
    while (1) {
        char packet[2052];
        //its time for the big mamaaa
        ssize_t recv_len = recvfrom(sock, packet, sizeof(packet) -1, 0, (struct sockaddr*)&server_addr, &addr_len);
        if (recv_len < 0) {
            printf("Packet length less than 0");
            continue;
        }
        if (recv_len == 4) {
            printf("done!\n");
            break;
        }
        packet[recv_len] = '\0';
        printf("recieved %zd bytes from %s:%d: %s\n", recv_len, inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port), packet);
        char *sig = "-1";
        strcpy(outpacket, sig);
        sendto(sock, outpacket, strlen(sig), 0, (struct sockaddr*)&server_addr, addr_len);
        if (strstr(packet, "Net Connection Established") != NULL ||
            strncmp(packet, "GET Recieved.", 13) == 0) {
            continue; // skip these header packets
        }
        fwrite(packet + 4, 1, recv_len - 4, fp);
    }
    return(0);
}