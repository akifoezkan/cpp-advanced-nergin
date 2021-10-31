# Templates
  - derleyiciye kod yazdirir  
  - template => metacode

## cesitleri
  - function template
  - class template
  - alias template        (modern c++)  
    => using anahtar sozucugu ile template support
       (typedef template support etmiyordu)
  - variable template     (modern c++)  
     => template keyword lere bagli kod yazdirma


 >   => standardlara gore "function template" ve "template function"
       diye bir terim farkliligi yok.

 >   => instantiation: user-code un compiler tarafindan yazilmasi   


## examples from STL:
* function template:
    - algorithm headerindaki fonksiyonlar
    - numeric headerindaki algoritma fonksiyonlar

* class template:
    - containerlar
        * sequence (vector, list, forward_list, string, array)
        * assocoative (set, map, multiset, multimap)
        * unordered associative (unordered set/map/multiset/multimap)

    - stack, queue, priority queue  
        => bir container alip bunlarin interface ini adapte eder

    - smart pointers  
        => unique ptr, shared ptr,

    - RAII amacli guard siniflari

    - hash sinif sablonu

    - metafonksiyon olarak kullanilan siniflar
        * type_traits dosyasindaki class lar
            => compile time programlamasi icin veri uretirler

* alias template:
    - sonu "_t" ile biten fonksiyonlar

    _ex:_
    ```cpp
      remove_reference_t<int &>
    ```

    >=> template icinde kullanidiginda typename anahtar sozcugunun
       kullanilma zorunlulugunu ortadan kaldirir

    _ex:_  
    ```cpp
    template<typename T>
    using RemoveReference_t = typename std::remove_reference<T>::type
    ```


* variable template:
    - sonu "_v" ile biten fonksiyonlar

      _ex:_
      ```cpp
        is_rvalue_reference_v<int&&>
      ```

      _implementation:_
      ```cpp
        template<typename T>
        inline constexpr bool is_RvalueReferenc_v = std::is_rvalue_reference<T>::value;
      ```

===========================================================================
## reminder: inline variable
```cpp
int x = 10; // global variable, violates one definition rule if in the header
```

similar for functions

```cpp
// ali.h
inline void func(int x) { ... }
```

linker inline func dan sadece 1 tane gorur, ODR ihlal edilmez

=> token-by-token ayni kod inline edildigi takdirde, birden fazla kez include edilse bile ODR (one definition rule) i ihlal etmez

## c++17 inline for variables
  ayni sekilde optimizasyona yonelik degil

  _ex:_  
  ```cpp
  inline int x = 10;
  ```

  => token by token ayni olmasi durumunda birden fazla dosyada include edilse bile 1 tane degisken olusturulur. ODR ihlal edilmez

===========================================================================

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

==================== template argument deduction ====================
# Derleyicinin kod yazabilmesi icin parametreleri compile-time da bilmesi gerekir
  peki derleyici bunu nasil bilecek?

