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
               |--- COMPILABLE_PROJECT_1
               |--- COMPILABLE_PROJECT_2
         |--- bugs
               |--- bug_1.json
               |--- bug_2.json
               |--- ...
```

Name | Description
---- | -----------
src  | compilable Java project source codes ???.
bug  | bug info file in json format.

#### src
The **src** directory consists of compilable Java project source codes.  

#### bugs
The **bugs** directory consists of bug info files in json format.  
The contents of each json file are as follows:

Name        | Description
----------- | -----------
file_path   | the exact path to the file where the error occurs
line        | the exact line number where the error occurs
npe_method  | ?
deref_field | ?
npe_class   | ? 
