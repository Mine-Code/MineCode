# MCLドキュメント

MCLはディレクトリ構造をもったMineCode用ライブラリ形式です\
MineCode1.0.0では単一のjsonファイルを使用していましたが

- ファイルサイズが膨大になる
- 管理が難しい
- jsonでは16進数をかけないのでアドレスの記述が難しい

などの理由でMineCode2.0.0ではディレクトリ構造をもった独自形式を用いることにしました

# 概要

基本的な構造(markdownでみたほうがわかりやすいかもしれません)

## MineCode/mcls/{name}

- mcl.json
- `**/*.mcl`

### mcl.json

mcl.jsonではmclの基本情報を記します

```json
{
  "displayname": "表示名",
  "version": "MCLのバージョン",
  "ver": 2 // 前方互換性のため
}
```

### *.mcl

```
class ClassName
  [offset, ":"], name type
  [offset, ":"], name type

Type ClassName::MethodName [MethodAddress](ArgumentList)
```

こんな感じです
