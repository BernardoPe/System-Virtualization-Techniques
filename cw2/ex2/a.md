### Exercise 2

**a) List and explain the sequence of calls performed by the program in x86-64/prog.s**

----

This is a program that executes the equivalent of a cat command in linux 
for the file /etc/os-release. It opens the file and prints out its contents to 
the standard output.

The program has a total of 5 calls.

The first call, openat(AT_FDCWD, "/etc/os-release", O_RDONLY) = 3,
is used to open a file descriptor for the file /etc/os-release in the current directory in read-only mode. 
The return of this call is 3, which is the value that represents the created file descriptor.

The second call, lseek(3, 0, SEEK_END) = 396, 
is used to point the file descriptor created in the previous call to the end of the file.
The return of this call is 396, which is the offset in bytes from the beginning of the file 
in bytes. In this case, since the file descriptor is now pointing to the end of the file
396 is the size of the file in bytes.

The third call, mmap(NULL, 396, PROT_READ, MAP_PRIVATE, 3, 0) is used 
to map the content of the first 396 bytes in file descriptor 3 to memory. In other words,
it maps the previously opened file's contents to memory. PROT_READ specifies that the created pages can be read,
and MAP_PRIVATE specifies that updates to the mapping are only visible to the current process, and do not 
affect the underlying file.

The fourth call, write(1, "PRETTY_NAME=\"Ubuntu 24.04 LTS\"\nN"..., 396PRETTY_NAME="Ubuntu 24.04 LTS"),
Writes the previously allocated file contents to the standard output.

The final call, exit_group(0) is used to terminate all the threads in the current process with status 0,
indicating a normal process exit status.