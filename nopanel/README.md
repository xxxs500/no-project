# nopanel

C++初心者が作ったパネル。nowmと世界観をともにします。

・Ubuntu16.4とDebian9.1で動作を確認しています。  
・1920x1080画面専用です。  
・ボタンは10個までしか表示しない仕様です。  
・ボタンをマウスの中ボタンでクリックするとウィンドウが強制終了します。

## ビルドに必要なパッケージ

xorg-devが必要です

    $ sudo apt install xorg-dev

## インストール

makeしてできたファイル「nopanel」をパスの通ったデレクトリにコピーしてください。

## 起動

~/.xinitrcを以下のように書き換えてstartxしてください。

    nopanel &
    nowm &  
    xterm

ターミナルエミュレータはお好みで。必ずnowmの後に記述してください。

## 終了

ターミナルからnopanelを終了させてください。

    $ killall nopanel

