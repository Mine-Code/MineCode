# MineCode
[![GitHub issues](https://img.shields.io/github/issues/yukikamome316/MineCode)](https://github.com/yukikamome316/MineCode/issues) [![license](https://img.shields.io/github/license/yukikamome316/MineCode)](https://github.com/yukikamome316/MineCode/blob/main/LICENSE)

[日本語](https://github.com/yukikamome316/MineCode/blob/main/docs/README-ja.md) ・ [English](https://github.com/yukikamome316/MineCode/blob/main/README.md)

[MineCode](https://github.com/yukikamome316/MineCode/) is a programming language which allows you to make cheat codes easier, faster and emmits PowerPC assembles or something else.

## Announcement
MineCode2.0.0 is still at the considing syntax and some program flow
The main branch is MineCode1.x.x

## Requirements
- [Meson Build System](https://mesonbuild.com/)
- [stringUtility](https://github.com/syoch/stringUtility/)
- [Assembler-ppc](https://github.com/syoch/Assembler-ppc)

## Sample Code
~~~
mcl minecraft
for player in ServerPlayer.List {
    Local.Console << player.name
}
~~~
~~~
ptr = [[0x1E4A1220] + 0x12] + 0x144
[ptr] = 1
~~~

## How To Build?
Run these commands in your shell
```
mkdir builddir
cd builddir
meson .. .
ninja
```

## Feedback
- Request a new feature
- File an issue

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
