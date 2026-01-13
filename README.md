# SnakeOS: An OS that only runs the Snake game
## How to run
To compile and run via [QEMU](https://qemu.org) on macOS/Linux, simply run:
```
$ make run
```
If you only want to compile:
```
$ make
```
## Dependencies
**To compile, you need the `i386-elf` toolchain.** Install it via [Homebrew](https://brew.sh/):
```
brew tap nativeos/i386-elf-toolchain
brew install nativeos/i386-elf-toolchain/i386-elf-binutils
brew install nativeos/i386-elf-toolchain/i386-elf-gcc
```
**To run, you need [QEMU](https://qemu.org)** (installation guide can be founde [here](https://www.qemu.org/download/#macos))
