# Ilk 3 dakikayi kacirdim

---
library design decisions
- const correctness
- const span x =
- x = 
- x.swap(y)
---

# Span Library

  _ex:_
  ```cpp
    #include <span>

    // inline constexpr std::size_t dynamic_extent = std::numeric_limits<std::size_t>::max();

    template <typename T, std::size_t Extent = dynamic_extent>
    class Span {
        T* ptr;         // baslangic adresu
        size_t val;     // ne kadar oge, dinamik
                        // default deger max(size_t)

    };

  ```


## fixed (static) extent
  - kac eleman gosterilecegi turun bir parcasi
## dynamic (static) extent
  - eleman sayisi default olarak size_t nin en buyuk degerini alir

  _ex:_
  ```cpp
    int ar[5]{};
    span<int, 5> sp_s(ar);    // static_extent
    span<int>    sp_d(ar);    // dynamic_extent

    std::cout << "sizeof(sp_s) = " << sp_s.extent << " " << sizeof(sp_s) << "\n";     // ??
    std::cout << "sizeof(sp_d) = " << sp_s.extent << " " << sizeof(sp_d) << "\n";   // ??
  ```

## default ctorable olmasi icin
    - dynamic extent, yada
    - static extent ve span<int, 0>

  _ex:_
  ```cpp
    span<int> spx;      // ok
    span<int, 0> spy;   // ok
    span<int, 10> spy;  // error, not default ctorable
  ```
##
  _ex:_
  ```cpp
    vector<int> ivec {2, 5, 7, 9, 1, 8, 3, 4};

    span<int, 10> spx{ ivec };
    
    span<int, 5> spx{ ivec };   // ub
    
    span<int, 20> spx{ ivec };  // ub
    
    span<int> spx{ ivec };      // ok


    array<int, 5> ar;
    span<int, 10> spx{ar};  // syntax error (size can be understood at compile-time)
  ```

  _ex:_
  ```cpp
    vector<int> ivec {2, 5, 7, 9, 1, 8, 3, 4};
    array<int, 5> ar;

    span<int, 5> spx{ar.data()};    // ok   ??
    span<int, 5> spx{ar.data()};    // ok
  ```

  _ex:_
  ```cpp
    int a[20];
    std::array ar{1, 2, 3, 4, 5};

    span<int> s1 = a;    // ok
    span<int> s2 = ar;   // ok

    span<int, 20> s1 = ar;  // error ??

    span<int, 5> sp{ar.data(), 5};  // ??
  ```

## member functions
  - .back
  - .begin
  .data
  .end
  .extent
  first front last
  ...

  _ex:_
  ```cpp
    vector<int> ivec [2, 3, 5, 8, 12];

    span<int> sx{ ivec };

    sx.front() = 9;
    sx[3] = -1;

    for(auto val : ivec)
        std::cout << val << " ";
  ```

  _ex:_
  ```cpp
    vector<int> ivec {2, 3, 5, 8, 12};
    array<int> arr {2, 3, 5, 8, 12};

    span<int> vec_span {ivec};
    span<int> arr_span {arr};

    static_assert(std::same_as<decltype(vec_span), decltype(ar_span)>);

    std::cout << vec_span.extent << "\n";   // max(size_t)
    std::cout << arr_span.extent << "\n";   // max(size_t)

    std::cout << vec_span.size() << "\n";   // 5 
    std::cout << arr_span.size() << "\n";   // 5 
  ```

## .szie() 
  _ex:_
  ```cpp
    // CTAD
    vector<int> ivec {2, 3, 5, 8, 12};
    array<int> arr {2, 3, 5, 8, 12};
  ```

## CTAD
  _ex:_
  ```cpp
    vector<int> ivec {2, 3, 5, 8, 12};
    array<int> arr {2, 3, 5, 8, 12};

    span x{ ivec };     // span<int>
    span y{ arr };      // span<int, 5>

    cout << x.extent;   // max(size_t)
    cout << x.extent;   // 5 
  ```

  _ex:_
  ```cpp
    int ar[]{2, 3, 5};

    span x{ar};     // span<int, 3>

    cout << x.extent;   // 3 ?? 
  ```

  _ex:_
  ```cpp
    vector<int> ivec(100);
    span x{ivec.data(), 4}; // dynamic extent?? 

    array<int, 5> arr; 
    span x{ar.data(), 4};   // dynamic extent
  ```

