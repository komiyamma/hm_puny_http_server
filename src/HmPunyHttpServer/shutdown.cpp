#include <windows.h>

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

