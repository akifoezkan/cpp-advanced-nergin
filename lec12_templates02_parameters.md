# template parameters:

  ```cpp
    template<typename T>
    void func(T x, T y)
    {
        // x,y are call parameters
        // T template parameter
    }
  ```

# categories of template parameters:
  - type parameter
  - non-type parameter
  - template template parameter

## type parameter
  class, typename keywords

  _ex:_
  ```cpp
  template<class T>   // class keyword u kotu bir tercihdi, typename kullan
  ```

  _ex:_
  ```cpp
  template<typename T> void func(T x) { ... }
  ```

## non-type parameter
  ``template<int n>``

  _ex:_ ``std::array``, ``bitset``

  _ex:_
  ```cpp
      template<typename T, size_t n>
      struct Array{
          T a[n];
      };
  ```

  _ex:_
  ```cpp
      template<size_t n>
      class Bitset{...}

      std::bitset<16>
  ```

### Non-type parametre nin hangi turlerden olabilecegi her versiyonda genisletildi
  - tam sayi turler

  - pointer tur
    ```cpp
    template<int* p>
    class Myclass {};
    ```

  - function pointer
    ```cpp
    template<int(*p)(int)>
    ```

  - member function pointer

    ```cpp
      class Myclass {
      public:
          int foo(int);
      };

      template<int (Myclass::*p)(int)>
      class Neco {

      };
    ```

  - gercek sayi turu (c++20)
    ```cpp
    template<double x>
    class Myclass {};

    Myclass<2.3> x;
    ```

  - constexpr ctor a sahip sinif turleri (c++20)

  - non-type auto parameter (c++14)
    ```cpp
      template<auto n>
      class Myclass {};

      Myclass<14>
      Myclass<20U>
      Myclass<3.4>    (c++20)
    ```

example:

  ```cpp
  template<typename T, T n>
  class Myclass {};

 // * T type parameter
 // * n non-type parameter
  ```

---
#### Reminder: integral constant type
> yukaridaki ornegin cok kullanildigi bir sinif:  
> - cok sik, basit ama metafunction sinifinda asil destegi veren sinif

kullanim ornek:
  ```cpp
  std::integral_constant<bool, true>    // true type
  std::integral_constant<bool, false>   // false type

  template<bool B>
  using bool_constant = integral_constant<bool, B>;  // allias template
  ```

integral constant implementation:

  ```cpp
    template<class T, T v>
    struct integral_constant;

    template<class T, T v>
    struct integral_constant {
        static constexpr T value = v;
        using value_type = T;
        using type = integral_constant; // using injected-class-name
        constexpr operator value_type() const noexcept { return value };
        constexpr value_type operator()() const noexcept { return value; } // C++14
    };
  ```
---

## template template parameter
>  => eskiden class anahtar sozcugu zorunluydu, simdi typename kullanilabilir

  ex:
  ```cpp
      template<template <typename, typename > class X>
      void func
  ```
  ex:
  ```cpp
      template<typename T>
      class Myclass {};

      template<typename T, template<typename> typename X>
      void func(T x, X<T>)
      {
          // ...
      }
  ```

  ex:
  ```cpp
      template<typename T>
      class Myclass {};

      template<template<typename> typename X>
      class Nec{
      public:
          Nec()
          {
              std::cout << typeid(X<int>).name() << "\n";
          }

      };

      int main()
      {
          Nec<Myclass> x;
      }
  ```

  ex:
  ```cpp
      template<typename T, typename A, template<typename, typename> class Con>
      void func(Con<T, A> &x)
      {
          // input vector olabilir: std::vector<type, Allocator>
      }
  ```

  ex:

  ```cpp
      template<typenmae T>
      class Myclass{};

      template<typename T, template<typename> class Con>
      void func(Con<T> x)
      {
          // input vector olabilir
      }


      int main()
      {
          func<int, Myclass>
      }
  ```
