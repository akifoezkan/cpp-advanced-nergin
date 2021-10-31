# Literal operators

* kendimiz yazarsak son ek '_' ile baslamali

    _ex:_  
        `7623_x      // user-defined literal`

    _ex:_  
        `"alican"s   // cpp literal`


## categories:

* __cooked__   -> dogrudan bir tamsayi/gercek sayi/karakter olarak fonksiyona arguman gecmesi

* __uncooked__ -> yazinin adresi const char* olarak gonderilir. Operator fonksiyon bunu karakter karakter okumak zorunda

  _ex:_

    `"alican"sv`  -> string_view  
    `"alican"s`   -> string

  _ex:_

```cpp  
    #include <chrono>

    using namespace std::chrono;

    constexpr auto x = 2h + 15min + 543s + 7651ms;  //everything is const
```

## ne ise yarar?

  _ex:_

```cpp    
        milliseconds ms;

        ms = 3445546;       // gecerli degil, bir duration deger gerekir
        ms = 3445546ms;     // ok
```
  _Note:_
        chrono sifir maliyet

## user-defined literal operator function parametresi sadece belirli turlerden secilebilir:

  => https://en.cppreference.com/w/cpp/language/user_literal  

| ( const char * )	                |(1)|
|-|-|
| ( unsigned long long int )	      | (2) |
  |( long double )	                  | (3) |
  |( char )	                          | (4) |
  |( wchar_t )	                        |(5)|
  |( char8_t )	                        |(6)	(since C++20)|
  |( char16_t )	                      |(7)|
  |( char32_t )	                      |(8)|
  |( const char * , std::size_t )	    |(9)|
  |( const wchar_t * , std::size_t )	  |(10)|
  |( const char8_t * , std::size_t )	  |(11)	(since C++20)|
  |( const char16_t * , std::size_t )	|(12)|
  |( const char32_t * , std::size_t )	|(13)|

## bir literal operator fonksiyonu nasil yazilir?
    - user-defined: ""\_name
    - compiler:     ""name
    - bir namespace e at, global namespace i kirletmemek icin
    - literal operator fonksiyon ismiyle cagirilabilir
    - yan etkileri olabilir

  _ex:_
```cpp
        // bu ornekte her seyi metre olarak kullanmak istiyoruz
        // yani 345_cm verildiginde biz otomatik 100 e bolucez
        // note that example below is cooked

        constexpr double operator""_m(long double val)
        {
            return static_cast<double>(val);
        }

        constexpr double operator""_cm(long double val)
        {
            return static_cast<double>(val / 100.);
        }

        constexpr double operator""_cm(long double val)
        {
            return static_cast<double>(val * 1000.);
        }

        int main()
        {
            constexpr auto val = operator""_m(34.5);

            constexpr auto valx = 34.5_m;

            constexpr auto m_distance = 0.05_km + 0.56_m + 34._cm + 345.1_mm;
        }
```
===========================================================================
## uncooked

  _ex:_    
```cpp
    double operator""_nec(const char* p)
    {
        for(int i = 0; p[i] != '\0'; ++i)
        {
            std::cout.put(p[i]);
        }

        return 5;
    }
```

_ex:_    
```cpp
    int main()
    {
        auto val1 = 987125_nec;                 // uncooked

        auto val = operator""_nec("987125");    // compiler bunu yazar:
                                                // => dikkat "numbers" (yazinin adresi)

        std::cout << "val = " << val << "\n";   // val = 5
    }
```

_ex:_    
```cpp
    constexpr double operator""_c(long double val)
    {
        return static_cast<double>(val);
    }

    constexpr double operator""_fah(long double val)
    {
        return static_cast<double>((val - 32) / 1.8);
    }

    int main()
    {
        constexpr auto diff = 38.6_c - 50._fah;
    }
```

  _ex:_    
```cpp
    constexpr std::size_t operator""_KB(unisgned long long val)
    {
        return static_cast<std::size_t>(val * 1024);
    }

    constexpr std::size_t operator""_MB(unisgned long long val)
    {
        return static_cast<std::size_t>(val * 1024 * 1024);
    }

    constexpr std::size_t operator""_GB(unisgned long long val)
    {
        return static_cast<std::size_t>(val * 1024 * 1024 * 1024);
    }

    int main()
    {
        std::array<char, 32_KB> ax;
        std::array<char, 1_MB> ax;
    }
```
===========================================================================
## Cooked, size girildi  

_ex:_    
```cpp
    Date operator""_date(const char *p, std::size_t)
    {
        return Date{p};

    }

    int main()
    {
        "18-10_2021"_date;

        operator""_date("18-10-2021", 11);

        std::cout << "18-10_2021"_date << "\n";
    }
```

  _ex:_    
```cpp
    std::string operator""_s2(const char* p, std::size_t)
    {
        return std::string(p) + p;
    }

    std::string operator""_s3(const char* p, std::size_t)
    {
        return std::string(p) + p;
    }


    int main()
    {
        auto str = "sercan"_s2;
        auto s = "enesalp"_s3;

        std::cout << str << \n << s << "\n";
    }
```

_ex:_    
```cpp
    constexpr int operator""_i(char c)
    {
        return c;
    }

    int main()
    {
        auto constexpr x = 'A';     // type: char
        auto constexpr x = 'A'_i;   // type: int

        cout << 'A'_i << '+'_i << '0'_i;
    }
```
