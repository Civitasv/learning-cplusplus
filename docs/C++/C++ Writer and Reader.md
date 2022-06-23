---
author: 胡森
date: 2022/05/25
email: hscivitasv@gmail.com
---

`ofstream`: Stream class to write on files.
`ifstream`: Stream class to read from files.
`fstream`: Stream class to both read and write from/to files.

These classes are derived directly or indirectly from the classes `istream` and `ostream`.

## An example

```c++
#include <iostream>
#include <fstream>
using namespace std;

int main(){
	ofstream myfile;
	myfile.open ("example.txt");
}
```