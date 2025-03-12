# Chell

A lightweight Unix-like shell implementation in C, designed for both Windows and Unix environments.

## Current Features

- [x] Basic command execution
- [x] Cross-platform support (Windows and Unix-based systems)
- [x] Windows `ls` command (automatically translates to `dir`)
- [x] Custom line reading and parsing implementation
- [x] Simple error handling
- [x] Built-in commands (`cd`, `help`, `exit`)

## Planned Features

- [ ] Support for pipelines (`|`)
- [ ] Input/output redirection (`<`, `>`, `>>`)
- [ ] Background process execution (`&`)
- [ ] Command history
- [ ] Tab completion
- [ ] Signal handling (`Ctrl+C`)
- [ ] Environment variable support

## Building

To compile Chell, run the following command in a terminal:

### Unix/macOS
```bash
make
```

### Windows (MSYS2/UCRT64)
```bash
make
```

This will generate an executable named `chell` on Unix/macOS or `chell.exe` on Windows.

## Usage

After building, you can start the shell by running:

```bash
./chell      # On Unix/macOS
./chell.exe  # On Windows
```

You should see the prompt:

```
chell> 
```

You can now enter commands as you would in a typical shell.

## Command Support

### Current Commands
- Any system command available in your PATH
- Special handling for `ls` on Windows (converts to `dir`)
- `exit` - Exit the shell
- `cd [directory]` - Change the current directory
- `help` - Display help information

### Planned Built-in Commands
- `history` - Show command history

## Project Structure

- `chell.c` - Main shell implementation with command parsing and execution
- `chell_extensions.c` - Additional features and built-in commands
- `makefile` - Build configuration

## Troubleshooting

- If you get "Permission denied" when compiling on Windows, ensure `chell.exe` is not currently running
- Use `rm -f chell.exe` before running `make` if you encounter compilation issues

## License

[MIT License](LICENSE)

## Contributing

Contributions are welcome! Feel free to submit pull requests or open issues for new features, bug fixes, or improvements.