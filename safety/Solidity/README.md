Solidity
========

This is for the benchmarks written in solidity. (Ethereum Smart Contract)  
The name of each directory denotes the CVE ID number for each vulnerable sol file.  
We have collected 200 benchmarks with different CVE ID number.

### Error Type

We mainly focus on the **Integer Over/Underflow** errors in smart contracts.

### Directory

Each directory consists of two subdirectories (bugs and src) as follows:

```
Solidity
  |--- CVE_ID_NUM
            |--- src 
                   |--- CVE_ID_NUM.sol
            |--- bugs
                   |--- bug_1.json
                   |--- bug_2.json
                   |--- ...
```

Name | Description
---- | -----------
src | project source code for each CVE.
bug | bug info file in json format.

#### src

The **src** directory consists of compilabe solidity file.  
The names are the CVE ID number. 

#### bugs
The **bugs** directory consists of buf info file in jason format.  
The contents of each file are as follows:

Name     | Description
-------- | -----------
err_type | The type of error occured. (Integer Over/Underflow)
source   | Always NULL (for solidity, we don't consider this info.)
sink     | The exact path and line number where the error occurs.

### meta.csv

It is a meta data for vulnerable solc codes. Description of the columns in meta.csv is as follows:

Column | Description
------ | -----------
ID     | The CVE ID number of solc code
LINE   | The exact line number where the vulnerability exists
MAIN_CONTRACT | The main smart contract deployed on the block chain (The sol file itself can have multiple contracts.) 
ADDRESS | <pre>The address of each smart contract for the etherscan site.<br>You can downlaod the sol file from the etherscan site with the given address<br>(`https://etherscan.io/address/[ADDRESS]`)<br><br>For example, you can download the code of 2018-10299 from<br>https://etherscan.io/address/0xc5d105e63711398af9bbff092d4b6769c82f793d
LOC    | Lines of Code for each sol file
COMPILER_VERSION | The used version of solc (solidity compiler) when deployed

