# Assignment 5, SS ENCRYPTION
## Alexander Ragland #1858717 ID: adraglan
### Build, run, and clean repository
```
$ make all
$ ./keygen [OPTIONS]
$ ./encrypt [OPTIONS]
$ ./decrypt [OPTIONS]
$ make clean
```
### Files
- Makefile
- README.md
- WRITEUP.pdf
- DESIGN.pdf
- keygen.c
- encrypt.c
- decrypt.c
- ss.c
- numtheory.c
- randstate.c
- ss.h
- numtheory.h
- randstate.h
### Note
The Makefile specifies "clang-15" because I'm on linux mint, which is on an older version of Ubuntu. Because of this, I had to install "clang-15" separately from "clang".