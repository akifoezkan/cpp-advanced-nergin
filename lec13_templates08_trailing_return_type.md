# trailing return type

oyle bir template function yazalim ki 
  - farkli turler uzerinden toplama yapsin
  - return type da veri kaybi olmasin

  way-1: return type as a template parameter
  ```cpp
    template<typename R, typename T, typename U>
    R sum(T x, U y);
    {
        return x + y;
    }

    sum<double>(1.2, 16);

    // return type explicit olarak belirtilmek zorunda
  ```

  way-2: trailing return type
  ```cpp
    // trailing return type

    template<typename R, typename T, typename U>
    auto sum(T x, U y) -> decltype(x + y)
    {
        return x + y;
    }

    // Note: asagidaki kod syntax error,
    // return declaration x ve y nin scope u disinda
    template<typename R, typename T, typename U>
    decltype(x + y) sum(T x, U y);
    {
        return x + y;
    }
  ```

> trailing return type cogunlukla template ler icin matikli.
> Bunun disinda bir de su seneryo var:
    
  ```cpp
  int(*foo(int))(int*, int*)  // using function pointer as a return type
  foo();
  
  // using bildirimi ile kodu kolaylastirabiliriz:

  using FPTR = int(*)(int*, int*):)
  FPTR foo();
  
  // yada trailing return type kullaniriz:
    
  auto foo() -> int(*)(int*, int*);
  { ... }
  ```
    
  way-3 : ``auto`` return type
  - retrun type decay olur
  - birden fazla return type varsa geri donus turu ayni olmak zorunda

  ```cpp
    template<typename T, typename U>
    auto foo(T x, U y)
    {
        return /* Karmasik bir return type */;
    }
  ```

  _ex:_
  ```cpp
  // syntax error, multiple different return types

  auto func(int x)
  {
      if (x < 10)
          return x;

      return 1.5 * x; 
  }
  ```

  way-4 : ``decltype(auto)`` return type
  ```cpp
    decltype(auto) foo(...)
    {
        ...
    }
  ```
