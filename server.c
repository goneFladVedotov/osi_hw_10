#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int server_sock, client_sock1, client_sock2;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    char buffer[BUFFER_SIZE];

    // Проверка наличия необходимого количества аргументов
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP> <port>\n", argv[0]);
        return 1;
    }

    // Создание сокета
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Error creating socket");
        return 1;
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    // Привязка сокета к адресу
    if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_sock);
        return 1;
    }

    // Переход в режим ожидания подключений
    listen(server_sock, 2);
    client_addr_size = sizeof(client_addr);
    client_sock1 = accept(server_sock, (struct sockaddr *) &client_addr, &client_addr_size);
    client_sock2 = accept(server_sock, (struct sockaddr *) &client_addr, &client_addr_size);

    // Цикл пересылки сообщений от первого клиента ко второму
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        if (recv(client_sock1, buffer, BUFFER_SIZE, 0) > 0) {
            send(client_sock2, buffer, strlen(buffer), 0);
            if (strcmp(buffer, "The End") == 0) break;
        }
    }

    // Закрытие сокетов
    close(client_sock1);
    close(client_sock2);
    close(server_sock);
    return 0;
}
