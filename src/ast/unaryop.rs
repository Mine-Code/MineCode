use strum_macros::EnumString;

#[derive(Debug, EnumString, strum_macros::Display, Clone, PartialEq, Eq)]
pub enum UnaryOp {
    #[strum(serialize = "&")]
    Reference,
    #[strum(serialize = "*")]
    DeReference,
    #[strum(serialize = "!")]
    LogicalNot,
    #[strum(serialize = "~")]
    BitwiseNot,
    #[strum(serialize = "-")]
    Negative,
}
