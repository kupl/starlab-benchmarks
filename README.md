# SW STARLAB Benchmarks by Software Analysis Laboratory

This repository features two sorts of bugs: functional bugs and safety bugs.
Functional bugs are software errors that violates the programmer's intention or other
expectations that the software is supposed to meet. These are typically manifested as
simple expectation violations such as assertion violations, or unexpected software errors
such as crashes, infinite loop, or resource exhaustion.
On the other hand, safety bugs are software errors that may result in software crashes
or other software problems such as memory leak, use-after-free, or double free. Unlike
functional bugs, these do not induce assertion errors or crashes; rather, they are
typically manifested as software vulnerabilities such as memory management faults in C
projects or overflow/underflow during integer arithmetic in Solidity smart contracts.

## Directory Overview

- `Benchmarks_with_Functional_Bugs` contains functional bugs found from open-source C projects and student assignment submissions written in OCaml.
  - The C projects show undesirable behaviors such as infinite loop, resource exhaustion, or crashes.
  - The OCaml student submissions feature assertion violations required by the respective exercise problems.
- `Benchmarks_with_Safety_Bugs` contains safety bugs found from open-source C projects and Real-world Ethereum smart contracts written in Solidity.
  - The C projects show faults in memory management, namely, memory-leak, double-free, or use-after-free. 
  - Teh Solidity smart contracts show faults in integer arithmetic, such as integer overflows or underflows.

## 스타랩 dataset
<table>
  <tbody>
    <tr>
      <th>Language</th>
      <th align="center">Repo</th>
      <th align="center">K-loc</th>
      <th align="right">Number of Bugs</th>
      <th align="center">Vulnerability</th>
    </tr>
  <tr>
    <td align="center" rowspan="3">C</td>
    <td>Repo1</td>
    <th align="center">logs</th>
    <td>200</td>
    <td rowspan="3">
    <ul>
          <li>memoryleak1</li>
          <li>memoryleak2</li>
    </ul>
    </td> 
  </tr>
  <tr>
    <td>Repo2</td>
    <th align="center">loc</th>
    <td>200</td>
    
  </tr>
  <tr>
    <td>Repo3</td>
    <th align="center">loc</th>
    <td>200</td>  
  </tr>
  <tr>
    <td align="center" rowspan="3">JAVA</td>
    <td>Repo1</td>
    <th align="center">loc</th>
    <td>200</td>
    <td rowspan="3">
    <ul>
          <li>NPE</li>
          <li>NPE</li>
    </ul>
    </td> 
  </tr>
  <tr>
    <td>Repo2</td>
    <th align="center">loc</th>
    <td>200</td>
    
  </tr>
  <tr>
    <td>Repo3</td>
    <th align="center">loc</th>
    <td>200</td>  
  </tr>
  <tr>
    <td align="center" rowspan="3">Ocaml</td>
    <td>Repo1</td>
    <th align="center">loc</th>
    <td>200</td>
    <td rowspan="3">
    <ul>
          <li>OcamlError</li>
          <li>OcamlError</li>
    </ul>
    </td> 
  </tr>
  <tr>
    <td>Repo2</td>
    <th align="center">loc</th>
    <td>200</td>
    
  </tr>
  <tr>
    <td>Repo3</td>
    <th align="center">loc</th>
    <td>200</td>  
  </tr>
  <tr>
    <td align="center" rowspan="3">Solidity</td>
    <td>Repo1</td>
    <th align="center">loc</th>
    <td>200</td>
    <td rowspan="3">
    <ul>
          <li>SolidityError</li>
          <li>SolidityError</li>
    </ul>
    </td> 
  </tr>
  <tr>
    <td>Repo2</td>
    <th align="center">loc</th>
    <td>200</td>
    
  </tr>
  <tr>
    <td>Repo3</td>
    <th align="center">loc</th>
    <td>200</td>  
  </tr>
 
  
  </tbody>
</table>


  

## License
### ToDo
