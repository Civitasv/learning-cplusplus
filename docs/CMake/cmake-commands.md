---
author: 胡森
date: 2022/05/25
email: hscivitasv@gmail.com
---

**Command:**

```cmake
file(GLOB_RECURSE <variable> [FOLLOW_SYMLINKS]
     [LIST_DIRECTORIES true|false] [RELATIVE <path>] [CONFIGURE_DEPENDS]
     [<globbing-expressions>...])
```

> Generate a list of files that match the `<globbing-expressions>` and store it into the `<variable>`.

**Example:**

```cmake
# find all header files under ${CMAKE_CURRENT_LIST_DIR}, and store them in _headers
file(GLOB_RECURSE _headers CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/*.h)
target_sources(gisoft PRIVATE ${_headers})
unset(_headers)
```
