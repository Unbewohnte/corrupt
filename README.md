# corrupt 
## a utility to corrupt FILE streams without changes in file size.

### Usage

`corrupt -[hvm] FILE...`

Flags

- `-h` to see the relevant usage information
- `-v` to get version information
- `-m METHOD (bit)` to specify corruption method (MORE TO BE ADDED)

### Examples

- `corrupt testfile.txt` - corrupt `testfile.txt` with default corruption method
- `corrupt -m bit testfile.txt` - corrupt `testfile.txt` with `bit` corruption method 

### Build

Simply `make` or if you don't have it - compile `corrupt.c` with the compiler of choice. The only dependency is the () standart library 

### License

MIT