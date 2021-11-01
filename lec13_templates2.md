# template function overloading cont.
 gercek fonksiyonlarla template foksiyonlarin imza kurallari farkli

 * su fonksiyonlar farkli imzaya sahip  
   > return type imzanin bir parcasi  
    
    _ex:_  
    ```cpp
     template<typename T>
     int foo(T);

     template<typename T>
     double foo(T);
    ```
    _ex:_
    ```cpp
     // burada funtion template overlading var (imzalar farkli)

     template<typename T, typename U>
     int foo(T, U) {};
     
     template<typename U, typename T>
     int foo(U, T) {};

     // bu su acidan anlamli:
     // Bu functionlar farkli dosyalarda tanimlanabilir.
     // Farkli dosyalarda tanimlanan bu fonksiyonlar farkli imzaya sahip olduklarindan compilation da sorun olustrumazlar.
     // Link asamasinda ise farkli sembola sahip olacaklar (farkli imzalar).
    ```

    ```cpp
     // redeclaration ayni imzaya sahip 
     // Bu functionlar farkli dosyalarda instantiate edilse bile
     //   undefined behaviour a sebep olur. ODR i ihlal eder.

     template<typename T, typename U>
     int foo(T, U) {};
     
     template<typename U, typename T>
     int foo(U, T) {};
    ```

>   => Function overloading olup/olmamasi ve 
>      fonksiyon cagirisinda ambiguity, syntax error olup/olmamasi farkli seyler  
>   Yani yukaridaki gibi overloading olan fonksiyonlara yapilan fonksiyon cagrisi ambigious olabilir.  


# Partial Ordering Rules - Intro:
Birbirinin overloadu olan template functionlara yapilan cagri uzerine hangi fonksiyonun secileceginin belirlenmesi.
  - daha specific olan cagirilir
  > Oyle seneryolar var ki daha specific olani standard kurallari belirler, fikir yuruterek bulmasi zor.

  _ex:_
  ```cpp
    // 1
    template<typename T>
    void func(T*)
    {
        std::cout << "T *\n";
    }

    // 2
    template<typename T>
    void func(const T*)
    {
        std::cout << "const T*\n";
    }

    //overloading var
    int x = 10;
    func(&x);       // 1

    const int y = 10;
    func(&y);       // 2
  ```

  _ex:_
  ```cpp
    // 1
    template<typename T>
    void func(T)
    {
        std::cout << "T\n";
    }

    // 2
    template<typename T>
    void func(T*)
    {
        std::cout << "T*\n";
    }

    int x = 10;
    func(&x);       // 2
  ```

# Partial Ordering Rules:
> Function Overloading: Ortada birden fazla fonksiyon varsa, derleyici once "function overload set" i olusturur.  

  - Exact match normal fonksiyon, function overload set e oldugu gibi katilir 
    > Exact match normal fonksiyon (Template olmayan) varsa, template olmayan fonksiyon cagirilir
  - Ortada template fonksiyon varsa derleyici 
    1. Once "argument deduction" yapmak zorunda  
    2. Ardindan derleyici "substitution" yapar (function i yazar).  

    _ex:_
    ```cpp
    // 1
    // argument deduction: T = int
    // substitution: void func(int *)
    template<typename T>
    void func(T*)
    {
        std::cout << "T *\n";
    }

    // 2
    // argument deduction: T = int
    // substitution: void func(const int *)
    template<typename T>
    void func(const T*)
    {
        std::cout << "const T*\n";
    }

    int x = 10;
    func(&x);       // 1
    ```

