C
========

This is for the benchmarks written in C.  
The name of each directory denotes the name of the vulnerable program.  
We have collected ? vulnerabilities in ? programs.

### Error Types

We mainly focus on the **Memory Leak** vulnerabilities.

### Directory

Each directory consists of two subdirectories (bugs and src) as follows:

```
C
|--- PROG_NAME
         |--- src 
               |--- COMPILABLE_PROGRAM
         |--- bugs
               |--- bug_1.json
               |--- bug_2.json
               |--- ...
```

Name | Description
---- | -----------
src  | compilable program source codes.
bug  | bug info file in json format.

#### src
The **src** directory consists of compilable program source codes.  
To compile each program, follow the instruction(e.g., README.md) in each program.

#### bugs
The **bugs** directory consists of bug info files in json format.  
The contents of each json file are as follows:

Name      | Description
--------- | -----------
err_type  | the error type (e.g., memory leak)
source    | ?
sink      | ?
