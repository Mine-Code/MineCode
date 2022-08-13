use strum_macros::EnumString;

#[derive(Debug, EnumString, strum_macros::Display, Clone, PartialEq, Eq)]
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

impl Into<u8> for BinaryOp {
    fn into(self) -> u8 {
        match self {
            BinaryOp::Add => 0,
            BinaryOp::Sub => 1,
            BinaryOp::Mul => 2,
            BinaryOp::Div => 3,
            BinaryOp::Mod => 4,
            BinaryOp::Power => 5,
            BinaryOp::LogicalOr => 6,
            BinaryOp::LogicalAnd => 7,
            BinaryOp::BitwiseOr => 8,
            BinaryOp::BitwiseAnd => 9,
            BinaryOp::BitwiseXor => 10,
            BinaryOp::ShiftLeft => 11,
            BinaryOp::ShiftRight => 12,
            BinaryOp::Equal => 13,
            BinaryOp::NotEqual => 14,
            BinaryOp::LessThan => 15,
            BinaryOp::LessThanOrEqual => 16,
            BinaryOp::GreaterThan => 17,
            BinaryOp::GreaterThanOrEqual => 18,
            BinaryOp::Assignment => 19,
        }
    }
}
