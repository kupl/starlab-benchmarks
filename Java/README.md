Java
========

This is for the benchmarks written in Java.  
The name of each directory denotes the name of the vulnerable project.  
We have collected ? vulnerabilities in ? projects.

### Error Types

We mainly focus on the **Null Point Exception** vulnerability.

### Directory

Each directory consists of two subdirectories (bugs and src) as follows:

```
C
|--- PROJECT_NAME
         |--- src 
               |--- COMPILABLE_PROJECT
         |--- bugs
               |--- FILE_1
                      |--- bug.java
                      |--- bug.json
               |--- FILE_2
               |--- ...
```

Name | Description
---- | -----------
src  | compilable Java project source codes ???.
bugs | subdirectories for the bug info and failing test case.

#### src
The **src** directory consists of compilable Java project source codes with the errors.  
To compile each project, follow the instruction(e.g., README.md) in each project.

#### bugs
The **bugs** directory consists of subdirectories which include the failing test case(**bug.java**) and bug info file(**bug.json**) in json format.

Name       | Description
---------- | -----------
bug.java   | a compilable source code which triggers the error.
bug.json   | a bug info file


The contents of each **bug.json** are as follows:

Name        | Description
----------- | -----------
file_path   | the exact path to the file where the error occurs
line        | the exact line number where the error occurs
npe_method  | ?
deref_field | ?
npe_class   | ? 
