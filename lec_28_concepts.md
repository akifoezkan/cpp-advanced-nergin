algorithms yerine ranges
--

# concepts reminder
  - requirements?
  - SFINAE yerine concept
  - kodlar daha okunabilir
  - constraintler, interface in bir parcasi
  - template ler overload edilebilir
  - sinif sablonlarinda specialization
  - hata mesajlari daha okunabilir
  - ranges tamamen conceptlerden faydalaniyor

  _ex:_
  ```cpp
    template<typename T>
    concept = requires (T ) {

    }
  ```
 
 # concept
  - concept in kendisi de bir template
  => aslinda constraintlerin isimlendirilmis hali

  - cogu concept type_traits deki traitlerden olusturulmus

  _ex:_
  ```cpp
    static_assert(std::integral<int>);      // true
    
    static_assert(std::integral<double>);   // false
  ```

  _ex:_
  ```cpp
    template<typename C>
    concept nec = requires {
      typename C::value_type;     // C turunun ::value_type i olmasi gerekir
    }; 

    struct A{

    };

    struct B{
      using ....  // ??

    };

    template<typename T>
    requires nec<T>
    class Myclass {
      // ...
    };

    int main()
    {
      Myclass<int> x;   // error
      
      Myclass<A> x;     // error
      
      Myclass<B> x;     // ok
    }
  ```

  _ex:_
  ```cpp
    template<typename C>
    concept nec = requires {
      typename C::iterator::pointer;
      typename C::iterator::reference;
    }; 

    // std kutuphanesi
    // vector<int>::iterator::pointer
  ```

  _ex:_
  ```cpp
    template<typename C>
    concept = cnec = requires {
        typename C::key_type;
        typename C::value_type;
    };

    template<typenmae T>
    requires cnec<T>
    void func(T);

    #include <map>

    int main()
    {
        using namespace std;

        map<int, double> mx;

        func(mx);
        
        std::string s;
        func(s);  // error
    }
  ```

  _ex:_
  ```cpp
    template<typename T>
    class Tclass{};

    template<typename T>
    concept nec = requires {
      typename Tclass<T>;
    }

    // Tclass<T> ile yazilabilen bir tur olmak zorunda

    int main()
    {
        static_assert(nec<int>);
    }
  ```

  _ex:_
  ```cpp
    template<typename T>
    class Tclass{

    };

    template<typename T>
    concept nec = requires {
        typename Tclass<T>;
    }

    int main()
    {
        static_assert(nec<int>);  // ok
    }
  ```

  _ex:_
  ```cpp
    #include <ranges>

    template<typename T1, typename T2>
    concept nec = requires {
        typename T1::value_type;
        typename std::ranges::iterator_t<T1>;
        typename std::ranges::iterator_t<std::vector<T1>>;
        typename std::common_type_t<T1, T2>;
    }
  ```

## ?? amaci
  - constrainti en iyi ifade etmee

  _ex:_
  ```cpp
    template<typename T>  
    concept nec = requires(T a, T b) {
        a + b;
    };

    static_assert(nec<int>);  // ok, operator+ is defined for int
    
    static_assert(nec<int*>); // error
  ```

  _ex:_
  ```cpp
    template<typename T>  
    concept nec = requires(T a, T b) {
        a - b;
    };

    static_assert(nec<int>);  // ok
    
    static_assert(nec<int*>); // ok 
  ```

  _ex:_
  ```cpp
    template<typename T>  
    concept nec = requires(T p) {
        p == nullptr;
    };

    // p,nullptr ile karsilastirilabilir olmali 

    static_assert(nec<int>); // error
    
    static_assert(nec<int*>); // ok
    
    static_assert(nec<std::optional<int>>); // error

    static_assert(nec<std::unique_ptr<int>>); // ok
  ```

  _ex:_
  ```cpp
    template<typename T>  
    concept nec = requires(T p) {
        p == nullptr;
    };

    static_assert(nec<std::optional<int>>);
  ```

  _ex:_
  ```cpp
    template<typename T1, typename T2>  
    concept nec = requires (T1 x, T2 y) {
        x > y && *x < *y;
    };

    static_assert(nec<int, double>);  // error
    
    static_assert(nec<int*, int*>);   // ok
  ```

  _ex:_
  ```cpp
    template<typename T>
    concept nec = requires (T x) {
        std::cout << x;
        std::cin >> x;
    };

    struct A {};
    static_assert(nec<A>);    // error
  ```

