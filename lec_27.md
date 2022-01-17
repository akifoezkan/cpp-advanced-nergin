# reminder: aggregate types
  - eskiden aggregate type olan bazi turler artik agregate degil  
    bu yuzden aggregate initialization ypamiyoruz


  _ex:_
  ```cpp
    ??
  ```

## neden eklendi
  - daha onceden aggreagetlar () ile ??? init edilemiyordu
    C++20 de neden bu ozellik getirildi:
    1 - onislem programlari ile uyumsuzlugu
    2 -

  _ex:_
  ```cpp
  ```

### 

  _ex:_
  ```cpp
    
  ```

### Designated init. sentaxi {} ile gecerli degil

  _ex:_
  ```cpp
    struct Nec{
        int a, b, c;
    };
    
    Nec{.b = 6};    // gecerli

    Nec n(.b = 6);  // gecersiz
  ```

### Life extension {} ile uygulanir, () ile uygulanmaz
  _ex:_
  ```cpp
    struct Nec{
        const int& r;
    };

    int foo()
    {
        return 1;
    }

    int main(){
        Nec n(.b = 6);

        Nec n{ foo() };     // life extension oldu
        
        Nec n((foo()));       // life extension yapilmadi
    }
  ```

====================================
# Brace ellision
  - {} icin gecerli
  - () icin gecersiz

  _ex:_
  ```cpp
    struct Data {
        int x, y;
        struct Nested {
            int a, b;
        } z;
    };


    // normal
    struct Data mydata = {1, 2, {3, 4}};

    // brace ellision, {}
    struct Data mydata = {1, 2, 3, 4};  // this is also ok, and same with above
                                        /  both in C and C++

    // brace ellision, ()
    Data mydata(1, 2, 3, 4);    // error, brace ellision is not supported for '()'
  ```

  _ex:_
  ```cpp
    struct Data {
        int a;
        int b = 10;
        struct Nested {
            int x;
        } c;
    };

    int main()
    {
        Data mydata{ 3 };
        Data mydata2( 3 );

        std::cout << mydata.a << mydata.b << mydata.c.x;    // 310
        std::cout << mydata2.a << mydata2.b << mydata2.c.x;    // 310
    }
  ```

## arrays

  _ex:_
  ```cpp
    int a1[]{1, 2, 3};  // legal
    int a2[](1, 2, 3);  // legal

    int a3[10]{1, 2, 3};  // legal
    int a4[10](1, 2, 3);  // legal
  ```

  Neden?
  1. makrolar
  2. 

  _ex:_
  ```cpp
    struct Nec {
        int a, b;
        struct Nested {
            int x;
        } c;
    };

    int main()
    {
        auto p = make_unique<Nec>(1, 2, Nec::Nested{ 3 });  // c++20, error
            // make unique applies perfect forwarding


        // ??


        vector<Nec> nvec;
        nvec. emplace_back(1, 2, Nec::Nested{3});
    }
  ```

===========================
# C++20, cok derleyici C++17 de de destekliyordu

  _ex:_
  ```cpp
    class Nec{
        int mx, my;
        friend void func();
    };

    Nec foo();

    void func()
    {
        auto [x, y] = foo();
    }
  ```

?? kafa gitti

  _ex:_
  ```cpp
    class Nec {
    public:
        std::vector<int>& get_values();
    };

    Nec get_nec();

    int main()
    {
        for (auto x: get_nec().get_values())
        {
            // dangling reference since life of temporary object is not extended
        }

        for(auto Necx = get_nec(); auto item : necx.get_values())
        {
            ...
            // ok
        }
    }
  ```

  _ex:_
  ```cpp
    int main()
    {
        std::vector<int> ivec{1, 3, 5, 7, 9};

        for(std::size_t i{}; auto val : ivec)
        {
            std::cout << "{" << i++ << ", " << val << "}\n";
        }
    }
  ```

  _ex:_
  ```cpp
    struct Nec {
        int a : 4;  // bit-field member
        int b : 4;
    };
  ```

  _ex:_
  ```cpp
    // error in c++17, ok in C++20
    struct Nec {
        int a : 4{2};
        int b : 4 = 5;
    };

    // this it ok
    struct Nec {
        int a : 4;
        int b : 4;

        Nec() ???
    };
  ```

