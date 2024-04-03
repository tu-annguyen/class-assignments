# Assignment 7: Lempel-Ziv Compression
The purpose of this assignment is to create two programs, called encode and decode, which perform LZ78 compression and decompression, respectively. The encode program can compress any file, text, or binary, and the decode program can decompress any file, text, or binary that was compressed with encode.

Both programs operate on both little and big endian systems, use variable bit-length codes, and perform read and writes in efficient blocks of 4KB.

## Build
	$ make all

## Run
To encode a file:
	
	$ ./encode

To decode an encoded file:

	$ ./decode

### Program arguments
	-v: Print compression/decompression statistics to stderr.
	-i <input>: Specify input to compress/decompress (stdin by default).
	-o <output>: Specify output of compressed/decompressed input (stdout by default).

## Clean
	$ make clean

## Format
	$ make format
