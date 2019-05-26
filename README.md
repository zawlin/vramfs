# Intro
This is a fork from the very nice work here https://github.com/Overv/vramfs. As I needed a fast enough userland ramdisk, none of the existing fuse examples work reliably. 

A few changes from the vramfs:
* no more cl dependency and of course no more ability to allocate on gpu.
* convert to pure ramfs
* memory grow dynmically
* some bugs in the original where file permissions were not copied properly.
