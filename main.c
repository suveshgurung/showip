#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
  int status;
  struct addrinfo hints, *serverInfo, *nextNode;
  char ipStr[INET6_ADDRSTRLEN];

  if (argc != 2) {
    fprintf(stderr, "Usage : ./showip hostname\n");
    return 1;
  }

  // initialize the hints struct to be empty.
  memset(&hints, 0, sizeof(hints));
  // allow both IPv4 and IPv6 with AF_UNSPEC.
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo(argv[1], NULL, &hints, &serverInfo)) != 0) {
    fprintf(stderr, "getaddrinfo error : %s\n", gai_strerror(status));
    return 2;
  }

  printf("IP address for %s:\n\n", argv[1]);

  // iterate through the linked list of internet addresses.
  for (nextNode = serverInfo; nextNode != NULL; nextNode = nextNode->ai_next) {
    void *addr;
    char *ipVersion = (char *)malloc(5);

    if (ipVersion == NULL) {
      perror("malloc error\n");
      return 3;
    }

    // get the pointer to the IP address itself.
    if (nextNode->ai_family == AF_INET) {
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)nextNode->ai_addr;
      addr = &(ipv4->sin_addr);
      memcpy(ipVersion, "IPv4", 4);
      ipVersion[4] = '\0';
    } else {
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)nextNode->ai_next;
      addr = &(ipv6->sin6_addr);
      memcpy(ipVersion, "IPv6", 4);
      ipVersion[4] = '\0';
    }

    inet_ntop(nextNode->ai_family, addr, ipStr, sizeof(ipStr));
    printf("%s: %s\n", ipVersion, ipStr);

    free(ipVersion);
  }

  freeaddrinfo(serverInfo);
  return 0;
}
