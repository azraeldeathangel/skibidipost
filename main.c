#include <stdio.h>
#include <windows.h>
#include <wininet.h>

int main() {
    // Open an internet session
    HINTERNET hSession = InternetOpenW(
        L"Discord Webhook",           // User-Agent name (browser identifier)
        INTERNET_OPEN_TYPE_PRECONFIG, // Use system default proxy settings
        NULL,                         // No explicit proxy
        NULL,                         // No proxy bypass list
        0                             // No additional options
    );

    if (!hSession) {
        printf("InternetOpenW failed: %lu\n", GetLastError());
        return 1;
    }

    // Connect to the Discord API server
    HINTERNET hConnect = InternetConnectW(
        hSession,                     // Internet session handle
        L"discordapp.com",            // Server hostname
        INTERNET_DEFAULT_HTTPS_PORT,  // Use default HTTPS port (443)
        NULL,                         // No username (not needed for webhooks)
        NULL,                         // No password
        INTERNET_SERVICE_HTTP,        // We are making an HTTP request
        0,                            // No special flags
        0                             // Reserved, should be zero
    );

    if (!hConnect) {
        printf("InternetConnectW failed: %lu\n", GetLastError());
        InternetCloseHandle(hSession); // Clean up before exiting
        return 1;
    }

    // Create an HTTP POST request to send data
    HINTERNET hRequest = HttpOpenRequestW(
        hConnect, 
        L"POST",                      // HTTP method (POST request)
        L"/api/webhooks/1354552015124037935/fNh0zz10zIkWPlE0mG-6kOBtfdS7_uhqwfXrFylLmQr5YIf3BYNGSpOgKb6LuY7sWU8E", // Webhook URL path
        NULL,                         // No custom HTTP version
        NULL,                         // No referrer
        NULL,                         // No accepted types (use default)
        INTERNET_FLAG_SECURE,         // Use HTTPS (encrypted)
        0                             // No extra flags
    );

    if (!hRequest) { 
        printf("HttpOpenRequestW failed: %lu\n", GetLastError());
        InternetCloseHandle(hConnect); 
        InternetCloseHandle(hSession);
        return 1;
    }

    // Define the JSON payload to send in the request
    const char* postData = "{\"content\": \"Hello, Discord!\"}";
    DWORD postDataLength = (DWORD)strlen(postData);

    // Create HTTP headers with content type and length
    char headers[256];
    snprintf(headers, sizeof(headers),
        "Content-Type: application/json\r\n"
        "Content-Length: %lu\r\n", postDataLength);

    // Send the HTTP request with headers and the JSON payload
    if (!HttpSendRequestA(
        hRequest, 
        headers, 
        (DWORD)strlen(headers), 
        (LPVOID)postData, 
        postDataLength
    )) {
        printf("HttpSendRequestA failed: %lu\n", GetLastError());
    } else {
        printf("Request sent successfully!\n");
    }

    // Clean up: Close all open internet handles
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hSession);
    
    return 0;
}