##
  _ex:_
  ```cpp
    std::array a {1, 3, 5, 7, 9, 11, 13};
    std::array b {1, 3, 5, 7, 9, 11, 13};

    const span<int> x = a;
    // const span x = a;

    x[5] = 12;  // ok, const olan nesnenin kendisi

    x = b;  // syntax error
  ```

  _ex:_
  ```cpp
    std::array a {1, 3, 5, 7, 9, 11, 13};
    std::array b {1, 3, 5, 7, 9, 11, 13};

    span<const int> x = a;
    
    x = b;  // ok

    x[5] = 12;  // errr ??
  ```

  _ex:_
  ```cpp
    template<typenmae C>
    void func(const C& x)
    {
        x[3] = 5;
    }

    int main()
    {
        int ar[] = {1, 4, 56};
        span x = ar;

        func(x);    // ok
    }
  ```

-> span i ref semantik ile gecmenin anlami yok (kopyalama maliyeti yok)
  _ex:_
  ```cpp
    // bir spanin gosterdigi araligi yazdir

    template<typename T, std::size_t size>
    void span_print(std::span<T, size> sp)
    {
        if constexpr (size == std::dynamic_extent)
            std::cout << "(dynamic extent span\n)";
        else
            std::cout << "(fixed extent span\n)";

        for(const auto& elem : sp)
            std::cout << elem << '';
        std::cout << "\n";
    }

    int main()
    {
        vector<int> ivec;
        rfill(ivec, 20, Irand{0, 100});
        print(ivec);

        span<int> sx{ivec};

        span_print(ivec);       //error
        span_print(span{ivec}); //ok

        array ar{1, 2, 3, 4};   // error
        span_print(ar);         // ok


        array ca{1, 2, 3, 4};   // error
        span_print(ca);         // ok
    }
  ```
## Neler yok
   - asssign
   - swap
   - at (exception throw)
   - cbegin, cend
   - ??

  _ex:_
  ```cpp
    vector<double> dvec{1.2, 4.5, 7, 8, 9.3};

    span sx = dvec;

    std::cout << "size = " << sx.size() << "\n";
    std::cout << "size_bytes = " << sx.size_bytes() << "\n";

    sx.front() = -1.;   // ilkn eleman
    sx.back()  = -1.;    // son eleman
    
  ```

  _ex:_
  ```cpp
    auto val = sx.first(3); // val is span<double>, which is a subspan

    for(auto d : s){
        std::cout << d << "\n";     // first 3 elems
    }

    auto val = sx.last(3); // val is span<double>, which is a subspan

    for(auto d : s){
        std::cout << d << "\n";     // last 3 elems
    }
  ```

  _ex:_
  ```cpp
    vector<int> get_vec()
    {
        return {1, 2, 3, ...};
    }

    int main()
    {
        span<int> x{ get_vec() };   // syntax error

        span<int> x{get_vec().data()};   // ???, dangling pointer
    }
  ```

  _ex:_
  ```cpp
    vector<int> ivec{1, 2, 3, 4, 5};

    span sp{ ivec };

    for(int i = 0; i < 10; ++i)
    {
        for(auto x : sp) 
        {
            cout << x << " ";
        }
        cout << "\n\n\n";
        ivec.push_back(9);
    }

    // reallocation yaptigi noktada dangling pointer olacak
  ```

=========================================================
# Nranges
  - not evolution, it is a revolution
  - algorithms
  - numerics -> c++20 de karsiligi yok
    normal algoritmalar icin constraint/concept leri olusturmak zor degil.
    ancak numeric olagoritmalar icin bu daha zor, o yuzden gecikti


