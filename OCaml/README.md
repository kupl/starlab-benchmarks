OCaml
========

This is for the benchmarks written in OCaml.
The name of each directory denotes the name of each OCaml problem.
We have collected 100 benchmarks from 3 different problems.

### Error Type

All the benchmark programs have **funcionality errors** casuing assertion failure.

### Directory

Each Problem directory consists of two subdirectories (src and bugs) as follows:

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

The src directory contains the following three contents:

Name        | Description
----------- | -----------
solution.ml | correct source code for each problem
test.ml     | test code for executing given source
testcases   | direcotry storing all test cases

A test case is an assertion specifying that running test function with an input must return expected output: `assert ((test input) = output)`. 

For each buggy program in the bugs directory (bug_#), we collected the following two contents:

Name     | Description
-------- | -----------
bug.ml   | incorrect source code fails to pass the test cases
bug.json | bug info file in json format.

Each json file reports the test cases that the buggy program fails to satisfy.