## SFINAE:
> Substitution failure is not an Error  

  Substitution sirasinda hata olursa, derleyici syntax error vermez.  
  Sadece bu fonksiyonu overload setten cikarir

  _ex:_
  ```cpp
    template<typename T>
    typename T::nec func(T x);

    int main()
    {
        int x = 10;

        func(x);
    }

    // T = int, substitution sirasinda error olacak (int::nec yok)
    // Normalde bu syntax error olacakti, ancak hata vermek yerine 
    // derleyici bu fonksiyonu overload setten cikarir ve compilation a devam eder
  ```


  _ex:_
  ```cpp
   // 1
   template<typename T, unsigned N>
   std::size_t len(T(&)[N])
   {
       return N;
   }

   // 2
   template<typename T>
   typename T::size_type len(T const& t)   
   {
       // Note: template e bagli dependent type elde edildiginde, bunlarin type 
       // olarak kullanilmasi icin "typename" keyword unun kullanilmasi zorunlu
       return t.size();
   }

   int main()
   {
       int a[10]{};
       std::cout << len(a);    // T = int, N = 10
                               // derleyici 
                               // - 1. fonk. sunu yazar: 
                               //    size_t len(int(&)[10]) {...}
                               // - 2. fonksiyon icin sunu yazar 
                               //    typename int::size_typename ....
      // Substituion sirasinda 2. fonksiyon icin syntax error verilmesi yerine 
      // SFINAE out edildi (no compiler error)

      std::cout << len("tmp");  // 1) T = const char, N = 4
                                // 2) SFINAE out, no compiler error

      int* p{};
      std::cout << len(p); // syntax error:
                           // no matching overloaded function found

      std::allocator<int> x;
      std;:cout << len(x);     // syntax error
                               // 1) viable degil
                               // 2) T = allocator<int>, viable (argument deduction is ok)
      
      // Yani derleyici 2. fonksiyonu overload function olarak secti
      //    function overload resolution bitti, substitution oncesi hata yok
      //    instantiation error: 
      //        allocator<int>.size() is not a member 
   }
  ```

> c++20 ile conceptler SFINAE nin kullanim alani azaldi.

## Rule: variadic parametre herkese kaybeder
  _ex:_
  ```cpp
    // cpp_kodlar/ basic02.cpp

    template<typename T, unsigned N>
    std::size_t len(T(&)[N])
    {
    	return N;
    }
    
    template<typename T>
    typename T::size_type len(T const& t)
    {
    	return t.size();
    }
    
    // variadic parametre herkese kaybeder
    std::size_t len(...)
    {
    	return 0;
    }
    
    
    int main()
    {
      int a[10]{};    // selected: 1
                      // 1 viable, 2 not viable, 3 viable

      std::cout << len("tmp");  // 1
                                // 1 viable, 2 not viable (SFINAE out), 3 viable

      std::vector<int> v(5);
      std::cout << len(p);      // 2, (2, 3 viable)

    	int* p{};
      std::cout << len(p);      // 3, (3 viable, 2 SFINAE out)

    	std::allocator<int> x;
      std::cout << len(x);    // syntax error
                              // (2, 3 viable), overload resolution i 2 kazanir
                              // allocator<int>.size() yok, instantiation sirasinda hata verilir
    }
  ```

## Partial Ordering: How compiler selects the most specialized?
> source: https://docs.microsoft.com/en-us/cpp/cpp/partial-ordering-of-function-templates-cpp?view=msvc-160

Use the following process to determine if one function template candidate is more specialized:

1. Consider two function templates, T1 and T2.
2. Replace the parameters in T1 with a hypothetical unique type X.
3. With the parameter list in T1, see if T2 is a valid template for that parameter list. Ignore any implicit conversions.
4. Repeat the same process with T1 and T2 reversed.

5. If one template is a valid template argument list for the other template, but the converse isn't true, then that template is considered to be less specialized than the other template. If by using the previous step, both templates form valid arguments for each other, then they're considered to be equally specialized, and an ambiguous call results when you attempt to use them.

6. Using these rules:  
    * A template specialization for a specific type is more specialized than one taking a generic type argument.  

    * A template taking only ``T*`` is more specialized than one taking only ``T``, because a hypothetical type ``X*`` is a valid argument for a T template argument, but X is not a valid argument for a ``T*`` template argument.

    * ``const T`` is more specialized than ``T``, because ``const X`` is a valid argument for a ``T`` template argument, but ``X`` is not a valid argument for a ``const T`` template argument.

    * ``const T*`` is more specialized than ``T*``, because ``const X*`` is a valid argument for a ``T*`` template argument, but ``X*`` is not a valid argument for a ``const T*`` template argument.

    _ex:_
    ```cpp
        // bunlardan hangisi daha specific?

        template<typename T>
        void func(T x);       // 1, fictional type: A

        template<typename T>
        void func(T*);        // 2, fictional type: B

        // A turu fonksiyon-2 e gonderilemez
        // Ancak B* fonksiyon-1 e gonderilebilir (T = B*)
        // O zaman fonksiyon-2 daha specific
    ```