## requires expresssion in ici bos brakilamiyor

  _ex:_
  ```cpp
    template<typenmae<T>>
    concept nec = requires(typename T::value_type x) {
      true;   // burasi bos olamaz
    }
  ```

  _ex:_
  ```cpp
    template<typenmae T>
    requires (sizeof(T) > 4) &&
              std::integral<T> &&
              requires {
                  typename T::value_type;
              }
    

    // requires anahtarindan sonraki butun kisim: requires clause
  ```

  _ex:_
  ```cpp
    template<typenmae T>
    requires requires (T x) {
      x + x;}

    // expression above is ok
  ```

  _ex:_
  ```cpp
    template<typename T>
    concept nec = requires {
        std::integral<T>;
    };

    // concept sunu check eder: integral<T> yazilabilir mi?
    // integral<T> her tur icin yazilabilir, dolayisyla
    // yukaridaki ifade butun turler icin true degeri dondurur

    static_assert(nec<double>);   // true
  ```

## nested requirement

  _ex:_
  ```cpp

    template<typenmae t>
    concept myc = requires {
        requires std::integral<T>;
    };

    // T nin integral concept ini satisfy ediyormu? (true mu)
  ```

## compound requirement
  - {expr} noexception ->type_constraint(optional)
  - concept<decltype(decltype((expr)) ??

  _ex:_
  ```cpp
    template<typename T>
    concept nec = requires {
        {&x}->std::input_iterator 
    };

    // &x ifadesi std::input_iterator u satisfy etmek zorunda
  ```

  _ex:_
  ```cpp
    template<typename T>
    concept nec = requires (T x){
        {*x}->std::convertible_to<typename T::value_type>; 
    };

    // convertible_to<decltype(*x), T>
    //
    // convertible_to<param1, param2>,  2 parametreye sahip bir concept
    // static_assert(int, double): int den double a donusum var mi?
  ```

  _ex:_
  ```cpp
    template<typename T>
    concept nec = requires (T c){
      {c + 5} -> std::integral;
    }

    // c + 5, std::integral ifadesini saglamak zorunda
  ```

  _ex:_
  ```cpp
    template<typename T>
    concept nec = requires (T x) {
        {x == x} -> std::convertible_to<bool>;
    };
  ```

  _ex:_
  ```cpp
      template<typname T>
      concept Nec = sizeof(T) > 4;

      template<typename T>
      concept nec = requires (T x) {
          {x + x} -> Nec;
      };
  ```

## conceptleri nerelerde kullanabiliriz?

### requires clause

  _ex:_
  ```cpp
    template<typename T>
    requires std::floating_point
    void func(T x);

    // floating_point concepti direl requires clause da kullanildi
  ```

### trailing requires clause
  _ex:_
  ```cpp
      template<typename T>
      void func(T x) requires std::integral<T>
  ```

## constraint template parameter
  _ex:_
  ```cpp
    template<std::integral T>
    class Myclass {};

    // template parametresi integral constraintini satisfy etmek zorunda

    Myclass<double> x;  // error
  ```

  _ex:_
  ```cpp
    template<std::convertible_to<int T>
    class Myclass {};

    Myclass<std::string> x;
  ```

  _ex:_
  ```cpp
    template<std::convertible_to<int T, std::integral U>
    class Myclass {};
  ```

  _ex:_
  ```cpp
    // nontype template parameter
    template<auto x>
    class Myclass {

    };

    Myclass<10>   // type of x is int

    Myclass<1.2>
  ```

  _ex:_
  ```cpp
      template<typename T, std::convertible<T> auto x>
      class Myclass {};

      // 2. tur (x) 1. ture (T ye) donusturulebilir olmak zorunda
  ```

  _ex:_
  ```cpp
      template<std::integral T, std::integral U>
  ```

---
### reminder: abbreviated template syntax
  _ex:_
  ```cpp
    void func(auto x) {}
    
    // same as

    template<typenmae T>
    void func(T x)
  ```

  _ex:_
  ```cpp
    void func(auto x, auto y)
    {}

    int main()
    {
        func(2, 3.4)
    }
  ```
---

  _ex:_
  ```cpp
      auto func(std::integral auto x, std::floating_point auto y)

      // is abbreviation of

      template<std::integral T, std::floating_point U>
      auto func(T x, U y)
  ```

  _ex:_
  ```cpp
      std::integral auto foo(std::integral auto x)
  ```

## if constexpr icine yazilabilir
  _ex:_
  ```cpp
      template<typename T>
      void func(T x)
      {
          if constexpr(std::integral<T>)
          {
              std::cout << "integral type\n";
          }
          else if constexpr (std::floating_point<T>)
          {
              std::cout << "floating type\n";
          }
          else 
          {
              std::cout << "other types\n";
          }
      }

      int main()
      {
          func(12);
          func(2.3);
          func(nullptr);
      }
  ```

## auto kullanilan her yerde concept kullanilabilir
  _ex:_
  ```cpp
      int main()
      {
          std::integral auto x = 10;    // ok
          std::integral auto x = 1.0;   // error 
      }
  ```

## range base for loop
  _ex:_
  ```cpp
      int maint()
      {
          std::vector<int> ivec(10);
          for(std::integral auto x : ivec) {
              // ok 
          }

          std::vector<float> fvec(10);
          for(std::integral auto x : fvec) {
              // error
          }
      }
  ```

##
  _ex:_
  ```cpp
      class Myclass {
      public:
          void func(auto x);

          template<typename T>
          void func(T x);


          void func(std::integral auto x)
      };
  ```

## variadic template ler ile birlikte
  _ex:_
  ```cpp
      template<istd::integral ...Args>
      bool all(Args ...args)
      {
          return (... && args);
      }

      std::cout << std::boolalpha;
      std::cout << All(12, 3L, 'A', true);  // ok
      
      std::cout << All(12, 3., 'A', true);  // error
  ```

## constraintler overloading ile kullanilabilir
  _ex:_
  ```cpp
    // reminder
    #include<iterator>

    vector<int> ivec(100);
    auto iter = ivec.begin();
    advance(iter, 5);
  ```

  _ex:_
  ```cpp
    template<std::random_access_iterator Iter>
    void Advance(Iter&, int n)
    {
        cout << "random access iterator"; 
    }

    template<std::bidirectional_iterator Iter>
    void Advance(Iter&, int n)
    {
        cout << "bidirectional iterator"; 
    }

    template<std::forward_iterator Iter>
    void Advance(Iter&, int n)
    {
        cout << "forward iterator"; 
    }

    // all template parameters are constraints
    // SFINAE: constraint saglanmazsa yok sayilir

    int main()
    {
        vector<int> c;
        auto iter = c.begin();
        Advance(iter, 5);
        

        list<int> c2;
        Advance(c2.begin(), 5);
    }
  ```
???
??? partial ordering rules of specialization
  _ex:_
  ```cpp
    // ???
    void func(std::integral auto)
    {
        std::cout << "1\n";
    }

    void func(??)
    {
        std::cout << "2\n";
    }

    void func(??)
    {
        std::cout << "3\n";
    }
  ```

  _ex:_
  ```cpp
  // primary spec.
  // partial spec.
  // ??
  ```

  _ex:_
  ```cpp
      template<typename T>
      concept Pointer = requires (T p) {
          *p;
          p[0];
          p == nullptr;
          {p < p} noexcept -> std::convertible_to<bool>;
      }

      auto min_value(auto a, auto b) 
      {
          std::cout << "1\n";
          return a < b ? a : b;
      }

      auto min_value(Pointer auto a, Pointer auto b)
      requires std::three_way_comparible_with<decltype(*a), decltype(*b)>
      {
         std::cout << "2\n";
         return min_value(*a, *b); 
      }

      int main()
      {
         int a = 30;
         int b = 45;
         double d = 2.3;

         std::cout << min_value(a, b) << "\n";

         // 2, 1, 30


         int** ptr1{&p1};
         int** ptr2{&p2};
         cout << min_value(ptr1, ptr2);
      }
  ```

# cppreference.com/w/cpp/concepts
   integral
   unsigned_integral
   convertible_to
   movable
   copyable
   regular
   semiregular

next lecture: span, GSL -> span, 
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