## STL e gore getirdigi faydasi/eklemeler
  1. constrained algorithms
  2. constraints are part of the interface
  3. dogrudand container i arguman olarak gonderebiliriz 
    algorithms.hpp nin cogunlugu iteratur cifti ister.
     ranges.hpp de container in kendisini gonderebiliriz 
  ```cpp
     ranges::algo(con);
     ranges::algo(ter, iter);;
     ranges::algo(ter, int);;
  ```
  4. sentinel kavramininin genisletilmesi.
     Range i belirleyen konumlarin farkli turlerden olabilmesi.
    sentinel: bir seyin sonunu ifade etmek  icin kullanilan ozel bir ???
    - ``vec.begin()`` ile ``vec.end()`` in turleri ayni olmak zorunda
    - onemi: it == .end() karsilastirmasini ortadan kaldirmaya yarar
       * ??? whule(1) ...
       * const char *p = foo();
         range -> p, p + strlen(p)

  5. projection parametreleri:
     algoritmalarin cok daha esnek kullanimini saglar

  6. declarative programlama yaklasimiyla, composable algoritmalar olusturulabilir

  _ex:_
  ```cpp
    #include <algorithm>

    vector<int> ivec;
    rfill(ivec, 100, Irand{0, 1000});

    sort(ivec.begin(), ivec.end());
    print (ivec);
  ```

  _ex:_
  ```cpp
    vector<int> ivec;
    rfill(ivec, 100, Irand{0, 1000});
    
    ranges::sort(ivec);
    print (ivec);
  ```

## namespace

  _ex:_
  ```cpp
    namespace std {
        namespace Ranges {
            // yeni algoritmalar

            // views

            namespace Views {
               // take 
            }
        }


        // namespace alias
        namespace Views = Ranges::Views;
    }


    std::ranges::views::take
    std::views::take
  ```
- ranges i kullanan algoritmalar, algorithms header da
  soyle bir sik kullanim var:
  _ex:_
  ```cpp
    namespace rng = std::ranges

    // then

    rng::sort


    // asla std::ranges icin using kullnmayin.
    // cunku algoritmalarla cakisacak
  ```

22.00
2 dakika geciktim

##
    range begin     sentinel

### ranges::range concept
  - begin(T) ve end(T) olmak zorunda
    ex: std::vector bir range dir

  _ex:_
  ```cpp
    // ranges::range concept
    ??
    // begin() ve end() olmak zorunda
    
  ```
## sentinel
  - template ...

### unreachable_sentinel
  - 

  _ex:_
  ```cpp
      // Klasik find algorithmasi
      // aradigimiz karkterin, aranan text icinde oldugunu kesinlikle bilsek bile
      // her satirda beg != end karsilastirmasi yapilacak


      template<typename Iter, typename T>
      Iter Find(Iter beg, Iter end, const T& val)
      {
          while(beg != end) {
              if(*beg == val)
                  return beg;
              ++beg;
          }
      }
  ```

  _ex:_
  ```cpp
    // hangi nesne cagirilirsa cagirilsin false return edecek
    template<typename T>
    struct unreachable_sentinel {
        bool operator==(const auto val) { return false; }
    };

    template<typename Iter, typename S, typename T>
    Iter Find(Iter beg, Iter end, const T& val)
    {
        while(beg != end) {
            if(*beg == val)
                return beg;
            ++beg;
        }
    }
  ```
 
  _ex:_
  ```cpp
    string = read_from_file(const char* pfilename);

    int main()
    {
        const auto str = read_from_file("bigfile.txt");

        // bu dasyada '\n' oldugundan eminiz ve ilk ciktigi konumu ariyoruz
        auto iter = std::find(str.begin(), std::unreachable_sentinel, '\n');
    }

  ```

  _ex:_
  ```cpp
    struct nullchar_t {
        bool operator==(auto pos) const
        {
           return *pos == '\0'; 
        }

        // != will be written by the compiler
    };

    inline constexpr nullchar_t  nullchar;

    int main()
    {
        const char* p = "blabalabal";

        for(auto ptr{p}; ptr != nullchar_t{}; ++ptr)
        {
            std::cout << *ptr << " ";
        }
        std::cout << "\n"';

        std::ranges::for_each(p, nullchar_t{}, [](char c) {std::cout.put(c);});
    }
  ```
