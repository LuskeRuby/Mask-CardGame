#include <controller/playPhase.h>

#include "controller/startupPhase.h"
#include "model/playPhaseCommands.h"
#include "model/StartPhaseCommands.h"

/*
int main(void) {
    LD(NULL);
    RunStartupPhase();
}
*/


#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <view/print.h>
#include <view/print.h>
#include <view/print.h>
#pragma comment(lib, "ws2_32.lib")  // Link Winsock library

#define PORT 12345

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c;
    char buffer[1024];

    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Error creating socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, 3) == SOCKET_ERROR) {
        printf("Listen failed: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Waiting for client on port %d...\n", PORT);

    c = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr *)&client, &c);
    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    LD(NULL); //LD null before we send/receive commands
    // Continuously receive and process commands
    while (1) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received == SOCKET_ERROR) {
            printf("Error receiving data: %d\n", WSAGetLastError());
            break;
        }

        if (bytes_received == 0) {
            printf("Client disconnected\n");
            break;
        }
        buffer[bytes_received] = '\0';  // Null-terminate the received string
        printf("Received command: '%s'\n", buffer);  // Print the received command


        // Trim the received command from whitespace
        char *command = buffer;
        while (*command && (*command == ' ' || *command == '\n' || *command == '\r')) {
            command++;  // Skip leading spaces or newline characters
        }

        //GUI manageing the two phases
        if (currentPhase == STARTUP_PHASE) {
            RunStartupPhase(command);  // run Startup Phase
        } else if (currentPhase == PLAY_PHASE) {
            RunPlayPhase(command);  // run Play Phase
        }

        //The string we send will end with "END" so client know we are done reading
        strcat(outputString, "END\n");
        char *response = outputString;

        // Send response back to the client after processing each command
        printf("Sending response: '%s'\n", response);  // Print the response being sent

        int bytes_sent = send(client_socket, response, strlen(response), 0);
        if (bytes_sent == SOCKET_ERROR) {
            printf("Error sending response: %d\n", WSAGetLastError());
            break;
        }

        if (bytes_sent != strlen(response)) {
            printf("Warning: Not all data was sent. Only %d bytes sent.\n", bytes_sent);
        }
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
