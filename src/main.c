#include "controller/startupPhase.h"
#include "model/playPhaseCommands.h"
#include "model/StartPhaseCommands.h"

//USED FOR TEST (DELETE BEFORE SUBMISSION)
/*
int main(void) {
    LD(NULL);
    InitArray();
    Card* k1 = columnArr[0];
    Card* k2 = columnArr[1];
    Card* k3 = columnArr[2];
    Card* k4 = columnArr[3];
    Card* k5 = columnArr[4];
    Card* k6 = columnArr[5];
    Card* k7 = columnArr[6];

    RunStartupPhase();

    //MoveTopCards(&columnArr[1],&columnArr[0],5);
    PrintPlayPhase("ok", "ok");
}
*/
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
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

        // Trim the received command from whitespace (leading and trailing)
        char *command = buffer;
        while (*command && (*command == ' ' || *command == '\n' || *command == '\r')) {
            command++;  // Skip leading spaces or newline characters
        }

        // Prepare a response to send back
        char *response = "C1\tC2\tC3\tC4\tC5\tC6\tC7\nAC\t[]\t[]\t[]\t[]\t[]\t[]\t\t[]\tF1\n\t[]\t[]\t[]\t[]\t[]\t[]\t\t[]\tF2\n\t[]\t[]\t[]\t[]\t[]\t[]\t\t[]\tF3\n\t[]\t[]\t[]\t[]\t[]\t[]\t\t[]\tF4\n\nLAST Command: ok\nMessage: Everything is fine\nEND\n";

        // Send response back to the client after processing each command
        printf("Sending response: '%s'\n", response);  // Log the response being sent

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