3 temel arac var:
 1. template argument deduction
     - fonksiyon sablonlari
     - class templates, CTAD: (C++17)

      ex:
      ```cpp
             int x = 10, y = 34;
             swap(x, y);     // context den anliyor
      ```

      ex:  
      ```cpp
             vector x = {1, 2, 3, 5};   // buradan vector type i anlar
             vector<int> x = {1, 2, 3, 5};

             vector y = {x, x};  // vector<vector<int>>
      ```

      ex:
      ```cpp
             optional x = 5;
      ```

       ex:
      ```cpp
             pair p = {12, 6};
             pair p2 = {2.3, 3.4f};
      ```

      ex:
      ```cpp
             template<typename T, size_t n>
             void func(T(&)[n])

             int a[10]{};
             func(a);
      ```

      ex:
      ```cpp
             template<typename T, typename U>
             void func(T(*p)(U))
             {
                 cout << typeid(T).name() << "\n";
                 cout << typeid(U).name() << "\n";
             }


             int foo(double) { return 1 };

             int main()
             {
                 func(foo);      // decay to function pointer
                                 // T -> int, U -> double
             }
      ```

      ex:
      ```cpp
             template<typename T, typename U>
             void func(T(Myclass::*)(U))
             {
                 cout << typeid(T).name() << "\n";
                 cout << typeid(U).name() << "\n";
             }


             class Myclass{
             public:
                 int foo(long) { return 1; }
             }

             int main()
             {
                 func(&Myclass::foo);    // decay to function pointer
                                         // T -> int, U -> long
             }
      ```


 2. explicit template argument
     - template parametresini user verir

      ex:
      ```cpp
         template<typename T, typename U>
         void func(T, U)
         {

         }

         int main()
         {
             func<int, double>(12, 2.3)
         }
      ```

     - bazi template lerde deduction sansi yok, bu durumda explicit cok onemli

      ex:
      ```cpp
             template<typename T, typename ...Types>
             std::unique_ptr<t> MakeUnique(Types && ...args)
             {
                 return std::unique_ptr<T>{new T{std::forward<Types>(args)...}};
             }

             make_unique<int>(12);   // deduction sansi yok
      ```

      ex:
      ```cpp
            template<typename T, typename U>
            void func(T x, U y)
            {...}

            func<int>('A', 4.5);
      ```

      ex:
      ```cpp
            template<int x, int y>
            void func()
            {...}

            func<10, 20>();
      ```

 3. default template parameter
     - c++11 sonrasi fonksiyonlarda kullanilma hakki geldi

      ex:
      ```cpp
         template<typename T, typename U = int>
         class Myclass{};

         int main()
         {
             Myclass<std::string>
             Myclass<int, double>
         }
      ```

     - container siniflari

      ex:
      ```cpp
        vector<int> -> vector<int, std::allocator<int>>;

        template<typename K, typename Comp = less<K>, typename A = allocator<K>>
        class ...
      ```


      ex:
      ```cpp  
     template<typename T>
     struct DefaultDelete {
         operator()(T *p)
         {
             delete p;
         }
     };
    
     template<typename T, typename D = std::default_delete<T>>
     class UniquePtr {
     public:
         ~UniquePtr()
         {
             D{}(mp);    // aslinda delete eden deleter
         }

     private:
         T* mp;
     };
      ```

     ex:
     ```cpp
     // ostream sinif

     basic_ostream<char> => basic_ostream<char, ...>

     string => basic_string<char, char_traits<char>, allocator<char>>
      ```

> => default template parameter alan argumanin sagindaki butun parametreler
> default arg almak zorunda

## Fonksiyon cagirisinda kullanilan argumanlar yoluyla template parametre cikariminin yapilmasi deductiondaki tek yol degil

  ex:
  ```cpp
    template<typename T, typename U>
    void func(T x, U y);

    void(*fp)(int, double) = &func;   // template parameters of func are deduced
  ```

=====================================================================
# template argument deduction icin derleyici 2 fazda kontrol yapar
  1. parametreler eslestirilmeden bir sytax check yapilir:
     > instantiation yok (kod yazma yok), template kod var  
     > parameter-independent isim arama, parantez vs.

  2. derleyici template kodunu fiilen yazar (instantitation var). 
     Normal cpp kodda oldugu gibi normal bir syntax chack yapilir


## auto ile template type deduction arasinda 1 istisna harici hic bir fark yok
  ```cpp
    template<typename T>
    void func(T x) {}

    int main()
    {
        // kurallar ayni, bir istisna harici
        auto x = expr;
        func(expr)
    }
  ```

## template typename deduction rules:
  1. call parametresinin referans declaratoru icermemesi
  2. call parametresinin referans declaratoru icermesi
  3. call parametre forwarwding referans

### 1. call parametresinin referans declaratoru icermemesi
  - const luk ve referanslik kaybedilir kaybedilir

      ex:
    ```cpp
      template<typename T>
      void func(T x);
      
      int main() {
        const int x = 10;
        const int& z = 10;

        func(x);   // T = int, const luk duser
        func(z);   // T = int, ref lik de duser
      }
    ```

  - array decay olur

    ex:
    ```cpp
      template<typename T>
      void func(T x);
      
      int main() {
        int a[10]{}
        func(a);        // T = int*, array decay (dizi turu degil)

        const int b[10]{};
        func(b)         // T = const int*, low-level constluk dusmez
                        // (high-level constluk duser)
     }
    ```


  - string literal -> const char*

    ex:
    ```cpp
      template<typename T>
      void func(T x);
      
      int main() {
        func("ali");    // T= const char*, array decay
     }
    ```

  - function name -> function pointer 

    ex:
    ```cpp
      template<typename T>
      void func(T x);
      
      int main() {
        int foo(int);
        func(foo);      // T = int(*)(int), function -> function pointer
     }
    ```


