========================= template function overloading ===============================
sorular:
  - template function overload edilebilir
  - function template ile ile normal function bir arada bulunabilir
  - overload resolution rules onemli

# template function overloading:
  - explicit specialization (full specialization)
  - partial specialization (class templates only )

### overloading vs redeclaration
  ```cpp
    void func(int);
    void func(int); // redeclaration, syntax error
    
    int func(int);  // syntax error, same signature
  ```

### intro to template function overloading
  _ex_
  ```cpp
    // this is syntax error, same signmature
    template<typename T>
    void func(T x) {}

    template<typename T>
    void func(T x) {}
  ```

#### unlike function overloading, return type is part of a template function signature
  _ex_
  ```cpp
    // this is NOT a syntax error
    template<typename T>
    void func(T x) {}

    template<typename T>
    int func(T x) {}
  ```
  
  > => Note: (template) fonksiyonlarin bir arada bulunmasi syntax error olmamasi
  > bu fonksiyonlara valid cagri yapilabilmesi anlamina gelmiyor.
  > Burada, template function overloading kurallariyla fonksiyonlarin bir arada bulunabilme
  > ozelliklerini inceliyoruz



  _ex_
  ```cpp
    // this is NOT a syntax error, this is overloading
    template<typename T, typename U>
    void func(T x, U y) {}

    template<typename U, typename T>
    void func(T x, U y) {}

    // ambiguity might happen only for the function call
  ```



