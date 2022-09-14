use strum_macros::EnumString;

#[derive(Debug, EnumString, strum_macros::Display, Clone, PartialEq, Eq)]
pub enum Keyword {
    #[strum(serialize = "AnyType")]
    AnyType,
    #[strum(serialize = "nil")]
    Nil,
    #[strum(serialize = "u32")]
    U32,
    #[strum(serialize = "u16")]
    U16,
    #[strum(serialize = "u8")]
    U8,
    #[strum(serialize = "i32")]
    I32,
    #[strum(serialize = "i16")]
    I16,
    #[strum(serialize = "i8")]
    I8,
    #[strum(serialize = "f32")]
    F32,
}

impl From<Keyword> for u8 {
    fn from(k: Keyword) -> Self {
        match k {
            Keyword::Nil => 0xf7,
            Keyword::AnyType => panic!("Keyword[AnyType] is cannot be bytecode"),
            Keyword::U32 => panic!("Keyword[U32] is cannot be bytecode"),
            Keyword::U16 => panic!("Keyword[U16] is cannot be bytecode"),
            Keyword::U8 => panic!("Keyword[U8] is cannot be bytecode"),
            Keyword::I32 => panic!("Keyword[I32] is cannot be bytecode"),
            Keyword::I16 => panic!("Keyword[I16] is cannot be bytecode"),
            Keyword::I8 => panic!("Keyword[I8] is cannot be bytecode"),
            Keyword::F32 => panic!("Keyword[F32] is cannot be bytecode"),
        }
    }
}