==========================================
# typename keyword for dependant types
  - C++17: dependenty type lar icin typename kullnmak zorudayiz
    oysa bazi yerlerde bunun alternatifi yok
    ??? static

  - C++20: kullanilan dependent type icin baska bir ihtimal yoksa, tur olmak zorundaysa typename i kullanmak zorunda degiliz. 

  _ex:_
  ```cpp
    template<typename T>
    T::Neco func();         // error in c++17, ok in c++20
  ```

  _ex:_
  ```cpp
    template<typename T>
    void func(T::Neco);     // error in both c++17 and c++20

    template<typename T>
    void func(typename T::Neco);     // ok in both c++17 and c++20
  ```

  _ex:_
  ```cpp
    template <typename T>
    struct PointerTrait {
        using Ptr = void*;
    };

    template<typename T>
    struct Nec {
        using Ptr = PointerTrait<T>::Ptr;
    };

    // error in c++17, ok in c++20
  ```

  _ex:_
  ```cpp
    template <typename T>
    struct PointerTrait {
        using Ptr = void*;
    };

    template<typename T>
    struct Nec {
        using Ptr = PointerTrait<T>::Ptr;
        T::Neco foo();  // ok
        
        T::Neco foo(T::Ali x)   // ok
        {
            return static_cast<T::Neco>(x); //ok
        }

        auto f() -> Nec::<T>:: ???
    };

    // error in c++17, ok in c++20
  ```

==============================
# A::B::inline C
## reminder: inline namespace

  _ex:_
  ```cpp
    namespace A::B {
        namespace C {
            int x;
        }
    }

    namespace A2::B2 {
        inline namespace C2 {
            int x;
        }
    }

    int main()
    {
        A::B::x // error
        
        A2::B2::x // ok
    }
  ```

  _ex:_
  ```cpp
    // C++20
    namespace A::B::inline C{
            int x;
    }

    int main()
    {
        A::B::func();
    }
  ```

==================================
# array new ile deduction
  _ex:_
  ```cpp
    int a[]{1, 2, 3};   // derleyici array size ini  anlar

    // ancak su c++17 de hataydi

    int* p = new int[]{1, 2, 3};    // error in C++17, ok in C+++20

    char *p = new char[]{"necati"}; // error in C++17, ok in C+++20

    int* p = new int[1, 2, 3]; 
  ```

==================================
# Negative sayilarin gosterimi 2 ye tumleyen olmak zorunda.

  _ex:_
  ```cpp
    // left shif of negative values

    int x = -1;
    x << 1;         // ub in C++17, ok in C++20

    // 111111111
    // 111111110
  ```

  _ex:_
  ```cpp
    int x = INT_MAX;
    
    x <<= 1;
  ```

##                      filling
    unsigned              0
    signed positive       0
    signed              implementation defined
            
  _ex:_
  ```cpp
    int x = -1;

    if((x >> 1) == -1);     // 
        cout << "dogru;"
    else
        cout << yanlis;

    // result was implementation defined
  ```

==============================================

## reminder: ??

  _ex:_
  ```cpp
    struct Data{
    public:
        Data(Data&&) noexcept(false);
    };

    class Myclass {
    public:
        Myclass(Myclass&&) = default;
        Data x;
    };

    int main()
    {
        static_assert(std::is_nothrow_move_constructible_v<Myclass>);
    }

    ??
  ```

