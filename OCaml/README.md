OCaml
========

This is for the benchmarks written in OCaml.  
The name of each directory denotes the name of each OCaml problem.  
We have collected 100 benchmarks from 3 different problems.

### Error Type
All the benchmark programs have **funcionality errors** casuing assertion failure.


### Detail
Name      | Program Description                                  | # of bug
--------- | ---------------------------------------------------- | --------:
Problem 1 | Symbolic Differentiation of an arithmetic expression |       33
Problem 2 | Checking the validity of a lambda term               |       33
Problem 3 | Evluationa a propositional formula                   |       34

### Directory
Each Problem directory consists of two subdirectories (**src** and **bugs**) as follows:

```
OCaml
  |--- Problem #
            |--- src 
                   |--- solution.ml
                   |--- test.ml
                   |--- testcases
                           |--- tc1.ml
                           |--- tc2.ml
                           |--- ...
            |--- bugs
                   |--- bug_#
                           |--- bug.ml
                           |--- bug.json
```

Name  | Description
----- | -----------
src   | a solution program for each problem
bugs  | buggy programs for each problem

#### src
The **src** directory contains the following three contents:

Name        | Description
----------- | -----------
solution.ml | correct source code for each problem
test.ml     | test code for executing the given source program
testcases   | subdirecotry storing all test cases

A test case is an assertion which specifies the input and the expected output.   
(e.g., `assert ((test input) = output)`) 

#### bugs
Each buggy program (bug_#) consists of the following two files:

Name     | Description
-------- | -----------
bug.ml   | incorrect source code
bug.json | bug info file in json format.

Each json file reports the test cases that the buggy program fails to satisfy.