> => Ayni template tur parametresi kullanan birden fazla parametre varsa bunlar ayni olmak zorunda

  ex:  
  ```cpp
  template<typename T>
  void func(T x, T y)

  func(1, 4.5);     // ambiguity
  func('A', 120);   // ambiguity
  ```


### 2. call parametresinin referans declaratoru icermesi

> =>Note: deduction her zaman dogru bir sekilde yapilamayabilir
>  ex:  
>  ```cpp
>  template<typename T>
>  void func(T x, T y)
>
>  func(12);   // error, sol taraf PR value ya baglanamz
>  ```

  - const luk dusmez
    ex:  
    ```cpp
    template<typename T>
    void func(T &x)
    {
        T y;  // error, const int y must be initialized
    }

    const int x = 10;
    func(x);            // T = const int
    ```

  - array to pointer conversion olmaz (array decay yok):

    ex:  
    ```cpp
    template<typename T>
    void func(T &x)
    {
        T y{};  // T = int[3]

        std::cout << typeid(y).name() << "\n";
    }

    int a[3]{};
    func(a);      // T= int[3], decay yok

    // derleyicinin yazacagi kod
    void func(int(&x)[3]);
    ```


    ex:  
    ```cpp
    template<typename T>
    void func(T &x) {}

    const int a[] = {1, 2, 3};
    func(a);    // T = const int[3]
    ```


  - function -> function referans

    ex:  
    ```cpp
    template<typename T>
    void func(T &x) {}

    int bar(int);

    func(bar);  // function reference

    // derleyici sunu yazacakti

    func(int(&r)(int))
    ```

    soru:
    ```cpp
    template<typename T>
    void func(T &x) {}

    func("alican"); // T = const char[7]    
    ```


    error ex:
    ```cpp
    template<typename T>
    void func(T &x, T& y) {}

    func("alican", "ab");   // ambiguity, const char[7] vs const char[3]
    ```

    error ex:
    ```cpp
    template<typename T>
    void func(T &x, T& y) {}
    
    int a[5]{};
    int a[7]{};
    func(a, b); // ambiguity,  int[7] vs int[3]

    // fonksiyon turleri referans olmasaydi ambiguity olmazdi:
    template<typename T>
    void func(T x, T y) {}
    
    int a[5]{};
    int a[7]{};
    func(a, b); // array decay
    ```

### 3. call parametre forwarding referans
  ne olursan ol ben seni alirim

  ex:
  ```cpp
  template<typename T>
  void func(T&& x);     // forwarding ref, not a PR-val ref
  ```

  - derleyicinin yazdigi fonksiyonun parametre degiskeni mutlaka referans olacak  
    ya L-val ref yada R-val ref olacak

    > => Parametre icin reference collapsing rules
    
    ex:
    ```cpp
    template<typename T>
    void func(T&& x);     // forwarding ref, not a PR-val ref

    int main()
    {
      func(10);       // T = R-val ref, int
                      // type of x: int&& 
      
      int ival{10};
      func(ival);     // T = L-val ref, int&
                      // type of x: int& 
    }
    ```

  => tabiki forwarding referans perfect-forwarding olmak zorunda degil

  ex:
    ```cpp
    template<typename T>
    void func(T&& x, T y) {}

    int main()
    {
        int a = 10;

        func(a, a); // ambiguity: T= int&, T = int
    }
    ```

    ex:
    ```cpp
    template<typename T>
    void func(T&& x, const std::vector<T>& vec) {}

    int main()
    {
        vector<int> ivec(10);
        func(ivec[0], ivec);    // ambiguity, T = int& vs T = int
                                // we can solve this by type identity veya explicit argument
    }
    ```

# tur cikarimini bloke etme
  - type identity
  - yada 2. bir tur kullan

  ex:
  ```cpp
  # include <type_traits>

  template<typename T>
  void func(T x, std::add_lvalue_reference<T> y)
  {
     // 2.parametre birinci parametrenin turunden elde edilecek
  }
  ```


========================= template funciton overloading ===============================
sorular:
  - tempalte function overload edilebilir
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



