# Chell

A simple Unix shell implementation in C (currently in early development).

## Planned Features

This project is in its initial stages. The following features are planned for implementation:

- Basic command execution
- Built-in commands (cd, help, exit)
- Support for pipelines (|)
- Input/output redirection (<, >, >>)
- Background process execution (&)
- Command history
- Signal handling (Ctrl+C)

## Building (Coming Soon)

Once the initial implementation is complete:
```bash
make
```

## Usage (Coming Soon)

After building:
```bash
./chell
```

## Planned Implementation

### Built-in Commands
- `cd [directory]` - Change current directory
- `help` - Display help information
- `exit` - Exit the shell
- `history` - Show command history

### External Commands
The shell will support running any program in your PATH.

### Advanced Features
The following features will be added incrementally:
- Piping between commands
- Input/output redirection
- Background process execution
- Command history

## Project Structure

- `chell.c` - Main shell implementation (currently in development)
- `chell_extensions.c` - Additional features (to be implemented)
- `Makefile` - Build configuration

## Development Roadmap

1. Implement basic shell functionality
2. Add built-in commands
3. Implement command parsing and execution
4. Add piping and redirection
5. Implement background processes
6. Add command history
7. Add signal handling

## License

[MIT License](LICENSE)