simdi hem default edip hem de exception kosulunu degistirebiliriz
  _ex:_
  ```cpp
    struct Data{
    public:
        Data(Data&&) noexcept(false);
    };

    class Myclass {
    public:
        // legal in C++20, error before
        Myclass(Myclass&&) noexcept = default;
        Data x;
    };

    int main()
    {
        static_assert(std::is_nothrow_move_constructible_v<Myclass>);
    }
  ```

  _ex:_
  ```cpp
    struct Data{
    public:
        Data(Data&&) noexcept(false) = default;
    };

    int main()
    {
        static_assert(std::is_nothrow_move_constructibel_v<Data>);
    }
  ```

========================================================
# conditionally explicit ctor 
## reminder: explicit ctor
  _ex:_
  ```cpp
    class Myclass {
    public:
        Myclass();
        explicit Myclass(int);
    };

    Myclass m = 1;  // error

    Myclass m;
    m = 1

    ??
  ```

## conditionally explicit
  - ya ctor template olacak
  - yada sinif bir template olacak

  _ex:_
  ```cpp
    template<typename T>
    struct Nec {
        explicit(std::is_integral_v<T>) Nec(T);
    };

    Nec mynec = 12;     // error
    
    Nec mynec = 1.2;    // ok
  ```

  _ex:_
  ```cpp
    template<typename T>
    struct Nec {
        explicit(std::is_convertible_v<T>) Nec(T);
    };

    Nec mynec = 'A';     // ok, char can be converted to int

    Nec<char> x = 'A';          // ok
    Nec<char> x = 12;           // ok
    Nec<string> x = "murat";    // error    ???
  ```

============================================================
?? -> narrowing conversion
  _ex:_
  ```cpp
  ```

============================================================
# type alias kullanildiginda deduction yapilma

  _ex:_
  ```cpp
    template<typename T>
    using ipair = std::pair<int, T>;

    int main()
    {
        ipair<double>   // ipair<int, double>

        ipair<double> p{1, 2.3};    // ok

        ipair p{1, 2.3};    // was error in C++17, ok in C++20
    }
  ```
=========================== Concept =========================================
# template metaprogramming problemleri
  - zor ve karmasik
  - requirements, constraints
  - okunmasi cok zor
  - hata mesajlari kotu
  - debugging compile-time i cok uzatiyor

## concept
 - okunmasi yazilmasi daha kolay (tmp for eveyone)
 - requirements and constrains can be defined in a better way
 - hata mesajlari daha kolaylasti


## concept ve requires kullanarak
   template parametreleri uzerindeki kisitlamalri belirleyebiliriz

  _ex:_
  ```cpp
    template<std::integral T>   // concept type
    void func(T x);

    // integral concept requirements lari saglamazsa, bu template devre disi kalir
    // -> bu da fonksiyon cagrisini gecersiz kilar
    // -> SFNIAE de oldugu gibi baska fonksiyon varsa onlara bakilir


    func(1.2);  // syntax error
                // shorter error message: fun: assaciated constraints are not satisfied

  ```

  _ex:_
  ```cpp
    template<std::integral T>   // concept type
    void func(T x);

    func<int>;  // ok
    func<double>;  // error 

    // T integral conceptinin requirementtlerine uyan bir tur olmak zorunda
  ```

  _ex:_
  ```cpp
  template<typename T>
  void func(std::integral<T> x)
  ```

## concept icin constraint yazma
 ``requires`` keyword

  _ex:_
  ```cpp
  template<typename T>
  requires (sizeof(T) > 2) // ad-hoc boolean expression
  ```

### ``requires`` keyword unun yazimi
  - template basligindan sonra geri donus turunun yazilmasindan once
  - function parameter parantezinden sonra

  _ex:_
  ```cpp
    template<typename T>
    requires(sizeof(T) > 2)
    void func(T x)
    {
        ... 
    }
  ```

  _ex:_
  ```cpp
    template<typename T>
    void func(T x) requires(sizeof(T) > 2)
  ```

  _ex:_
  ```cpp
    template<typename T>
    void func(T x) requires (std::is_integral_v<T>)
  ```

  _ex:_
  ```cpp
    template<typename T>
    requires (std::is_integral_v<T>)
    class Myclass{};

    int main()
    {
        Myclass<double> x;  // error
        Myclass<int > y;    // ok 
    }
  ```

