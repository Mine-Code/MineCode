# MineCode
[![GitHub issues](https://img.shields.io/github/issues/yukikamome316/MineCode)](https://github.com/yukikamome316/MineCode/issues) [![license](https://img.shields.io/github/license/yukikamome316/MineCode)](https://github.com/yukikamome316/MineCode/blob/main/LICENSE)

[日本語](https://github.com/yukikamome316/MineCode/blob/main/docs/README-ja.md) ・ [English](https://github.com/yukikamome316/MineCode/blob/main/README.md)

[MineCode](https://github.com/yukikamome316/MineCode/)(マインコード) は、あらゆるゲームのチートコードの作成を素早く簡単に行うために開発されたプログラミング言語です。ソースコードをPowerPC等のアセンブリにコンパイル可能で、実行可能な形式への変換にも対応しています。
## サンプルコード
~~~
mcl minecraft
for player in ServerPlayer.List {
    Local.Console << player.name;
}
~~~
~~~
ptr = [[0x1E4A1220] + 0x12] + 0x144;
[ptr] = 1;
~~~
## フィードバック
- 新機能の提案・要望または不具合の報告等をお待ちしております
- [issue](https://github.com/yukikamome316/MineCode/issues)にてお願いします

## License
Copyright (c) MineCode Corporation. All rights reserved.

Licensed under the [MIT](https://github.com/yukikamome316/MineCode/LICENSE) license.

## Credit
<a href="https://github.com/yukikamome316">
  <img align="left" src="https://github.com/yukikamome316.png" hspace="2" width=100, height=100/>
</a>
<a href="https://github.com/syoch">
  <img align="left" src="https://github.com/syoch.png" hspace="2" width=100, height=100/>
</a>
