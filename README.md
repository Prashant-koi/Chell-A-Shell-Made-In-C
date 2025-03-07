# Chell

A simple Unix-like shell implementation in C (currently in early development).

## Features (In Progress)

This project is still in its initial stages. The following features are planned and partially implemented:

- [x] Basic command execution  
- [ ] Built-in commands (`cd`, `help`, `exit`)  
- [ ] Support for pipelines (`|`)  
- [ ] Input/output redirection (`<`, `>`, `>>`)  
- [ ] Background process execution (`&`)  
- [ ] Command history  
- [ ] Signal handling (`Ctrl+C`)  

## Building

To compile **Chell**, run the following command in a Unix-like terminal (**MSYS2 for Windows users**):

```bash
make
```

This will generate an executable named **`chell`** on Unix/macOS or **`chell.exe`** on Windows.

## Usage

After building, you can start the shell by running:

```bash
./chell      # On Unix/macOS
chell.exe    # On Windows
```

You should see the prompt:

```
chell> 
```

You can now enter commands as you would in a typical shell.

## Built-in Commands (Planned)

- `cd [directory]` - Change the current directory  
- `help` - Display help information  
- `exit` - Exit the shell  
- `history` - Show command history  

## External Commands

The shell currently supports running any program found in your system's `PATH`.  
Example:

```bash
chell> ls
chell> echo "Hello, Chell!"
```

## Planned Advanced Features

- Piping between commands (`ls | grep file`)  
- Input/output redirection (`ls > output.txt`)  
- Background process execution (`./script.sh &`)  
- Command history (`history`)  
- Signal handling (`Ctrl+C` to terminate processes)  

## Project Structure

- `chell.c` - Main shell implementation  
- `chell_extensions.c` - Additional features (in the future)  
- `Makefile` - Build configuration  
 
## License

[MIT License](LICENSE)
