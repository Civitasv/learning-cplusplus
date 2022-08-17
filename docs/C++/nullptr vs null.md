`nullptr` has type `std::nullptr_t`.  It's implicitly convertible to any pointer type.  Thus, it'll match `std::nullptr_t` or pointer types in overload resolution, but not other types such as `int`.   

`0` (aka. C's NULL bridged over into C++) could cause ambiguity in overloaded function resolution, among other things:

    f(int);
    f(foo *);

> https://stackoverflow.com/questions/20509734/null-vs-nullptr-why-was-it-replaced

