#include "mongoose.h"   // To build, run: cc main.c mongoose.c
#include <iostream>
#include <memory>
#include <string>

#include "semaphore.h"

using namespace std;

// HTTP server event handler function
void ev_handler(struct mg_connection* c, int ev, void* ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message* hm = (struct mg_http_message*)ev_data;
//        struct mg_http_serve_opts opts = { .root_dir = "./web_root/" };
        struct mg_http_serve_opts opts = { .root_dir = "./" };
        mg_http_serve_dir(c, hm, &opts);
    }
}

extern void BindWindow(HWND hWnd);
extern bool IsBindWindowClosed();
extern int getAvailablePort();
extern BOOL WINAPI ConsoleCtrlHandler(DWORD ctrlType);

int main(int argc, char* argv[]) {

    // コンソール制御ハンドラを設定
    SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

    if (argc >= 2) {
        // 一番目の引数があれば、そのウィンドウがなくなったら、このプロセスも終了する
        HWND hWnd = (HWND)(std::stoul(argv[1], nullptr));
        BindWindow(hWnd);
    }

    std::unique_ptr<HmSemaphore> semaphore = std::make_unique<HmSemaphore>();
	if (!semaphore->isSuccessWaitForOwnership()) {
		cout << 0 << endl; // flush兼ねる
		return 1;
	}

    // 空いているポートを探す。やや時間はかかるが確実性が段違い。
    int port = getAvailablePort();
    if (port <= 0) {
        port = 0;
        cout << 0 << endl; // flush兼ねる
        return 1;
    }

    // URLの構築
    string url = "http://localhost:" + to_string(port);

    // リソースを管理するためにunique_ptrを使用
    unique_ptr<mg_mgr, decltype(&mg_mgr_free)> mgr(new mg_mgr, mg_mgr_free);

	mg_mgr_init(mgr.get());  // イベントマネージャーの初期化

    // リスナー開始
    mg_connection *ret = mg_http_listen(mgr.get(), url.c_str(), ev_handler, NULL);  // Setup listener
    if (ret == NULL) {
        cout << 0 << endl; // flush兼ねる
        return 1;
    }

    cout << port << endl; // flush兼ねる

    // semaphoreオブジェのデストラクタを実行
    semaphore.reset();

    // イベントループ
    while (true) {

		// 結びつけていたウィンドウが閉じてしまっているなら、このプロセスも終了する
		if (IsBindWindowClosed()) {
			break;
		}

        mg_mgr_poll(mgr.get(), 1000);
    }

    // unique_ptrがスコープを離れると、自動でmg_mgr_freeが呼ばれる

    return 0;
}