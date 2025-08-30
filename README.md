# HmPunyHttpServer

秀丸エディタでブラウザ枠やレンダリング枠に `http://localhost:port/` 形式でアクセスできるようにするための、手軽でコピー運用しやすい簡易HTTPサーバーです。

![HmPunyHttpServer latest release](https://img.shields.io/github/v/release/komiyamma/hm_puny_http_server)
[![License: GPL v2](https://img.shields.io/badge/license-GPL2-blue.svg?style=flat)](LICENSE.md)
![Hidemaru: v9.25+](https://img.shields.io/badge/Hidemaru-v9.25%2B-6479ff.svg)

## 概要

Vue.jsやReactなどの現代的なフレームワークで作成されたWebページは、`file://` プロトコルでは正しく動作しないことが多く、動作確認のためにはHTTPサーバーが必要不可欠です。

`HmPunyHttpServer` は、秀丸エディタのマクロから簡単に利用できる軽量なHTTPサーバーを提供し、このような問題を解決します。指定したフォルダをドキュメントルートとして、ローカルでWebコンテンツを配信できます。

より詳しい情報は、作者のウェブサイトも参照してください。
- [HmPunyHttpServer (コピー運用しやすいHTML/httpサーバー)](https://秀丸マクロ.net/?page=nobu_tool_hm_puny_http_server)

## 主な特徴

- **簡単な連携**: 秀丸マクロ (jsmode) から簡単にHTTPサーバーを起動・終了できます。
- **動的なドキュメントルート**: マクロ実行時にドキュメントルート（公開するフォルダ）を自由に指定できます。
- **複数起動対応**: 複数のサーバーを同時に起動できます。
- **自動ポート割当**: 利用可能なポートを自動的に探し出してサーバーを起動します。
- **自動シャットダウン**: 秀丸エディタの終了（通常終了、強制終了を含む）を検知し、起動したHTTPサーバープロセスを自動的に終了させます。
- **非同期サポート**: サーバーの起動を非同期で行い、秀丸エディタのUIをブロックしません。
- **軽量**: 少ないファイルで構成されており、導入が簡単です。

## 必要なファイル

- `HmPunyHttpServer.exe`: HTTPサーバー本体
- `HmPunyHttpServer.js`: 秀丸マクロからサーバーを操作するためのラッパーライブラリ

## セットアップ

1.  最新版の `HmPunyHttpServer.zip` を[リリースページ](https://github.com/komiyamma/hm_puny_http_server/releases)からダウンロードします。
2.  ダウンロードしたzipファイルを展開します。
3.  `HmPunyHttpServer.exe` と `HmPunyHttpServer.js` を、秀丸マクロからアクセスしやすい任意のディレクトリに配置します。一般的には、秀丸マクロディレクトリ以下に `HmPunyHttpServer` のようなサブディレクトリを作成して配置すると管理しやすくなります。

    **配置例:**
    ```
    C:\Users\YourName\Documents\Hidemaru\Macros\
    └─ HmPunyHttpServer\
       ├─ HmPunyHttpServer.exe
       └─ HmPunyHttpServer.js
    ```

## 使い方

`HmPunyHttpServer` は、秀丸マクロ（jsmode）から `HmPunyHttpServer.js` を介して利用します。

### 基本的な使い方（同期的）

サーバーを起動し、取得したポート番号を使ってブラウザ枠でURLを開く基本的なマクロです。

```javascript
// 秀丸マクロファイル (.mac)

// JScriptモードで実行
jsmode "JScript\\" + currentmacrofilename;

// HmPunyHttpServer.js を読み込む
execjs @"C:\path\to\HmPunyHttpServer\HmPunyHttpServer.js";

js {
    // サーバーインスタンスを格納する変数
    // マクロ実行空間を超えて維持するため、varで宣言します。
    var server;

    // 既にサーバーが起動している場合は一度閉じる
    if (server) {
        server.close();
    }

    // サーバーインスタンスを作成
    // rootFolderに公開したいフォルダのパスを指定します。
    server = createPunyHttpServer({
        rootFolder: "C:\\path\\to\\your_contents"
    });

    if (server) {
        // サーバーを起動し、ポート番号を取得（同期的）
        var port = server.start();

        if (port) {
            // 成功した場合、ブラウザ枠で開く
            var url = "http://localhost:" + port + "/";
            browserpanecommand({
                target: "_each",
                url: url,
                show: 1,
                size: 500
            });
        } else {
            hidemaru.message("サーバーの起動に失敗しました。");
        }
    }
}
```

### 非同期での使い方

`startAsync` メソッドを使うと、サーバーの起動を待たずにマクロの処理を続行できます。ポート番号はコールバック関数経由で取得します。

```javascript
// 秀丸マクロファイル (.mac)
jsmode "JScript\\" + currentmacrofilename;
execjs @"C:\path\to\HmPunyHttpServer\HmPunyHttpServer.js";

js {
    var server;
    if (server) {
        server.close();
    }

    server = createPunyHttpServer({
        rootFolder: "C:\\path\\to\\your_contents"
    });

    if (server) {
        // サーバーを非同期で起動
        server.startAsync(function(port) {
            if (port) {
                var url = "http://localhost:" + port + "/";
                browserpanecommand({
                    target: "_each",
                    url: url,
                    show: 1,
                    size: 500,
                    initialize: "async" // ブラウザ枠も非同期で初期化
                });
            } else {
                hidemaru.message("サーバーの起動に失敗しました。");
            }
        });
    }
}
```

### APIリファレンス

`HmPunyHttpServer.js` が提供するAPIです。

#### `createPunyHttpServer(props)`

サーバーインスタンスを作成します。

-   **`props`** (Object): サーバーの設定を含むオブジェクト。
    -   **`rootFolder`** (String): **必須**。ドキュメントルートとして公開するフォルダのフルパス。
-   **戻り値**: サーバーインスタンス。失敗した場合は `null`。

#### `server.start()`

サーバーを同期的に起動します。

-   **戻り値** (Number): 割り当てられたポート番号。失敗した場合は `0`。

#### `server.startAsync(onPort)`

サーバーを非同期的に起動します。

-   **`onPort`** (Function): ポート番号が確定したときに呼び出されるコールバック関数。
    -   引数 (Number): 割り当てられたポート番号。失敗した場合は `0`。

#### `server.close()`

起動しているサーバープロセスを終了します。

## 開発者向け情報

### ビルド方法

ソースコードは C++ で書かれており、Visual Studio 2022 を使ってビルドできます。

1.  リポジトリをクローンします。
    ```sh
    git clone https://github.com/komiyamma/hm_puny_http_server.git
    ```
2.  `src/HmPunyHttpServer.sln` を Visual Studio 2022 で開きます。
3.  ビルド構成を `Release` / `x64` または `Release` / `x86` に設定します。
4.  「ビルド」メニューから「ソリューションのビルド」を実行します。
5.  `src/x64/Release/` または `src/x86/Release/` 以下に `HmPunyHttpServer.exe` が生成されます。

## ライセンス

このプロジェクトは **GPLv2** の下でライセンスされています。
詳細は [LICENSE.md](LICENSE.md) をご覧ください。

---
*Note: `HmPunyHttpServer.js` is licensed under CC0 1.0 Universal.*
