use std::str::FromStr;

use strum_macros::EnumString;

use nom::branch::{alt, permutation};
use nom::bytes::complete::{tag, take_till1, take_until};
use nom::character::complete::multispace0;
use nom::combinator::opt;
use nom::multi::{many0, separated_list0, separated_list1};
use nom::sequence::{delimited, preceded};
use nom::{IResult, Parser};

use super::basic::{self, ident, symbol};

#[derive(Debug, EnumString, strum_macros::Display)]
pub enum BinaryOp {
    #[strum(serialize = "+")]
    Add,
    #[strum(serialize = "-")]
    Sub,
    #[strum(serialize = "*")]
    Mul,
    #[strum(serialize = "/")]
    Div,
    #[strum(serialize = "%")]
    Mod,
    #[strum(serialize = "**")]
    Power,
    #[strum(serialize = "||")]
    LogicalOr,
    #[strum(serialize = "&&")]
    LogicalAnd,
    #[strum(serialize = "|")]
    BitwiseOr,
    #[strum(serialize = "&")]
    BitwiseAnd,
    #[strum(serialize = "^")]
    BitwiseXor,
    #[strum(serialize = "<<")]
    ShiftLeft,
    #[strum(serialize = ">>")]
    ShiftRight,
    #[strum(serialize = "==")]
    Equal,
    #[strum(serialize = "!=")]
    NotEqual,
    #[strum(serialize = "<")]
    LessThan,
    #[strum(serialize = "<=")]
    LessThanOrEqual,
    #[strum(serialize = ">")]
    GreaterThan,
    #[strum(serialize = ">=")]
    GreaterThanOrEqual,
    #[strum(serialize = "=")]
    Assignment,
}

#[derive(Debug)]
pub enum Expr {
    Num(i32),
    Ident(String),
    String(String),
    FuncCall(Box<Expr>, Vec<Expr>),

    Ranged(Box<Expr>, Box<Expr>),
    Pointer(Box<Expr>),
    CompileTime(Box<Expr>),

    ApplyOperator(BinaryOp, Box<Expr>, Box<Expr>),
    SubExpr(Box<Expr>),

    LogicalNot(Box<Expr>),
    BitwiseNot(Box<Expr>),
    Negative(Box<Expr>),

    Subscript(Box<Expr>, Box<Expr>),

    If {
        branches: Vec<(Expr, Expr)>,
        fallback: Option<Box<Expr>>,
    },
    For {
        name: Box<Expr>,
        iter: Box<Expr>,
        body: Box<Expr>,
        value: Option<Box<Expr>>,
    },

    Exprs(Vec<Expr>),
}

impl std::fmt::Display for Expr {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let s = match self {
            Self::Num(x) => x.to_string(),
            Self::Ident(x) => x.to_string(),
            Self::String(x) => format!("{}", x.escape_default()),
            Self::FuncCall(func, args) => format!(
                "{}({:?})",
                func,
                args.iter()
                    .map(|x| format!("{x}"))
                    .fold("".to_string(), |a, c| a + &c)
            ),
            Self::Ranged(begin, end) => format!("Range<{} -> {}>", begin, end),
            Self::Pointer(x) => format!("Ptr<{}>", x),
            Self::CompileTime(x) => format!("CompileTime<{}>", x),
            Self::ApplyOperator(op, r, l) => format!("{} {} {}", r, op, l),
            Self::LogicalNot(x) => format!("!{}", x),
            Self::BitwiseNot(x) => format!("~{}", x),
            Self::Negative(x) => format!("-{}", x),
            Self::Subscript(arr, ind) => format!("{}[{}]", arr, ind),
            Self::If { branches, fallback } => format!(
                "if {}{}",
                branches
                    .iter()
                    .fold("".to_string(), |a, (c, e)| a + &format!("{} => {}, ", c, e)),
                if let Some(fallback) = fallback {
                    format!("_ => {}", fallback)
                } else {
                    "".to_string()
                }
            ),
            Self::Exprs(exprs) => format!(
                "{{{}}}",
                exprs
                    .iter()
                    .fold("".to_string(), |a, c| a + &format!("{}; ", c)),
            ),
            Self::SubExpr(expr) => format!("({})", expr),
            Self::For {
                name,
                iter,
                body,
                value,
            } => {
                format!(
                    "for {} in {}: [{}] {}",
                    name,
                    iter,
                    body,
                    if let Some(x) = value {
                        format!("=> {}", x)
                    } else {
                        "".to_string()
                    }
                )
            }
        };

