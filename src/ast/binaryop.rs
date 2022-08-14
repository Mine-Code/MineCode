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
            BinaryOp::LessThan => 4,
            BinaryOp::LessThanOrEqual => 5,
            BinaryOp::GreaterThan => panic!("Please use LessThan insted of GreaterThan"),
            BinaryOp::GreaterThanOrEqual => {
                panic!("Please use LessThanOrEqual insted of GreaterThanOrEqual")
            }
            BinaryOp::Equal => 6,
            BinaryOp::NotEqual => 7,
            BinaryOp::BitwiseAnd | BinaryOp::LogicalAnd => 8,
            BinaryOp::BitwiseOr | BinaryOp::LogicalOr => 9,
            BinaryOp::BitwiseXor => 10,
            BinaryOp::Mod => 11,
            BinaryOp::Power => panic!("Please use Pow() insted of **"),
            BinaryOp::ShiftLeft => 12,
            BinaryOp::ShiftRight => 13,
            BinaryOp::Assignment => panic!("[Assignment] can't be opcode"),
        }
    }
}
