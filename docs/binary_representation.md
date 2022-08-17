# Binary Representation

| kind  |       |     0     |   1   |   2    |   3   |   4    |   5    |    6    |   7    |    8     |   9    |    a    |   b    |   c    |   d    |   e    |   f   |
| :---: | :---: | :-------: | :---: | :----: | :---: | :----: | :----: | :-----: | :----: | :------: | :----: | :-----: | :----: | :----: | :----: | :----: | :---: |
|  Op   |   0   |    Add    |  Sub  |  Mul   |  Div  |   Lt   |   Le   |   Eq    |   Ne   |   And    |   Or   |   Xor   |  Mod   |  ShR   |  ShL   |  RotR  | RotL  |
|   ^   |   1   |   Addi    | Subi  |  Muli  | Divi  |  Lti   |  Lei   |   Eqi   |  Nei   |   Andi   |  Ori   |  Xori   |  Modi  |  ShRi  |  ShLi  | RotRi  | RotLi |
|   ^   |   2   |     ^     |   ^   |   ^    |   ^   |   ^    |   ^    |    ^    |   ^    |    ^     |   ^    |    ^    |   ^    |   ^    |   ^    |   ^    |   ^   |
|   ^   |   3   |     ^     |   ^   |   ^    |   ^   |   ^    |   ^    |    ^    |   ^    |    ^     |   ^    |    ^    |   ^    |   ^    |   ^    |   ^    |   ^   |
|   ^   |   4   |     ^     |   ^   |   ^    |   ^   |   ^    |   ^    |    ^    |   ^    |    ^     |   ^    |    ^    |   ^    |   ^    |   ^    |   ^    |   ^   |
|  val  |   5   |    cgr    |  ccr  |  cfr   |  cdr  |  agr   |  acr   |   afr   |  adr   |   tgr    |  tcr   |   tfr   |  tdr   |  ggr   |  gcr   |  gfr   |  gdr  |
|   ^   |   6   | UF Exists |       |        |       |        |        |         |        |  Sock#0  | Sock#1 | Sock#2  | Sock#3 |   i    |  Com   |   ε    |  HDN  |
|   ^   |   7   |   UF GR   |       |        |       |        |        |         |        |  UF CR   |        |         |        |        |        |        |       |
|   ^   |   8   |   UF FR   |       |        |       |        |        |         |        |  UF DR   |        |         |        |        |        |        |       |
|   ^   |   9   |    u8     |  u16  |  u32   |  u64  |  Str   | Float  | Double  |  Neg   |   Inv    |  LNot  |  BNot   | Offset |  Call  | DeRef  |  ASM   | CafeC |
| Call  |   a   |  UF Call  |       |        |       |        |        |         |        | RPLCall  |        |         |        | CoreI  |  GX2   |  Curl  |  Net  |
|   ^   |   b   |    sin    |  cos  |  tan   | atan  | 2dAtan | 3dAtan |  lerp   | 2dLerp |  3dLerp  |  fma   |   abs   |  sign  | round  | floor  |  ceil  |  pow  |
|   ^   |   c   |   sqrt    | cbrt  | square | cube  |  exp2  | exp10  |  exp16  |  log2  |  log10   | log16  |   min   |  max   |  mean  |  dim   |   -    |   -   |
|   ^   |   d   |   Read    |       | Write  |       | SendTo |        | SendAll |        | RecvFrom |        | RecvMsg |        |  Open  |        | Clone  |       |
|   ^   |   e   |   Tell    |       |  Seek  |       |  Get   |        |  POST   |        |    -     |   -    |    -    |   -    |   -    |   -    |   -    |   -   |
|       |   f   |  Syscall  |       |        |       | Assign |   If   |  Exprs  |   Nil    |    -     |   -    |    -    |   -    | !ECall | !Ident | Method | Term  |
