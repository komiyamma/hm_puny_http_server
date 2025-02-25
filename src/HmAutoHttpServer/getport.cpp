#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int getAvailablePort() {
    // Winsockの初期化
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return -1; // 初期化失敗
    }

    // ソケット作成
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return -1; // ソケット作成失敗
    }

    // ポートを自動的に選択するための設定
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = 0; // ポート番号を0に設定すると、システムが空いているポートを選択します

    // バインドを実行
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        closesocket(sock);
        WSACleanup();
        return -1; // バインド失敗
    }

    // 使用中のポートを取得
    int addrLen = sizeof(addr);
    if (getsockname(sock, (struct sockaddr*)&addr, &addrLen) == SOCKET_ERROR) {
        closesocket(sock);
        WSACleanup();
        return -1; // ポート取得失敗
    }

    // クリーンアップ
    int port = ntohs(addr.sin_port);
    closesocket(sock);
    WSACleanup();

    return port; // 空いているポート番号を返す
}
