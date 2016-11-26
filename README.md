Description:
============

dcow is a possible exploit of the vulnerability CVE-2016-5195.
Running the program as unprivileged user on a vulnerable system, it'll modify the /etc/passwd file, forcing the password "dirtyCowFun" (SHA-512, but could be modified for older standards).
In case of successful execution, doing a "su" with that password, a root shell will be available.
Using the -s option, a root shell will be automatically opened.
A backup of the original /etc/passwd will be created in the current directory as .ssh_bak.

Prerequisites:
==============

A CVE-2016-5195 vulnerable system.

The program was successfully used with:

- RHEL7 Linux  x86_64;
- Debian 7 ("wheezy");
- Ubuntu 14.04.5 LTS
- Ubuntu 16.04.1 LTS
- Ubuntu 16.10
- Linux Mint 17.2

and compiled with: 

- clang version 4.0.0;
- gcc version 6.2.0 20161005 (Ubuntu 6.2.0-5ubuntu12) 
- gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.1) 
- gcc version 4.8.5 20150623 (Red Hat 4.8.5-4) (GCC);
- gcc version 4.8.4 (Ubuntu 4.8.4);
- gcc version 4.8.2 (Ubuntu 4.8.2-19ubuntu1)
- gcc version 4.7.2 (Debian 4.7.2-5);

Installation:
=============

- Compile the program:
  make

- Start the program:
  ./dcow
  or 
  ./dcow -s   # Automatically open a root shell

- Online help:

  ./dcow -h
