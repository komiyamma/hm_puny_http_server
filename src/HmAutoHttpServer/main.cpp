#include "mongoose.h"   // To build, run: cc main.c mongoose.c
#include <string> // getopt関数を使うために必要
#include <iostream>



// HTTP server event handler function
void ev_handler(struct mg_connection* c, int ev, void* ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message* hm = (struct mg_http_message*)ev_data;
//        struct mg_http_serve_opts opts = { .root_dir = "./web_root/" };
        struct mg_http_serve_opts opts = { .root_dir = "./" };
        mg_http_serve_dir(c, hm, &opts);
    }
}

BOOL WINAPI ConsoleCtrlHandler(DWORD ctrlType) {
    switch (ctrlType) {
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        // std::cout << "ログオフまたはシャットダウンイベントが発生しました。 プログラムを終了します。" << std::endl;
        ExitProcess(0);  // すぐにプログラムを終了
        return TRUE;  // イベントを処理したことをシステムに通知
    default:
        return FALSE; // 他のイベントは処理しない
    }
}

int getAvailablePort();

int main(int argc, char* argv[]) {
    using namespace std;

    // コンソール制御ハンドラを設定
    SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

    int port = getAvailablePort();
    if (port <= 0) {
        port = 0;
        cout << port << endl;
        return 1;
    }

    char url[100];
    sprintf_s(url, sizeof(url), "http://localhost:%d", port);
    cout << port << endl;

    struct mg_mgr mgr;  // Declare event manager
    mg_mgr_init(&mgr);  // Initialise event manager

    auto ret = mg_http_listen(&mgr, url, ev_handler, NULL);  // Setup listener
    for (;;) {          // Run an infinite event loop
        mg_mgr_poll(&mgr, 1000);
    }
    return 0;
}