#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <wininet.h>

void sendWebhook() {
    HINTERNET hSession = InternetOpenW(L"Discord Webhook", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hSession) {
        printf("Failed to open internet session\n");
        return;
    }

    HINTERNET hConnect = InternetConnectW(hSession, L"discord.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) {
        printf("Failed to connect\n");
        InternetCloseHandle(hSession);
        return;
    }

    HINTERNET hRequest = HttpOpenRequestW(hConnect, L"POST",
        L"/api/webhooks/<WEBHOOK>",  // Replace with your webhook URL endpoint
        NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);

    if (!hRequest) {
        printf("Failed to open request\n");
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hSession);
        return;
    }

    const char* headers = "Content-Type: application/json\r\n";
    const char* postData = "{\"content\": \"Hello, Discord!\"}";

    BOOL sent = HttpSendRequestA(hRequest, headers, strlen(headers), (LPVOID)postData, strlen(postData));
    if (!sent) {
        printf("Request failed\n");
    } else {
        printf("Webhook sent successfully!\n");
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hSession);
}

int main() {
    sendWebhook();
    return 0;
}
