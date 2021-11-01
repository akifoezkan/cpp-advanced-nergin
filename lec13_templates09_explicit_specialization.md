# Explicit specialization (full specialization)
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
