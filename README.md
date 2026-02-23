# shell-ish
A simple shell implementation for Koc University Operating Systems (COMP 304) course.<br>
Github Repo: "https://github.com/udalgic23/shell-ish"<br>
shellish supports regular /usr/bin commands and 3 builtin commands "cut", "chatroom" and "dirsize"<br>

### How to use shellish
Step 1: Compile the source code by running make command in terminal<br>
Step 2: Run shellish executable by typing ./shellish<br>
Step 3: Use your shellish similar to a bash or sh process as you wish :D<br>

### How to use my custom command "dirsize"
dirsize command prints all directories and their sizes in a specified directory.<br>
To use dirsize just use the format "dirsize ["path"]" path variable is optional and set to current directory by default.<br>
Output is the all files and directories in the path with their sizes in disk as bytes.<br>
Size of directories are computed recursively.<br>

### References
Q1: How to get environment variables in C?<br>
A1: "https://stackoverflow.com/questions/31906192/how-to-use-environment-variable-in-a-c-program"<br>

Q2: How to check if a file exists or not in C?<br>
A2: "https://www.techgrind.io/explain/whats-the-best-way-to-check-if-a-file-exists-in-c"<br>

Q3: How to set up pipe in C?<br>
A3: "https://tldp.org/LDP/lpg/node11.html#:~:text=To%20create%20a%20simple%20pipe,be%20used%20for%20the%20pipeline"<br>

Q4: How to loop through every file in a directory in C?<br>
A4: "https://stackoverflow.com/questions/1271064/how-do-i-loop-through-all-files-in-a-folder-using-c"<br>
