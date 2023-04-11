# corrupt 
## a utility to corrupt FILE streams without changes in file size.

### Usage

`corrupt -[hv] FILE...`

Flags

- `-h` to see the relevant usage information
- `-v` to get version information

### Example

- `corrupt testfile.txt` - corrupt `testfile.txt`

### Build

Simply `make` or if you don't have it - compile `corrupt.c` with the compiler of choice. The only dependency is the standart library. 

### License

MIT