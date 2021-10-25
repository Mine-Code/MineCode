# core

```
oct = 0o{(0-7)}
dec = (1-9){0-9}
hex = 0x{(0-9)|(a-f)}
ascii = [0x00-0xff]

word = {ascii}\
int = {oct|dec|hex}
```

# idnetities

```
ident = word;
imm = int;
str = "\"", {ascii}, "\"";
```

# expr : ordered

```
ua_asg = lvalue, "++" | "--"
ub_asg = "++" | "--", lvalue
assign = lvalue, operators, "=", rvalue
subexpr = "(", expr, ")"
equ_neq = expr, "==" | "!=", expr
inequal = expr, "<" | ">" | "<=" | ">=", expr
add_sub = expr, "+" | "-", expr
mul_div = expr, "*" | "/" | "%", expr
sht_sht = expr, "<<" | ">>", expr
bit_ope = expr, "|" | "&" | "^", expr
primary = value
```

# operator : ordered

```
equ_neq = "==" | "!="
inequal = "<" | ">" | "<=" | ">="
add_sub = "+" | "-"
mul_div = "*" | "/" | "%"
sht_sht = "<<" | ">>"
bit_ope = "|" | "&" | "^"
```

# value : select

```
lvalue = ptr | ident | attribute;
rvalue = str | imm | funccall | put;
```

## base

```
ptr = "[", value, [ "+", imm ], "]";
attribute = ident, {".", ident};
put = lvalue , "<<", expr;
```

## others

```
_range = ptr | ident | attribute
       | imm | funccall | put
range = value, "...", value
```

## funccall

```
funccall = calltype, "(", args, ")"
```

### calltype

```
rawcall = "func","[",expr,"]"
namecall = lvalue
```

# syntax

```
arg = ident, [*], ident;
args = arg, {"*", arg}
```

# stmt

```
for
  "for", "(",
    ident, "in", value|range,
  ")", ":",
  block

while = "while", "(" cond, ")", ":"

if
  "if", cond, ":", block,
  {"elif, cond, ":", block},
  "else, ":", block

func
  "func", "(", ident?, {",", ident}, ")", ":",
  block

mcl = "mcl", str

stmt_expr = expr
```

# program

`program = ["#do once"], {stmt}`
