/*
The MIT License (MIT)

Copyright © 2023 Kasyanov Nikolay Alexeyevich (Unbewohnte)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
corrupt.c - a utility to corrupt FILE streams
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>

// Corruption method. TODO(add more)
typedef enum Corruption {
    CORRUPTION_BITMAGIC, // Corrupt each byte via bit manipulation
} Corruption;

// Possible funcion errors
typedef enum CorruptResult {
    SUCCESS,
    ERR_NULLPTR,
} CorruptResult;


// Corrupts byte via bit manipulation
int corrupt_char(int byte) {
    static int last;
    int corrupted_byte = ((byte << 1) | last) + (byte ^ last);
    last = byte;
    return corrupted_byte;
}

/*
Corrupts the file with specified corruption method. Returns 0 if 
successful, 1 - if FILE is NULL.
*/
CorruptResult corrupt(FILE* file, Corruption corruption) {
    if (!file) {
        return ERR_NULLPTR;
    } else if (feof(file)) {
        rewind(file);
    }

    long long read_bytes = 0;
    char buff[1024];
    char corrupted_buf[1024];
    while (1) {
        // read chunk
        read_bytes = fread(buff, 1, 1024, file);

        // set cursor at the beginning        
        fseek(file, -read_bytes, SEEK_CUR);

        // replace the original bytes with corrupted ones 
        if (corruption == CORRUPTION_BITMAGIC) {
            for (unsigned int i = 0; i < read_bytes; i++) {
               corrupted_buf[i] = corrupt_char(buff[i]);
            }
            fwrite(corrupted_buf, 1, read_bytes, file);
        }

        if (read_bytes < 1024 || feof(file)) {
            break;
        }
    }

    return SUCCESS;
}

int main(int argc, char** argv) {
    char* file_paths[argc-1];
    unsigned int file_path_index = 0;
    FILE* file = NULL;
    Corruption corruption_method = CORRUPTION_BITMAGIC;
    CorruptResult result;

    unsigned int i = 1;
    while (i < argc) {
        // handle specified flags
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            fprintf(
                stdout, 
                "corrupt -[hvm] FILE...\n\n-h --help -> Print this message and exit\n-v --version -> Print version information and exit\n-m --method METHOD (bit) -> Specify corruption method\n"
            );
            return SUCCESS;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            fprintf(
                stdout,
                "corrupt v0.1.1 - a utility to corrupt FILE streams\n(c) Kasyanov Nikolay Alexeyevich (Unbewohnte)\n"
            );
            return SUCCESS;
        } else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--method") == 0) {
            i++;
            if (strcmp(argv[i], "bit") == 0) {
                corruption_method = CORRUPTION_BITMAGIC;
            } else {
                corruption_method = CORRUPTION_BITMAGIC;
                fprintf(
                    stdout,
                    "Invalid corruption method \"%s\". Defaulting to \"bit\"\n",
                    argv[i]
                );
            }
        } else {
            // remember this path
            file_paths[file_path_index] = argv[i];
            file_path_index++;
        }

        i++;
    }

    if (argc == 1 || file_path_index == 0) {
        fprintf(stdout, "No FILEs were specified. Run \"corrupt -h\" to get help\n");
        return SUCCESS;
    }

    for (unsigned int i = 0; i < file_path_index; i++) {
        // open stream
        file = fopen(file_paths[i], "r+");
        
        fprintf(
            stdout,
            "Corrupting \"%s\" ... ",
            file_paths[i]
        );

        // corrupt the whole stream
        result = corrupt(file, corruption_method);
        if (result == ERR_NULLPTR) {
            fprintf(
                stdout, 
                "error: %s\n", 
                strerror(errno)
            );
            continue;
        }

        // close stream
        fclose(file);

        fprintf(
            stdout,
            "done\n"
        );
    }

    return SUCCESS;
}