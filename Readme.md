# ASO_EXT3_Simulator
Repository for the final work of the discipline Architecture of Operating Systems at UFSC, Araranguá campus. The work consists of simulating a Linux EXT3 file system, in order to understand its basic operating concepts.

<br>

## ⚠️ Some Cares
*If you are a student at UFSC and depending on your professor, this subject is fully protected against plagiarism. So think twice before just copying the works.*

---

*The entire structure of the work was created by Professor Martin and his monitors. The only files that were implemented in this work were: fs.cpp and auxFunction.hpp*

- To Compile: g++ *.cpp -o exe.out -g -lgtest -std=c++17 -lpthread
- To Run: ./exe.out
- To check for leaks: *valgrind --leak-check=full ./exe.out*

## Prerequisite for Linux

- [x] gtest library;
- [x] openssl library;
- [x] gcc library; and
- [x] gdb library.

## Prerequisite for Windows

- [x] Install msys2 available from https://www.msys2.org/;
- [x] In the MSYS terminal, run commands
	- pacman -S mingw-w64-x86_64-gcc
	- pacman -S mingw-w64-x86_64-gdb
	- pacman -S mingw-w64-x86_64-gtest
	- pacman -S mingw-w64-x86_64-openssl
- [x] Add mingw64 directory (in my case, C:\msys64\mingw64\bin) in Path environment variable;
- [x] Install VS Code available from https://code.visualstudio.com/;
- [x] Launch VS Code and install the "C/C++ for Visual Studio Code" extension from Microsoft.

<br>

## Status

	- [x] File system initialization - ok;
	- [x] Add File - ok;
	- [x] Add Directory - ok;
	- [] Remove File - in implementation;
	- [] Remove Directory - in implementation;
	- [] Move File - in implementation;
	- [] Move Directory - in implementation;
	- [] Rename file - in implementation;

<br>

## Cloning this Repository
1. On GitHub.com, navigate to the repository's main page.
2. Above the list of files, click code.
3. To clone the repository using HTTPS, under "Clone with HTTPS", click 📋. To clone the repository using an SSH key, including a certificate issued by your organization's SSH certificate authority, click Use SSH, then click 📋. To clone a repository using GitHub CLI, click Use GitHub CLI, then click 📋.
4. Open Git Bash.
5. Type git clone (clone git) and paste the URL you copied earlier.
```c
$ git clone https://github.com/theHprogrammer/ASO_EXT3_Simulator
```
6. Press Enter to create your local clone.

<br>

## 👨‍💻 Author

<table align="center">
    <tr>
        <td align="center">
            <a href="https://github.com/theHprogrammer">
                <img src="https://avatars.githubusercontent.com/u/79870881?v=4" width="150px;" alt="Helder's Image" />
                <br />
                <sub><b>Helder Henrique</b></sub>
            </a>
        </td>    
    </tr>
</table>
<h4 align="center">
   By: <a href="https://www.linkedin.com/in/theHprogrammer/" target="_blank"> Helder Henrique </a>
</h4>

