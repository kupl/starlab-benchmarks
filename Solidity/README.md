Solidity
========

This is for the benchmarksk written in solidity. (Ethereum Smart Contract)  
The name of each directory denotes the CVE ID number for each vulnerable sol file.

### Vulnerability

We mainly focus on the **integer over/underflow** vulnerabilities in smart contracts.

### Directory

Each directory consists of two subdirectories (bugs and src) as follows:

```
Solidity
  |--- CVE_ID_NUM
            |--- src 
            |--- bugs
                   |--- bug_1.json
                   |--- bug_2.json
                   |--- ...
```

Name | Description
---- | -----------
src | project source code for each.
bug | bug info file in json format.

### meta.csv

It is the meta data for each vulnerable solc code.  
Description of the columns in meta.csv is as follows:

Column | Description
------ | -----------
ID     | The CVE ID number of solc code
LINE   | The exact line number where the vulnerability exists
MAIN_CONTRACT | The main smart contract deployed on the block chain (The sol file itself can have multiple contracts.) 
ADDRESS | <pre>The address of each smart contract for the etherscan site.<br>You can downlaod the sol file from the etherscan site with the given address<br>(httsp://etherscan.io/address/[ADDRESS])<br><br>Fr example, you can download the code of 2018-10299 from<br>https://etherscan.io/address/0xc5d105e63711398af9bbff092d4b6769c82f793d
LOC    | Lines of Code for each sol file
COMPILER_VERSION | The used version of solc (solidity compiler) when deployed