> Yukarida bahsedilen partial ordering rules ile daha specific fonksiyonu ararken sunlari ignore et:
>  1. default arguman varsa ama kullanilmamissa o parametreyi ignore et
>  2. variadic parametre argumani ignore et 

  _ex:_
  ```cpp
  templates/overloading/partial overlading.cpp

  template<typename T>
  void t(T*, T const* = nullptr, ...);  // 1
     // variadic parametreyi gormezden gel (daha specifik olana bakarken)
     // varsayilan arguman kullanilmis, ignore etme
     // yani su fonksiyonu incele:
     //    template<typename T>
     //    void t(T*, T const*);
     //        fictional type: A => (A*, A const*) ile fonk-2 cagirilamaz
  
  template<typename T>
  void t(T const*, T*, T* = nullptr);  // 2
     // kullanilmayan default argumani ignore et
     // yani su fonksiyonu incele:
     //    template<typename T>
     //    void t(T const*, T*);
     //        fictional type: B => (B const*, B*) ile fonk-1 cagirilamaz

  void example(int* p)
  {
     t(p, p);   // syntax error:
                 // daha sepecific fonk bulunamadi 
     
     // error message:
     // more than one instance of overload function "t" matches the argument list
  }
  ```

  _ex:_
  ```cpp
  // function overloading reminder

  void func(long double);   // 1
  void func(char);          // 2

  int main() {
      func(2.3f);   // synax error: ambiguity
                    // 1: standard conversion
                    // 2: standard conversion

  }
  ```

  _ex:_
  ```cpp
  // function overloading reminder
  
  void func(unsigned int);
  void func(long double);

  int main() {
      func(12);   // synax error: ambiguity
                  // both are standard conversion 
  }
  ```

  _ex:_
  ```cpp
  // function overloading reminder
  
  void func(long double); // 1
  void func(double);      // 2

  int main() {
      func(12.f); // 2 cagirilir (promotion)  
                  // 1: standard conversion 
  }
  ```
        

## reminder: template function in bulundugu yerde exact match (gercek fonksiyon) varsa gercek fonksiyon cagirilir

  _ex:_
  ```cpp
  int Max(int a, int b)   // 1
  {
      return b < a ? a: b;
  }

  template<typename T>
  T Max(T a, T b)         // 2
  {
      return b < a ? a: b;
  }

  
  auto n = Max(12, 45);   // 1 exact match (1, 2 viable)
  
  auto n = Max(12, 4.5);   // 1 (1 viable)
  
  auto n = Max('A', 'B');   // 2
  
  auto n = Max<>(12, 45);   // 2
  
  auto n = Max<int>(12, 4.5);   // 2
  
  auto n = Max<int>(12, 4.5);   // 2
  ```


  _ex:_
  ```cpp
  template<typename T1, typename T2>
  auto max(T1 a, T2 b)
  {
      std::cout << "1\n";
      return b < a ? a : b;
  }

  template<typename RT, typename T1, typename T2>
  RT max(T1 a, T2 b)
  {
      std::cout << "2\n";
      return b < a ? a : b;
  }

  int main() {}
      auto a = ::max(4, 7.2);     // 1, (2 viable degil, geri donus cikarimi yapilamaz)

      auto b = ::max<long double>(7.2, 4);    // 1 secilir
                                              // 1 (viable): auto max(long double, int)
                                              // 2 (viable): long double max(double, int) 

      auto c = ::max<int>(4, 7.2);    // ambiguity, 1 ve 2 arasindan secim yapilamiyor
                                      // 1 (viable): auto max(int, double)
                                      // 2 (viable): int max(int, double) 
  }
  ```
