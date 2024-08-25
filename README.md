# c6502 | 6502 CPU Emulator

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/a5c2e47531864725850a9e7e30705971)](https://app.codacy.com/gh/lvntky/c6502/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)

This is a simple emulator for the 6502 CPU written in C. The emulator allows you to run programs written for the 6502 architecture on modern systems.

## Features

- Accurate emulation of the 6502 CPU, including all official instructions and addressing modes.
- Support for common hardware devices such as RAM and ROM.
- Ability to load and execute binary programs written for the 6502.

## Requirements

To build and run the emulator, you'll need the following:

- CMake (version 3.22.1 or higher)
- A C compiler compatible with the C99 standard.
- A Linux System.
- Basic knowledge of the C programming language.

## Building the Emulator

To build the emulator using CMake, follow these steps:

1. Clone the repository or download the source code.
2. Navigate to the project directory.
3. Open a terminal or command prompt in the project directory.
4. Create a build directory: `mkdir build`
5. Build the project `cmake -B ./build -S ./`
6. Go to build directory: `make all`

This will generate the executable file for the emulator.

## Usage

To use the emulator, execute the generated binary from the command line. The following options are available:

- `./c6502 <filename>`: Load a binary program into memory.


## Contributing

Contributions are welcome! If you find any bugs or have suggestions for improvements, please open an issue or submit a pull request on the project's GitHub repository.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

- [Obelisk/Archive.org](https://web.archive.org/web/20190611003956/http://www.obelisk.me.uk/6502/)

## Contact

If you have any questions or need further assistance, feel free to reach out to the project maintainer at `leventkayadev@gmail.com`.

