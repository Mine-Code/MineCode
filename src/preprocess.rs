pub fn preprocess(input: String) -> String {
    return input
        .split("\n")
        .map(|line| {
            if line.starts_with("#") || line == "" {
                return "".to_string();
            } else {
                return line.to_string() + "\n";
            }
        })
        .collect::<Vec<String>>()
        .join("");
}