### exact match in partial ordering kurallarina gore template functionlara olan ustunlugunden su sekilde faydalanilabilir:

  _ex:_
  ```cpp
  // bir fonksiyonun sadece berlirli bir turle cagirilmasi
  // diger turlerle yapilan cagrilarda sentax hatasi verilmes 

  template<typename T>       
  void foo(T) = delete;

  void foo(int);

  int main()
  {
      foo(12);
      foo(.12);   // error, deleted
      foo(1.2f);  // error, deleted
  }

  // bu enable_if ile de yapilabilir
  ```


======================================================================
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

================ explicit / partial specialization ============================
# explicit specialization (full specialization)
  - Daha cok class template larda kullanilir  
  - function templates da beklenmedik seneryolar olusturabiliyor.  
    Bu yuzden tercih edilmesi gereken teknik function overloading.  

  _ex:_
  ```cpp
  template<typename T>
  void func(T x)
  {
      std::cout << "primary template\n";    
  }
  
  template<>
  void func(int x)
  {
      std::cout << "explicit/full specialization\n";
  }
  
  // yukaridaki ile asagidaki ayni:
  // template<>
  // void func<int>(int x)
  // {
  //     std::cout << "explicit/full specialization (another way)\n";
  // }

  int main()
  {
      func(55);   // primary
      func(4.5);  // full spec.
      func(4.5f); // full spec.
  }
  ```

  _ex:_
  ```cpp
    template <typename T, typename U>
    class Myclass {
        // ...
    };

    template<>
    class Myclass<int, double> {
        // ...
    };
  ```

## explicit specialization definition lar resolutiona dahil edilmez   

 _ex:_
  ```cpp
  template<typename T>
  void func(T x)
  {
      std::cout << "1\n";
  }

  template<>
  void func(int *p)   // bu fonksiyon overload resolution a katilmaz
  {
      std::cout << "2\n";

      // fonk 2, fonk 1 in full specialization i (3 u gormez) 
  }

  template<typename T>
  void func(T* x)
  {
      std::cout << "3\n";
  }


  int main()
  {
      int x { 98 };
      func(&x);       // 3
  }

  // fonk 2, fonk 3 u gormedigi icin fonk 1 in explicit specialization i
  ```

  _ex:_
  ```cpp
  template<typename T>
  void func(T x)
  {
      std::cout << "1\n";
  }

  template<typename T>
  void func(T* x)
  {
      std::cout << "3\n";
  }

  template<>
  void func(int *p)   
  {
      std::cout << "2\n";
      
      // fon 2, fonk 3 un full specialization i 
      // cunku fonk 3 (partial ordering kurallarina gore) daha specific
  }

  int main()
  {
      int x { 98 };
      func(&x);       // 2
  }
  ```
---
## template ler (dogal olarak specialization lari da) local space de tanimlanamaz  
  template ler sadece suralarda tanimlanabilir:
  - namespace icinde
  - class definition icinde

  _ex:_
  ```cpp
    void foo()
    {
        class Myclass2{};       // ok 

        template<typename T>
        class Myclass{};        // gecersiz

        using Nec = int;    // ok

        // burada alias template  de kullanilamaz
    }
  ```

  _ex:_
  ```cpp
    namespace Nec {
        template<typename T>
        class Myclass {};
    }

    template<>
    class Nec::Myclass<int> {
        // this is ok, defined for Nec namespace 
    };
  ```


  _ex:_
  ```cpp
  template<typename T>
  class Myclass {};

  namespace Nec {
    // Burada Myclass a dair bir specialization tanimlanamaz
  }
  ```
---

  _ex:_
  ```cpp
  template<typename T>
  void func(T x)
  {
      std::cout << "1\n";
  }

  template<>
  void func(int *p)   
  {
      // 1 in specialization i
      std::cout << "2\n";
  }

  template<typename T>
  void func(T* x)
  {
      std::cout << "3\n";
  }

  int main()
  {
      int* p = nullptr; 
      func(p);          // 3, note that 2 overload resolutiona girmez
  }
  ```

  _ex:_
  ```cpp
  template<typename T>
  void func(T)
  {
      std::cout << "1\n";
  }

  template<typename T>
  void func(T*)
  {
      std::cout << "2\n";
  }

  template<>
  void func(int*)   
  {
      // 2 nin specialization i
      std::cout << "3\n";
  }

  int main()
  {
      int* p = nullptr; 
      func(p);          // 3 
  }
  ```

  _ex:_
  ```cpp
  template<typename T>
  void func(T)
  {
      std::cout << "1\n";
  }

  template<>
  void func(int*)   
  {
      std::cout << "2\n";
  }

  template<typename T>
  void func(T*)
  {
      std::cout << "3\n";
  }

  template<>
  void func(int*)   
  {
      std::cout << "4\n";
  }

  int main()
  {
      int* p = nullptr; 
      func(p);          // 4, (derleyici 1 ve 3 u gorur)
                        // 4 3 un, 2 1 in specializationi
  }
  ```

