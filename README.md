# SW STARLAB Benchmarks<br>by Software Analysis Laboratory

This repository features two sorts of bugs: functional bugs and safety bugs. Functional bugs are software errors that violates the programmer's intention or other expectations that the software is supposed to meet. These are typically manifested as simple expectation violations such as assertion violations, or unexpected software errors such as crashes, infinite loop, or resource exhaustion. On the other hand, safety bugs are software errors that cause problems such as memory leak, use-after-free, or double free. Unlike functional bugs, these do not induce assertion errors or crashes; rather, they are typically manifested as software vulnerabilities such as memory management faults in C projects or overflow/underflow during integer arithmetic in Solidity smart contracts.

## Directory Overview

- `Benchmarks_with_Functional_Bugs` contains functional bugs found from open-source Java projects and student assignment submissions written in OCaml.
    - The Java projects has failing testcases revealing logical errors.
    - The OCaml student submissions feature assertion violations required by the respective exercise problems.
- `Benchmarks_with_Safety_Bugs` contains safety bugs found from open-source C projects and Real-world Ethereum smart contracts written in Solidity.
    - The C projects show faults in memory management, namely, memory-leak, double-free, or use-after-free. 
    - The Solidity smart contracts show faults in integer arithmetic, such as integer overflows or underflows.

## Dataset Overview
<table>
  <tbody>
    <tr>
      <th>Bug Type</th>
      <th align="center">Target Language</th>
      <th align="center">Fault kind</th>
      <th align="center"># of Faults</th>
      <th align="center">Sum</th>
    </tr>
  <tr>
    <td align="center" rowspan="3">Safety</td>
    <td align="center">C</td>
    <td align="center">Resource Leak,<br>Memory Leak</th>
    <td align="center">200</td>
    <td rowspan="3" align="center">1000</td> 
  </tr>
  <tr>
    <td align="center">Java</td>
    <td align="center">Null Pointer<br>Exception</th>
    <td align="center">500</td>
  </tr>
  <tr>
    <td align="center">Solidity</td>
    <td align="center">Integer<br>Overflow</th>
    <td align="center">300</td>
  </tr>
  <tr>
    <td align="center" rowspan="3">Functional</td>
    <td align="center">Java</td>
    <td align="center" rowspan="3">Logical Error</th>
    <td align="center">100</td>
    <td rowspan="3">400</td> 
  </tr>
  <tr>
    <td align="center">OCaml</td>
    <td align="center">300</th>
  </tr>
  </tbody>
</table>
