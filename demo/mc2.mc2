mcl minecraft
# Loads minecraft mcl file

@print("Hello world")
# Print "Hello world" in compile time

func print_text(text) {
  for i in 0...text.length {
    [0x10000000 + i] = text[i]
  }
  func[0x02000000](0x10000000)
}

print_text("Hello world!")
#   bl _print_text

@print_text("Hello world!")
#   r3 <- 0x10000000
#   r3 + 0x00 <- 0x48656c6c
#   r3 + 0x04 <- 0x6f20776f
#   r3 + 0x08 <- 0x726C6421
#   r3 + 0x0C <- 0x00
#   goto 0x02000000

for player in ServerPlayer.List {
    Local.Console << player.name
}