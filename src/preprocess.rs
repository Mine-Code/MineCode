pub fn preprocess(input: String) -> String {
    return input
        .split('\n')
        .map(|line| {
            let line = line.trim();
            if line.starts_with('#') || line.is_empty() {
                "".to_string()
            } else {
                line.to_string() + "\n"
            }
        })
        .collect::<Vec<String>>()
        .join("");
}