        write!(f, "{}", s)
    }
}

impl Expr {
    fn _num_hex(input: &str) -> IResult<&str, Self> {
        let (input, _) = tag("0x")(input)?;
        let (input, num) = take_till1(|c: char| !c.is_ascii_hexdigit())(input)?;

        Ok((input, Expr::Num(i32::from_str_radix(num, 16).unwrap())))
    }
    fn _num_oct(input: &str) -> IResult<&str, Self> {
        let (input, _) = tag("0o")(input)?;
        let (input, num) = take_till1(|c: char| !c.is_digit(8))(input)?;

        Ok((input, Expr::Num(i32::from_str_radix(num, 8).unwrap())))
    }
    fn _num_dec(input: &str) -> IResult<&str, Self> {
        let (input, num) = take_till1(|c: char| !c.is_ascii_digit())(input)?;
        Ok((input, Expr::Num(i32::from_str(num).unwrap())))
    }
    fn _num_bin(input: &str) -> IResult<&str, Self> {
        let (input, _) = tag("0b")(input)?;
        let (input, num) = take_till1(|c: char| !c.is_digit(2))(input)?;

        Ok((input, Expr::Num(i32::from_str_radix(num, 2).unwrap())))
    }
    fn _num(input: &str) -> IResult<&str, Self> {
        delimited(
            multispace0,
            alt((
                Self::_num_hex,
                Self::_num_oct,
                Self::_num_dec,
                Self::_num_bin,
            )),
            multispace0,
        )(input)
    }
    fn _ident(input: &str) -> IResult<&str, Self> {
        let (mut input, mut num) = ident(input)?;

        loop {
            let ret = symbol('.')(input);
            if ret.is_err() {
                break;
            }

            let tmp = ret.unwrap();
            input = tmp.0;
            num.push('.');

            let ret = ident(input);
            if ret.is_err() {
                break;
            }
            let tmp = ret.unwrap();
            input = tmp.0;
            num.push_str(&tmp.1);
        }

        Ok((input, Self::Ident(num)))
    }
    fn _string(input: &str) -> IResult<&str, Self> {
        let (input, s) =
            delimited(basic::symbol('"'), take_until("\""), basic::symbol('"'))(input)?;

        Ok((input, Self::String(s.to_string())))
    }
    fn _if(input: &str) -> IResult<&str, Self> {
        let (input, _) = tag("if")(input)?;

        permutation((
            multispace0,
            separated_list1(
                tag(","),
                permutation((Self::read, multispace0, tag("=>"), multispace0, Self::read))
                    .map(|(a, _, _, _, c)| (a, c)),
            ),
            opt(permutation((
                permutation((
                    symbol(','),
                    symbol('_'),
                    multispace0,
                    tag("=>"),
                    multispace0,
                )),
                Self::read.map(Box::new),
            ))
            .map(|(_, a)| a)),
        ))
        .map(|(_, branches, fallback)| Self::If { branches, fallback })
        .parse(input)
    }
    fn _for(input: &str) -> IResult<&str, Self> {
        let (input, _) = tag("for")(input)?;

        permutation((
            Self::read.map(Box::new),
            multispace0,
            tag("in"),
            multispace0,
            Self::read.map(Box::new),
            multispace0,
            Self::read.map(Box::new),
            opt(permutation((
                multispace0,
                tag("=>"),
                multispace0,
                Self::read.map(Box::new),
            ))
            .map(|(_, _, _, a)| a)),
        ))
        .map(|(name, _, _, _, iter, _, body, value)| Self::For {
            name,
            iter,
            body,
            value,
        })
        .parse(input)
    }
    fn _exprs(input: &str) -> IResult<&str, Self> {
        delimited(symbol('{'), many0(Self::read), symbol('}'))
            .map(Self::Exprs)
            .parse(input)
    }
}

