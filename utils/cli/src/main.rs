use tokio::io::{AsyncReadExt, AsyncWriteExt};
use tokio::net::TcpStream;
use rustyline::{Editor, error::ReadlineError, Config, history::FileHistory};
use std::error::Error;
use std::path::PathBuf;

const HISTORY_FILE: &str = "history.txt";
const HISTORY_LIMIT: usize = 100;

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    let addr = "127.0.0.1:6378";

    let mut stream = TcpStream::connect(addr).await?;
    println!("Connected to LittleDB at {}", addr);

    let mut buffer = vec![0; 1024];
    let n = stream.read(&mut buffer).await?;
    println!("{}", String::from_utf8_lossy(&buffer[..n]));

    let config_builder = Config::builder()
        .history_ignore_space(true)
        .history_ignore_dups(true);

    let config = config_builder.expect("Failed to initialize Config builder").build();

    let mut editor = Editor::<(), FileHistory>::with_config(config)?;

    let history_path = PathBuf::from(HISTORY_FILE);
    if history_path.exists() {
        editor.load_history(&history_path).ok();
    }

    loop {
        let readline = editor.readline(&format!("{}> ", addr));

        match readline {
            Ok(command) => {
                let trimmed_command = command.trim();

                if trimmed_command.is_empty() {
                    continue;
                }

                if trimmed_command.eq_ignore_ascii_case("quit") {
                    break;
                } else if trimmed_command.eq_ignore_ascii_case("help") {
                    println!("Available commands:");
                    println!("  SET <key> <value>  - Set a key-value pair");
                    println!("  GET <key>          - Get the value for a key");
                    println!("  HELP               - Show this help message");
                    println!("  QUIT               - Exit the CLI");
                    continue;
                }

                stream.write_all(format!("{}\n", trimmed_command).as_bytes()).await?;

                let n = stream.read(&mut buffer).await?;
                if n == 0 {
                    println!("Connection closed by the server.");
                    break;
                }
                println!("{}", String::from_utf8_lossy(&buffer[..n]));

                editor.add_history_entry(trimmed_command);
            }
            Err(ReadlineError::Interrupted) => {
                continue;
            }
            Err(ReadlineError::Eof) => {
                break;
            }
            Err(err) => {
                println!("Error: {:?}", err);
                break;
            }
        }
    }

    editor.save_history(&history_path).ok();

    Ok(())
}
