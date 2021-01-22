
# Madison Adams' Projects



## Animal Recognition (AniRec) - SPRING 2020
* About : Group project utilizing Mobile App Development (frontend) and Machine Learning (backend), connected using Firebase; built and deployed a Convolutional Neural Network (CNN) model that identifies the type of animal in an upload image
* How to Run : 
  1. Navigate to directory
  2. Use Jupyter Notebook to run the AniRec Model
  3. Use Android Studio to run the AniRec app (build it, run it, sign up, login, and follow instructions in app)


## FAT32 File System - FALL 2020
* About : Shell that can navigate and read through a FAT32 file system programmed in C and provides 10 different commands to use with the disk image
* How to Run : 
  1. Navigate to directory
  2. Add a FAT32 disk image ([img_name].img file)
  3. In a terminal/command window, compile and run mfs.c ("gcc mfs.c")
  4. Open the image ("open [img_name].img")
  5. Enter any of the available commands ("bpb", "stat [filename]", "get [filename]" or "get [filename] [new_filename], "cd [directory_name]" or "cd ..", "ls" or "ls ." or "ls ..", "read [filename] [position] [number_of_bytes]")
      * The filename must be a file in the disk image
      * If you use new_filename, it will retrieve the file with filename with the new_filename as the filename (get filename as new_filename)
      * Commands entered must follow the formats listed above in 5
  6. Close the image when done ("close")
  7. Exit the command line ("quit" or "exit")


## malloc (Heap Management) - FALL 2020
About : Utilizes a linked list to keep track of free blocks in order to perform heap management; It performs the malloc, calloc, and realloc commands, using an allocation algorithm choice of best fit, worst fit, next fit, or first fit. This was programmed and ran on a Linux Fedora (64-bit) Linux virtual machine using Virtual Box.
How to Run :
  1. Navigate to directory
  2. Compile "malloc.c" and enter the command "env LD_PRELOAD=lib/libmalloc-[fit_name].so tests/[test_name].c"
  3. Run "bfwf.c" for testing best fit (bf) and worst fit (wf) or "ffnf.c" for testing first fit (ff) and next fit (nf). Make sure to change the fit_name and test_name and enter the one you want to test.
      * First Fit: "env LD_PRELOAD=lib/libmalloc-ff.so tests/ffnf.c"
      * Next Fit: "env LD_PRELOAD=lib/libmalloc-nf.so tests/ffnf.c"
      * Best Fit: "env LD_PRELOAD=lib/libmalloc-bf.so tests/bfwf.c"
      * Worst Fit: "env LD_PRELOAD=lib/libmalloc-wf.so tests/bfwf.c"


## MavShell (msh) - FALL 2020
About : Shell that supports up to 10 command-line arguments that performs different commands, including “history”, “ls” and “cd” (programmed in C)
How to Run : 
  1. Navigate to directory
  2. In a terminal/command window, compile and run msh.c ("gcc msh.c")
  3. Enter any of the available commands ("![command#_from_history]" - performs up to 15 previous commands which are stored in the history of the shell, "history", "cd [directory_name]", "showpids" - shows process id's of previously ran processes (ran using execvP)", other commands which are ran using fork() and execvP() searching the current working directory first, then /usr/local/bin, then /local/bin, and finally /bin)
  4. Exit the command line ("quit" or "exit")

## Slash-Trash - FALL 2020
* About : Group project utilizing Website Development (PHP and HTML) and Database Management (SQL), connected using XAMPP and MySQL; focusing on the reduce portion of Reduce, Reuse, Recycle, it focuses on reducing trash produced by food establishments on a daily basis. It promotes the use of reusable items by providing incentives to customers. Keeps track of the reusable items used, points gained by the customers, and also the waste points saved by the establishments.
* How to Run (using XAMPP): 
  1. Place "Slash-Trash" directory in htdocs folder
  2. Navigate to directory
  3. Run "Initialize_Data.php" by navigating to (/src/utils/Initialize_Data.php) and loading it in a web browser. This will initialize the database and fill it with test data
  4. Navigate to the website by navigating to (/src/index.html) and loading "index.html" in web browser. Reference the "How to Use Slash-Trash?" section and follow the instructions provided


## System Call - FALL 2020
* About : Utilizes two system calls added to the Linux kernel, "hello", and “procstat", which takes a process id (pid) and a pointer to a struct and returns the pid’s information/stats in the struct back to the user (utilizing user and kernel mode). This was programmed and ran on a Linux Fedora (64-bit) Linux virtual machine using Virtual Box.
* How to Run : 
  1. Navigate to directory
  2. Decompress "system_call_2020-10-20.tgz"
  3. In a terminal/command window, first run "make", then run "./proc_stat_test [pid_number]"


## License (MIT License)

Copyright (c) 2021 Madison Adams (madileigh)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.