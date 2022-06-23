---
author: 胡森
date: 2022/06/02
email: hscivitasv@gmail.com
---

1. **`make_unique<T>(args)` vs `unique_ptr<T>(new T(args))`**
	brief answer: make_unique is exception safe, the latter not.

	Also see [stack overflow](https://stackoverflow.com/questions/19472550/exception-safety-and-make-unique/19472607#19472607)

2. [unique_ptr::get - C++ Reference (cplusplus.com)](https://m.cplusplus.com/reference/memory/unique_ptr/get/)
