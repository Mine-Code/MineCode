# ビルドイン機能

## 変数

### stdout (OutStream)
### stdin (InStream)
### stdio (IOStream)

## オブジェクト

### String オブジェクト

- `"..."` で定義できます
- `length` メンバーを持ちます

### Range オブジェクト

- `"A...B"` て定義できます
- `begin`, `step`, `stop`, `count` メンバーを持ちます

### Stream オブジェクト

- `read`, `write` メソッドを持ちます

### InStream オブジェクト

- `stdin` などに使用されています
- `Stream` を継承しています
- `operator >>` を使用して読み込みができます
- `write` メソッドは使用できません

### OutStream オブジェクト

- `stdin` などに使用されています
- `Stream` を継承しています
- `operator <<` を使用して読み込みができます
- `read` メソッドは使用できません

### IOStream オブジェクト

- `stdio` などに使用されています
- `InStream`, `OutStream` 両方の特徴を持ちます

## 言語機能

### 関数

以下のように定義できます
```plain
func add(String a, int b) -> String {
  return a + b;
}
```

### 演算子オーバーライド

任意のクラス内で `operator <演算子>` の形で挙動をオーバーライドできます

override
virtual