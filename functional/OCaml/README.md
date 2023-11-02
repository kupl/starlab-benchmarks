OCaml
========

This is for the benchmarks written in OCaml.  
The name of each directory denotes the name of each OCaml problem.  
We have collected 300 benchmarks from 3 different problems.

### Error Type
All the benchmark programs have **funcionality errors** casuing assertion failure.


### Detail
Name      | Program Description                                  | # of bug
--------- | ---------------------------------------------------- | --------:
diff      | Symbolic Differentiation of an arithmetic expression |      100
lambda    | Checking the validity of a lambda term               |      100
formula   | Evaluation a propositional formula                   |      100

### Directory
Each Problem directory consists of two subdirectories (**bugs** and **src**) as follows:
```
OCaml
  |--- [Problem]
            |--- bugs
                   |--- bug_#
                           |--- bug.ml
                           |--- bug.json
            |--- src
                   |--- testcases
                   |--- test.ml
```
#### src
The **src** directory contains the following three contents:

Name        | Description
----------- | -----------
testcases   | subdirecotry storing all test cases
test.ml (optional)     | test driver code for running the test cases. If it is not necessary, it is not provided. 

Testcases is a set of input-output examples written in the following format:
```
{
  [input arguments separated by ';'] => [expected output];
  ...
}
```

#### bugs
Each directory (bug_#) includes one buggy code
