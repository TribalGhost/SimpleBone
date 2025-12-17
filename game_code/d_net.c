#ifdef BUILD_LINUX_NET
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
typedef struct addrinfo addrinfo; 
#endif

#ifdef BUILD_WINDOWS_NET
#include "errno.h"
#include "winsock2.h"
#include "ws2tcpip.h"

typedef ADDRINFOA addrinfo;
#endif

typedef struct NetState NetState;
struct NetState
{
    int server_socket;
    bool is_server;
    bool is_client;
};

global NetState net_state;

#define MESSAGE_SIZE 64

internal void server_update()
{
    char message[MESSAGE_SIZE] = {};
    
    struct sockaddr_storage connection_address = {};
    int conection_address_size = sizeof(struct sockaddr_storage);
    int connection_socket = accept(net_state.server_socket, (struct sockaddr * )&connection_address , &conection_address_size);
    
    if(connection_socket == -1) 
    {
        int error_code = errno;
        
        if(error_code == EAGAIN)
        {
        }
        else if(error_code == EWOULDBLOCK)
        {
        }
        else 
        {
            CATCH;
        }
    }
    else
    {
        int receive_byte = recv(connection_socket, message , MESSAGE_SIZE, 0);
        if(receive_byte == 0) CATCH;
        if(receive_byte == -1) CATCH;
        
        printf(message);
    }
}

internal void start_connection()
{
#define TEST_PORT 35222
    
#ifdef BUILD_WINDOWS_NET
    WSADATA data = {};
    WSAStartup( MAKEWORD(2,2) , &data);
#endif
    
    char ip_string[INET6_ADDRSTRLEN] = {};
    
    if(net_state.is_server)
    {
        struct hostent * host = gethostbyname("192.168.10.8");
        inet_ntop(AF_INET, &((struct in_addr *)(host->h_addr))->s_addr , ip_string, INET6_ADDRSTRLEN);
        
        struct sockaddr_in test = {};
        test.sin_port = htons(TEST_PORT);
        test.sin_family = AF_INET;
        test.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
        //test.sin_addr.s_addr = INADDR_ANY;
        
        net_state.server_socket = socket(AF_INET, SOCK_STREAM, 0);
        
        int yes = true;
        if(setsockopt(net_state.server_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int)) == -1) CATCH;
        
#ifdef BUILD_WINDOWS_NET
        int non_blocking = 1;
        ioctlsocket(net_state.server_socket, FIONBIO, &non_blocking);
#endif
        
#ifdef BUILD_LINUX_NET
        fcntl(net_state.server_socket, F_SETFL, O_NONBLOCK);
#endif
        
        if(bind(net_state.server_socket, (struct sockaddr * )&test, sizeof(struct sockaddr_in)) == -1) 
        {
            int error_code = errno;
            CATCH;
        }
        
        if(listen(net_state.server_socket , 10) == -1) CATCH;
    }
    
    if(net_state.is_client)
    {
        struct hostent * host = gethostbyname("192.168.10.8");
        struct sockaddr_in test = {};
        test.sin_port = htons(TEST_PORT);
        test.sin_family = AF_INET;
        test.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
        
        //inet_ntop(AF_INET, &test.sin_addr.s_addr , ip_string, INET6_ADDRSTRLEN);
        
        int retry_attemp_on_refuse = 0;
        
        net_state.server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if(net_state.server_socket == -1) CATCH;
        
#if 0
        if(bind(net_state.server_socket, (struct sockaddr * )&test, sizeof(struct sockaddr_in)) == -1) 
        {
            int error_code = errno;
            CATCH;
        }
#endif
        
        for(;;)
        {
            if(retry_attemp_on_refuse >= 5) CATCH;
            
            if(connect(net_state.server_socket , (struct sockaddr *)&test , sizeof(struct sockaddr_in)) == -1) 
            {
                int error_code = -1;
                
#ifdef BUILD_WINDOWS_NET
                error_code = WSAGetLastError();
                switch(error_code)
                {
                    case 10061: retry_attemp_on_refuse++; Sleep(1); break;
                    default: CATCH; break;
                }
#endif
                
#ifdef BUILD_LINUX_NET
                error_code = errno;
                switch(error_code)
                {
                    //case 10061: retry_attemp_on_refuse++; Sleep(10); break;
                    default: CATCH; break;
                }
#endif
            }
            else
            {
                break;
            }
        }
        
        char message[MESSAGE_SIZE] = {};
        
        sprintf(message , "hello from the other side\n");
        int remaining_byte = send(net_state.server_socket, message, MESSAGE_SIZE, 0);
        if(remaining_byte == -1) CATCH;
    }
}

internal void end_connection()
{
#ifdef BUILD_LINUX_NET
    close(net_state.server_socket);
#endif
    
#ifdef BUILD_WINDOWS_NET
    closesocket(net_state.server_socket);
    WSACleanup();
#endif
}