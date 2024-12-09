
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib") // Link della libreria per le socket

#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    const char *messagePrefix = "messaggio iniziale per il server: ";
    const char *serverName;
    struct hostent *serverHost;
    int port;

    // Inizializzazione Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Errore: WSAStartup fallito.\n");
        return 1;
    }

    // Creazione della socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        printf("Errore: socket() fallito.\n");
        WSACleanup();
        return 1;
    }
    // 2. Il client legge da tastiera il nome dell'host e il numero di porta del server
    printf("Inserire il nome dell'host: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0; 
    serverName = buffer;
	
	// 2. Il client legge da tastiera il numero di porta del server
    printf("Inserire la porta (es. 53477): ");
    scanf("%d", &port);
    getchar();

    // Risoluzione dell'indirizzo IP del server con gethostbyname
    serverHost = gethostbyname(serverName);
    if (serverHost == NULL) {
        printf("Errore nella risoluzione dell'host %s\n", serverName);
	closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Configurazione dell'indirizzo del server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr = *((struct in_addr *)serverHost->h_addr_list[0]);
    
	// 3. Il client legge una stringa di caratteri alfanumerici e la invia al server
    printf("Inserisci una stringa da inviare al server: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    // 9. Il client legge la risposta inviata dal server e visualizza la risposta
    int numeroVocali;
    int serverAddrLen = sizeof(serverAddr);
    int bytesReceived = recvfrom(clientSocket, (char *)&numeroVocali, sizeof(numeroVocali), 0, (struct sockaddr *) &serverAddr, &serverAddrLen);
    if (bytesReceived > 0) {
        printf("La stringa %d ricevuta dal server nome:'%s' indirizzo:'%s'", numeroVocali, serverHost->h_name, inet_ntoa(serverAddr.sin_addr));
    }	
    Sleep(2000);
    // Chiusura socket
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}