## require clause olusturma yollari
  1. ad-hoc condition yazma
  2. conceptin kendisi de compile time da bir boolean deger olusturabilir
  3. requires expresssion

## requires clause
  _ex:_
  ```cpp
    template<typename T>
    requires(std::is_integral<T>)
    void foo(){}
  ```

  _ex:_
  ```cpp
    template<typename T>
    requires (sizeof(T) > 8) && 
  ```

  _ex:_
  ```cpp
    template<typename T>
    requires (sizeof(T) > 8) &&     // constant expression 
        requires(T p) {                  // requires expression
            typename T::value_type;
        } && std::integral<T>       // ??
  ```

  _ex:_
  ```cpp
    template<typename T>
    requires (sizeof(T) > 8) &&
        requires(T p) {
            typename T::value_type;
            *p;                     // *p must be valid
            p[0];                   // must be valid
        } && std::integral<T>
  ```

  _ex:_
  ```cpp
    template<typename T>
    concept harun = sizeof(T) > 10 && std::is_integral_v<T>;
  ```

  _ex:_
  ```cpp
    template<typename T>
    requires (sizeof(T) > 10) && requires(std::is_integral_v<T>)    ??
  ```

### logical operators ||, &&
  _ex:_
  ```cpp
    template<typename T>
    requires std::integral<T> || std::floating_point<T>
    void func()
    {}

    int main()
    {
        func<int>();
        func<double>();
        func<int*>();
    }
  ```

  _ex:_
  ```cpp
    template<typename T>
    requires (sizeof(int) > 4) && sizeof(T) > 16
  ```

### type traits
  _ex:_
  ```cpp
    template<typename T>
    requires (std::is_pointer_v<T>)
  ```

  _ex:_
  ```cpp
    template<typename T, typename U>
    requires (sizeof(T) > sizeof(U))
  ```

### conceptlerin kendisini kullanabiliriz
  _ex:_
  ```cpp
    template<typenmae T, typename U>
    requires std::convertible_to<T, U>
  ```

  _ex:_
  ```cpp
    template<typenmae T, typename U>
    requires (sizeof(int) < sizeof(long)) 
  ```

### non-type parametre icin de condition olusturulabilir
  _ex:_
  ```cpp
    template<std::size_t n>
    requires(n > 20)
    class Myclass {};

    int main() 
    {
        Myclass<200> n;     // ok
        Myclass<10> n;      // error
    }
  ```

  _ex:_
  ```cpp
    template<typename T>
        requires(false)     // ok
  ```

  _ex:_
  ```cpp
    template<typename T>
    requires(std::is_pointer_v<T> || std::is_same_v<T, std::nullptr_t>);
  ```

  _ex:_
  ```cpp
    // T turu string turune donusturulemeyecek tur

    template<typenmae T>
    requires(std::is_convertible_v<T, std::string>)
  ```

  _ex:_
  ```cpp
    template<typename T>
    constexpr T foo(T x, T y);

    template<typenmae T, int x, int y>
    requires (foo(x, y) > 10)
    class Myclass {};
  ```

## requires expression
  _ex:_
  ```cpp
    template<typename T>
    concept Neco = requires (T x) {
        typename T::value_type;

        x + x;
        x > x;
    };
  ```

  _ex:_
  ```cpp
    template<typename T>
    concept Neco = requires (T x) {
        x == nullptr;   // T turunden nesneler, nullptr sabiti ile esitlik karsilastirmasina sokulabilir

        x.foo();        // T turunun foo isimli bir fonksiyonu olmak zorunda
    };
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