==================================================================
# Explicit Specialization of variable templates

  _ex:_
  ```cpp
    template<typename T>
    constexpr T SZ = sizeof(T);

    int main()
    {
       constexpr auto x = SZ<int>;
       cout << x;

       constexpr auto x = SZ<void>; // error, void is incomplete, size ini alamayiz
    }
```
> void turunun sizeof(void) u alinamaz, 
> ama void turu icin explicit spec. yapabilir miyiz?

  _ex:_
  ```cpp
    template<typename T>
    constexpr size_t SZ = sizeof(T);

    // explicit spec.
    template<typename T>
    constexpr size_t SZ<void> = 0;

    int main()
    {
        constexpr auto val = SZ<void>;
    }
  ```

  _ex:_
  ```cpp
    template<typename T>
    T x = T{67392.787878};

    template<>
    constexpr int x<int> = 999;

    int main()
    {
        cout << x<double>;
        cout << x<float>;
        cout << x<int>;
    }
  ```
    
  _ex:_
  ```cpp
  // pow, factorial
  template<size_t n>
  constexpr size_t factorial()
  {
      return n * factorial<n - 1>();
  }

  template<>
  constexpr size_t factorial<1>()
  {
    return 1;
  }

  int main()
  {
      constexpr fact = factorial<5>();
  }
  ```


_ex:_  
> 1 den 100 e kadar olan sayilari ekrana yazdiran bir kod yazin.
> Ama kodda dongu olmasin.

  _solution1_  
  ```cpp
    struct Myclass {
        Myclass()
        {
            static int x = 0;
            std::cout << ++x << " ";
        }
    };

    int main()
    {
        Myclass a[100];
    }
   ```

  _solution2_    
  ```cpp
  template<int n>
  struct Nec : Nec<n - 1> {
      Nec()
      {
          std::cout << n << " ";
      }
  };

  template<>
  struct Nec<0> {};

  int main()
  {
      Nec<100> nec;
  }
  ```

=================================================================
# partial specialization:
  primary templatein daraltilmis alternatifi icin kod saglar
  - fonksiyonlar icin tanimli degil
  - class ve variable icin gecerli

> siniflarin partial/full specialiation tanimlari birbirinden farkli interface lere sahip olabilir

  _ex:_
  ```cpp
    class Myclass {
    public:
        Myclass()
        {
            std::cout << "primary template\n";
        }
    };

    template<>
    class Myclass<int> {
    public:
        Myclass()
        {
            std::cout << "explicit specialization\n";
            std::cout << "Myclass<int>\n";
        }
    };
  ```


  _ex:_
  ```cpp
    template<typename T>
    class Myclass {
    public:
        Myclass()
        {
            std::cout << "primary template\n";
        }
    };

    template<typename T>
    class Myclass<T*> {
    public:
        Myclass()
        {
            std::cout << "partial specialization\n";
            std::cout << "Myclass<T*>\n";
        }
    };

    template<typename T>
    class Myclass<T[10]> {
    public:
        Myclass()
        {
            std::cout << "partial specialization 2\n";
            std::cout << "Myclass<T[10]>\n";
        }
    };

    template<typename T>
    class Myclass<T[]> {
    public:
        Myclass()
        {
            std::cout << "partial specialization 3\n";
            std::cout << "Myclass<T[]>\n";
        }
    };

    template<typename T>
    class Myclass<T&> {
    public:
        Myclass()
        {
            std::cout << "partial specialization 4\n";
            std::cout << "Myclass<T[]>\n";
        }
    };

    Myclass<long> a1;   // primary
    Myclass<float> a2;  // primary
    Myclass<int*> a3;   // partial
    Myclass<int&> a5;    // partial4
    
    Myclass<int[10]> a4;    // partial2
    Myclass<double[10]> a4; // partial2
    Myclass<double[20]> a4; // primary
    Myclass<int[]> a5;      // partial3
    
  ```

