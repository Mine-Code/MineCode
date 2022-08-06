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

impl BinaryOp {
    pub fn calculate(&self, a: i32, b: i32) -> i32 {
        match self {
            BinaryOp::Add => a + b,
            BinaryOp::Sub => a - b,
            BinaryOp::Mul => a * b,
            BinaryOp::Div => a / b,
            BinaryOp::Mod => a % b,
            BinaryOp::Power => {
                if b >= 0 {
                    a.pow(b as u32)
                } else {
                    1 / a.pow((-b) as u32)
                }
            }
            BinaryOp::LogicalOr => {
                if (a != 0) || (b != 0) {
                    1
                } else {
                    0
                }
            }
            BinaryOp::LogicalAnd => {
                if (a != 0) && (b != 0) {
                    1
                } else {
                    0
                }
            }
            BinaryOp::BitwiseOr => a | b,
            BinaryOp::BitwiseAnd => a & b,
            BinaryOp::BitwiseXor => a ^ b,
            BinaryOp::ShiftLeft => a << b,
            BinaryOp::ShiftRight => a >> b,
            BinaryOp::Equal => {
                if a == b {
                    1
                } else {
                    0
                }
            }
            BinaryOp::NotEqual => {
                if a != b {
                    1
                } else {
                    0
                }
            }
            BinaryOp::LessThan => {
                if a < b {
                    1
                } else {
                    0
                }
            }
            BinaryOp::LessThanOrEqual => {
                if a <= b {
                    1
                } else {
                    0
                }
            }
            BinaryOp::GreaterThan => {
                if a > b {
                    1
                } else {
                    0
                }
            }
            BinaryOp::GreaterThanOrEqual => {
                if a >= b {
                    1
                } else {
                    0
                }
            }
            BinaryOp::Assignment => a,
        }
    }
}