impl Expr {
    fn _binary_op<'a>(
        i: &'a str,
        mut joiner: impl FnMut(&'a str) -> IResult<&'a str, &'a str>,
        mut parser: impl FnMut(&'a str) -> IResult<&'a str, Expr> + Copy,
    ) -> IResult<&'a str, Expr> {
        let (i, mut r) = parser(i)?;

        let b = joiner(i);
        if b.is_err() {
            return Ok((i, r));
        }

        let (i, parts) = many0(permutation((
            delimited(multispace0, joiner, multispace0),
            parser,
        )))(i)?;

        for (op, x) in parts {
            r = Self::ApplyOperator(BinaryOp::from_str(op).unwrap(), Box::new(r), Box::new(x));
        }
        Ok((i, r))
    }

    fn _primary(input: &str) -> IResult<&str, Self> {
        if input.trim().is_empty() {
            return Err(nom::Err::Error(nom::error::Error::new(
                input,
                nom::error::ErrorKind::Eof,
            )));
        }
        let parser = alt((
            preceded(basic::symbol('-'), Self::read.map(Box::new)).map(Self::Negative),
            preceded(basic::symbol('~'), Self::read.map(Box::new)).map(Self::BitwiseNot),
            preceded(basic::symbol('!'), Self::read.map(Box::new)).map(Self::LogicalNot),
            // CompileTime
            preceded(basic::symbol('@'), Self::read.map(Box::new)).map(Self::CompileTime),
            // []
            delimited(
                basic::symbol('['),
                Self::read.map(Box::new),
                basic::symbol(']'),
            )
            .map(Self::Pointer),
            // SubExpr
            delimited(
                basic::symbol('('),
                Self::read.map(Box::new),
                basic::symbol(')'),
            )
            .map(Self::SubExpr),
            //
            Self::_num,
            Self::_for,
            Self::_if,
            Self::_string,
            Self::_ident,
            Self::_exprs,
        ));

        let parser = permutation((parser, opt(delimited(symbol('['), Self::read, symbol(']')))))
            .map(|(array, index)| {
                if let Some(x) = index {
                    Self::Subscript(Box::new(array), Box::new(x))
                } else {
                    array
                }
            });

        let parser =
            permutation((parser, opt(preceded(tag("..."), Self::_primary)))).map(|(begin, end)| {
                if let Some(x) = end {
                    Self::Ranged(Box::new(begin), Box::new(x))
                } else {
                    begin
                }
            });
        let mut parser = permutation((
            parser,
            opt(delimited(
                basic::symbol('('),
                separated_list0(basic::symbol(','), Self::read),
                basic::symbol(')'),
            )),
        ))
        .map(|(p, a)| {
            if let Some(x) = a {
                Self::FuncCall(Box::new(p), x)
            } else {
                p
            }
        });

        parser.parse(input)
    }

    fn _pow(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(input, tag("**"), Self::_primary)
    }
    fn _level_shift(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(input, alt((tag("<<"), tag(">>"))), Self::_pow)
    }
    fn _bitwise(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(
            input,
            alt((tag("|"), tag("&"), tag("^"))),
            Self::_level_shift,
        )
    }
    fn _term(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(input, alt((tag("*"), tag("/"), tag("%"))), Self::_bitwise)
    }
    fn _math_expr(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(input, alt((tag("+"), tag("-"))), Self::_term)
    }
    fn _size_comp(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(
            input,
            alt((tag("<="), tag(">="), tag("<"), tag(">"))),
            Self::_math_expr,
        )
    }
    fn _equ_neq(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(input, alt((tag("!="), tag("=="))), Self::_size_comp)
    }
    fn _logical_and(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(input, tag("&&"), Self::_equ_neq)
    }
    fn _logical_or(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(input, tag("||"), Self::_logical_and)
    }
    fn _assignment(input: &str) -> IResult<&str, Self> {
        Self::_binary_op(input, tag("="), Self::_logical_or)
    }
    pub fn read(input: &str) -> IResult<&str, Self> {
        let (i, a) = Self::_assignment(input)?;
        let (i, _) = opt(tag(";"))(i)?;
        Ok((i, a))
    }
}