> primary template den daha az parametreye sahip partial specialization olusturulabilir

  _ex:_
  ```cpp
    template<typename T, typename U>
    class Myclass {
    public:
        Myclass()
        {
            std::cout << "primary template\n";
        }
    };

    template<typename T>
    class Myclass<T, T> {
    public:
        Myclass()
        {
            std::cout << "partial specialization\n";
            std::cout << "Myclass<T, T>\n";
        }
    };

    Myclass<long, int> a1;  // primary
    Myclass<long, long> a2; // partial
    Myclass<long, char> a3; // partial
  ```

  _ex:_
  ```cpp
    template<typename T, typename U>
    class Myclass {
    public:
        Myclass()
        {
            std::cout << "primary template\n";
        }
    };

    template<typename T>
    class Myclass<T, int> {
    public:
        Myclass()
        {
            std::cout << "partial specialization\n";
            std::cout << "Myclass<T, int>\n";
        }
    };


    Myclass<double, int> x;     // partial
    Myclass<double, char> y;   // primary
  ```
> primary template den daha fazla parametreye sahip partial specialization olusturulabilir
  _ex:_
  ```cpp
    template<typename T>
    class Myclass {
    public:
        Myclass()
        {
            std::cout << "primary\n";
        }
    };

    template<typename T, typename U>
    class Myclass<std::pair<T, U> {
    public:
        Myclass()
        {
            std::cout << "partial specialization\n";
        }
    };

    Myclass<int> x;
    Myclass<std::pair<int, float>> y;
  ```


=========================================================================
# enable_if ve SFINAE
> => SFINAE sadece function overloading icin gecerli  
> enable_if i kullanarak SFNIAE idiomundan faydalanabiliriz 

std::enable_if
  - ici bos bir yapi
    ```cpp
    template< bool B, class T = void >
    struct enable_if;

    template<class T>
    struct enable_if<true, T> { typedef T type; };
    ```

  - ``enable_if<true>`` nun bir type i varken  
    ``enable_if<false>`` in bir type i yok

    ```cpp
    enable_if<true>::type;
    enable_if<false>;       // type i yok, kullanildiginda SFINAE out olur
    ```

  _ex:_
  ```cpp
    template<typename T>
    std::enable_if<true, T>::type void foo(T x);
        // T void foo(T x);
    
    template<typename T>
    std::enable_if<false, T>::type void foo(T x);   // bu durumda gecersiz tur olusturur
        // substitution failure, overload set den duser
  ```

  _ex:_
  ```cpp
    template<typename T>
    std::enable_if<std::is_floating_point_v<T>, T>::type foo(T x);

    // ya da
    template<typename T>
    std::enable_if_t<std::is_floating_point_v<T>, T> foo(T x)
    {
        return x;
    }

    int main()
    {
        foo(12.4);  // enable_if<true, T>::type => float

        foo(12);    // enable_if<false, T>::type => SFINAE out
                    // no matching overloading func
    }
  ```

  _ex:_
  ```cpp
    // SFNIAE out 
    template<typename T>
    std::enable_if_t<std::is_floating_point_v<T>, T> foo(T x)
    {
        return x;
    }

    // only function in the overload set for foo(12) 
    foo(double);

    int main()
    {
        foo(12);  // not a syntax error
    }
  ```

  _ex:_
  ```cpp
    template<typename T>
    std::enable_if_t<std::is_floating_point_v<T>, T> foo(T x)
    {
        // 1
        return x;
    }

    template<typename T>
    std::enable_if_t<std::is_integral_v<T>, T> foo(T x)
    {
        // 2
        return x;
    }

    int main()
    {
        foo(12);    // 2 
        foo(4.5);   // 1 
    }
  ```
