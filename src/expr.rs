use std::str::FromStr;

use strum_macros::EnumString;

use nom::branch::{alt, permutation};
use nom::bytes::complete::{tag, take_till1, take_until};
use nom::character::complete::multispace0;
use nom::combinator::opt;
use nom::multi::{many0, separated_list0, separated_list1};
use nom::sequence::{delimited, preceded};
use nom::{IResult, Parser};

use crate::basic::{self, ident, symbol};

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
pub enum Primary {
    Num(i32),
    Ident(String),
    String(String),
    FuncCall(Box<Primary>, Vec<Primary>),

    Ranged(Box<Primary>, Box<Primary>),
    Pointer(Box<Primary>),
    CompileTime(Box<Primary>),

    ApplyOperator(BinaryOp, Box<Primary>, Box<Primary>),

    LogicalNot(Box<Primary>),
    BitwiseNot(Box<Primary>),
    Negative(Box<Primary>),

    Subscript(Box<Primary>, Box<Primary>),

    If {
        branches: Vec<(Primary, Primary)>,
        fallback: Option<Box<Primary>>,
    },
    Exprs(Vec<Primary>),
}

impl std::fmt::Display for Primary {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let s = match self {
            Self::Num(x) => format!("{}", x),
            Self::Ident(x) => format!("{}", x),
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
        };

        write!(f, "{}", s)
    }
}

impl Primary {
    fn _num_hex(input: &str) -> IResult<&str, Self> {
        let (input, _) = tag("0x")(input)?;
        let (input, num) = take_till1(|c: char| !c.is_digit(16))(input)?;

        Ok((input, Primary::Num(i32::from_str_radix(&num, 16).unwrap())))
    }
    fn _num_oct(input: &str) -> IResult<&str, Self> {
        let (input, _) = tag("0o")(input)?;
        let (input, num) = take_till1(|c: char| !c.is_digit(8))(input)?;

        Ok((input, Primary::Num(i32::from_str_radix(&num, 8).unwrap())))
    }
    fn _num_dec(input: &str) -> IResult<&str, Self> {
        let (input, num) = take_till1(|c: char| !c.is_digit(10))(input)?;
        Ok((input, Primary::Num(i32::from_str(&num).unwrap())))
    }
    fn _num_bin(input: &str) -> IResult<&str, Self> {
        let (input, _) = tag("0b")(input)?;
        let (input, num) = take_till1(|c: char| !c.is_digit(2))(input)?;

        Ok((input, Primary::Num(i32::from_str_radix(&num, 2).unwrap())))
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
            num.push_str(".");

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
                    multispace0,
                    tag(","),
                    multispace0,
                    tag("_"),
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
    fn _exprs(input: &str) -> IResult<&str, Self> {
        delimited(symbol('{'), many0(Self::read), symbol('}'))
            .map(|x| Self::Exprs(x))
            .parse(input)
    }
}

impl Primary {
    fn _binary_op<'a>(
        i: &'a str,
        mut joiner: impl FnMut(&'a str) -> IResult<&'a str, &'a str>,
        mut parser: impl FnMut(&'a str) -> IResult<&'a str, Primary> + Copy,
    ) -> IResult<&'a str, Primary> {
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
            preceded(basic::symbol('-'), Self::read.map(Box::new)).map(|e| Self::Negative(e)),
            preceded(basic::symbol('~'), Self::read.map(Box::new)).map(|e| Self::BitwiseNot(e)),
            preceded(basic::symbol('!'), Self::read.map(Box::new)).map(|e| Self::LogicalNot(e)),
            // CompileTime
            preceded(basic::symbol('@'), Self::read.map(Box::new)).map(|e| Self::CompileTime(e)),
            // []
            delimited(
                basic::symbol('['),
                Self::read.map(Box::new),
                basic::symbol(']'),
            )
            .map(|e| Self::Pointer(e)),
            //
            Self::_num,
            Self::_if,
            Self::_string,
            Self::_ident,
            Self::_exprs,
        ));

        let parser = permutation((parser, opt(delimited(symbol('['), Self::read, symbol(']')))))
            .map(|(array, index)| {
                if index.is_none() {
                    array
                } else {
                    Self::Subscript(Box::new(array), Box::new(index.unwrap()))
                }
            });

        let parser =
            permutation((parser, opt(preceded(tag("..."), Self::_primary)))).map(|(begin, end)| {
                if end.is_some() {
                    Self::Ranged(Box::new(begin), Box::new(end.unwrap()))
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
            if a.is_some() {
                Self::FuncCall(Box::new(p), a.unwrap())
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
            alt((tag("<"), tag(">"), tag("<="), tag(">="))),
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
