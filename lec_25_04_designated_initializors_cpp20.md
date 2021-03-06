# (C++20) designated initializers
  - C de olan designated initializers ile C++ da tanimlanan tool ile farklililari var.
  
  - C de dizilerde yogun olarak kullanilir
    C++ da array ler de bu syntax yok

## reminder: designated initializers in C
  _ex_
  ```cpp
    int a[10] = {[5] = 9, [7] = 3};
        // element 5 and 7 are initialized, rest is 0

    int b[10] = {[9] = 9, [2] = 3};     // this is also legal
    
    int c[10] = {[9] = 9, [2] = 3, [4] = 7};     // this is also legal
  ```

### normal init. ile designated init can be combineds
  _ex_
  ```cpp
    int a[10] = {1, 2, [5] = 9};
  ```

  _ex_
  ```cpp
    int a[] = {
        [4] = 7,
        [19] = 19, 
    };  // size of this array is 20
  ```

### 2-dimensional arrays
  _ex_
  ```cpp
    int a[4][10] = {
        [3] = {1, 2, 3, 4, 5, 6};
    };  // ok 
  ```

  _ex_
  ```cpp
    int a[4][10] = {
        [3] = {[5] = 7};
    };  // ok 
  ```

### can be used in struct as well
  _ex_
  ```cpp
    struct Data{
        int a, b, c;
        char str[20];
    };

    struct Data mydata = {.b = 40};     // diger elemanlar default init edilir
  ```

  _ex_
  ```cpp
    struct Date {
        int day, mon, year;
    };

    int main()
    {
        struct Date mydate { .day = 5, .mon = 12, .year = 1987};
    }
  ```

## C++20, designated init
  - bir aggreagate 
    * dizi turu
    * sinif turu when
      - private or protected non-static data olmayacak, 
      - inherited, 
      - virtual fonk olmayacak, 
      - private ...???

  _ex_
  ```cpp
    struct Point{
        double x;
        double y;
        double z;
    };

    int main()
    {
        Point p;

        p.x = 3.4;


        Point p; //
    }
  ```

  _ex_
  ```cpp
    // c++20
    
    Point p = {
        .x = 3.0,
        .y = 4.0,
        .z = 6.0
    };
  ```
## sirayi degisteremeyiz

  _ex_
  ```cpp
    struct Point{
        double x;
        double y;
        double z;
    };

    Point p = {
        .y = 4.0,
        .x = 3.0,
        .z = 6.0
    };

    // error, sirayi degistiremeyuz
  ```

  _ex_
  ```cpp
    struct Point{
        double x;
        double y;
        double z;
    };


    std::vector<Point> pvec;

    pvec.push_back(Point {.x = 1.1, .y = 2.2, .z =3.3});
  ```

## ilk deger verirken kume parantezi kullanabiliriz 
  _ex_
  ```cpp
    struct Point{
        double x;
        double y;
        double z;
    };
    
    // ok
    Point p = {
        .y {4.0},
        .x {3.0},
        .z {6.0}
    };

    // below is error
    Point p2 = {
        .y (4.0),
        .x (3.0),
        .z (6.0)
    };
  ```

  _ex_
  ```cpp
    struct Date {
        int year;
        int mon;
        int day;
    }

    Date mydate{.year = 1987, .mon = 5, .day = 3};
  ```

## sirayi degistiremem ama atlama hakkim var, default degeri olacak
   - inclass init kullanilmissa o degeri alacak

  _ex_
  ```cpp
    struct Point{
        double x;
        double y = 1.1;
        double z;
    };

    Point p = {.z = 5.5};   // ok

    cout << p.x < p.y << p.z; // 0, 1.1, 5.5
  ```

  _ex_
  ```cpp
    struct Point{
        double x;
        double y = 1.1;
        double z;
    };

    Point p = {.y = 2.1, .z = 5.5};   // ok

    cout << p.x < p.y << p.z; // 0, 2.1, 5.5
  ```

ozet C ve C++ arasi fark: 
    - ??
    - sira onemli,
    - normal ilk deger verme ve designated C++ da birlikte kullanilamaz,
    - veri elemaninin kendisi de bir aggregate ise ??

  _ex_
  ```cpp
    struct Time {
        int min;
        int hour;
    };

    struct Date{
        int year;
        int month;
        int day;
        Time time;

        static int hmode;   // still an aggregate
    };


    int main()
    {
        Date d1 = {hmode = 0};  // error, static elemana ilk deger verilemez
                                // (cunku main baslamadan verilir) 

        Date x = {.month = 3, .year = 1998};    // error, unordered


        // I cann't mix designated and normal init.
        Date x = {199s, .day = 12};     // error

        // error?
        Date x = {
            .time.min = 24;
        };
    }

  ```

  _ex_
  ```c
    // this is C code

    struct A {int x, y;}
    struct B {struct A a;}

    int main()
    {
        struct A a = {.y = 1, .x = 3};      // C: ok, C++: error

        int ar[] = { [2] = 6 };      // C: ok, C++: error

        struct B b = { .a.x = 10 };  // C: ok, C++: error

        struct A a2 = { .x = 5, 34 }; // C: ok, C++: error

    }
  ```

  _ex_
  ```cpp
    struct Data{int a, b, c, d;};

    Data mydata{ .a = 5, .a = 6; }  // C: ok, C++: error
  ```

## unionlardda da ilke deger verebiliriz:
  - sadece 1 tane initializer kullanabiliriz
  _ex_
  ```cpp
    union Nec{int a; double b;};
    
    Nec n1 == {.a = 45};
    
    Nec n2 == {.b = 45};
    
    Nec n3 == {.a = 45, .b = 1.3};  // syntax error
  ```


# Advantages
  - kodun kolay okunmasi
  - hangi elemani ilk deger verildigi acikca gorunur
  - esnek, belirli elemanlara ilk deger verilmeyip default member init veya
    default init ile ilk deger almasi saglanabilir
  - C99 ile ortak syntax a yakin
  - bu extension olarak kullaniliyordu (yaygin)

  _ex_
  ```cpp
    struct ScreenConfig {
        bool scale = false;
        bool full_screen_mode = false;
        int bits { 32 };
        int plane { 3 };
    };


    ScreenConfig  myconfig = { true, false, 64, 2}; // hangi degiskenlere hangi
                                                    // deger gittigini gormek zor
  ```

===========================================================================
# std::format (C++20)
  - type safe olmasi
  - hizli calismasi
  - extensible for user-defined types
  - localizable, translateble
  - readable

## printf
  - [+] cok hizli (ozellikle iostream ile karsilastirildiginda)
  - [+] okunabilir
  - [-] type-safe degil
  - [-] user-defined type lar icin kullanilamaz
  - [+] local- ???

## iostream
  - [-] interface i cok kotu, C++ in en cok elestirilen kutuphanesi
  - [-] hantal, yavas
  - [-] okunmasi zor
  - [-] formatlama ve argumanlar birbirinde ayri degil
  - [+] user-defined type icin extensible  
        kendi sinifimiz icin operator<< yazilabilir

  _ex_
  ```cpp
    using namespace std;

    time_t timer;
    time(&timer);
    auto p = localtime (&timer);

    ostringstream oss;

    // format soyle olsun: 02_05_1987_14_52_45
    
    oss << setw(2) << setfill('0') << p-> tm_mday << "_" >> ...;
  ```

> std::format iostream ve printf deki defficiencylere sahip degil ve efficient 

## 
  - baslik dosyasi format.hpp
  - std::format()
    * parametresi string_view'e donusturulebileck tur
    * return type: string

  _ex_
  ```cpp
    #include <format>

    int main()
    {
        auto s = std::format();     // can be stored as a string

        cout << std::format();      // cikis akimina verilebilir (string)
    }
  ```

  _ex_
  ```cpp
    #include <format>

    int main()
    {
        auto s = std::format("kadir c++ programcisidir");

        cout << std::format("kadir c++ programcisidir") << "\n";
    }
  ```

## "{}" placeholder -> formatlama bilgisini icerir 
  - formatlama bilgisi yazilmadiginda "default formatlama" ozelligi kullanilir.
    ex: default format for integer types is 10-base


  ```cpp
    #include <format>

    int main()
    {
        std::string name{"necati"}
        int cnt{20};


        std::cout << std::format("{} kisisinin {} kitabi var", name, cnt) << "\n";
    }
  ```

## 

  _ex_
  ```cpp
    cout << format("{} {}\n", "merhaba", "dunya");
  ```

## index berlirtebiliriz
  _ex_
  ```cpp
    
    cout << format("{1} kisiisnin {0} kitabi var\n", cnt, name); // name cnt
        // necati kisisinin 20 kitabi var
    
    cout << format("{} kisiisnin {} kitabi var\n", cnt, name); 
        // 20 kisisinin necati kitabi var

  ```

## ayni arguman birden fazla kez kullanilabilir
  _ex_
  ```cpp
    int x = 47802;

    cout << format("{0} {0} {0}", x);

    
    cout << format("{0:b} {0:x} {0}", x);
  ```

## kume parantezi icine formatlama ozelligi girilebilir
  _ex_
  ```cpp
    cout << format("{0:b} {0:x} {0:X}  {0:d} {0:o}", x);
        - binary, hexadecimal, decimal, onluk
  ```

## genel yapi
  - {}      -> default 
  - {n}     -> default, index
  - {:fmt}  -> formatlama nitelikleri ile formatlanacak
  - {n:fmt} -> index, formatlama bilgisi

## format specifier
  - printf de oldugu gibi bir takim bilesenler var
  - genellikle opsiyonel
  - bazilari digerlerinin varligini gerektirir

pattern su
  [fill][alignment][sign][#][0][width][.prec][L][type]

  - # -> formatlamanin niteligini belirli formatlama ozellikleri icin degistirir
       ex: 10 luk sayi siteminde ???

  - 0 -> padding zero

  - L -> localization properties

  - type -> also includes number base info


## indexing
    - fazla arguman olmasi bir hata degil
      ancak belirli hatalar runtimeda exception a sebep olur
    - compile time da bulunamayan hatalar exception throw eder

  _ex_
  ```cpp
    cout << format("{} {} {}\n", "Arda", "Ozlem", "Melih");
    
    cout << format("{1} {0} {2}\n", "Arda", "Ozlem", "Melih");
            // Ozlem Arda Melih
   
    cout << format("{2} {1} {0}\n", "Arda", "Ozlem", "Melih");
            // Melih Ozlem Arda

    cout << format("{2} {1} {0}\n", "Arda", "Ozlem", "Melih", "Tuncay"); // ok
    
    
    int x = 47802;
    cout << format("{0:b} {0:x} {0:o}\n", x);
            // 101010... baba 13.. 
  ```

### pozisyonel indexleme ile manuel indexleme birarada kullanilamaz
  _ex_
  ```cpp
    // hata compile-time da yakalanamadi, run time hatasi verdi
    
    try (const std::exception& ex){
        cout << format("{1} {0} {}\n", "Arda", "Ozlem", "Melih");
    } catch {
        cout << ex.what();
    }
  ```

### formata arguman olarak gonderebilmek icin compile time sabiti olmasi gerekir
 > vformat a runtime arguman gonderilebilir

  _ex_
  ```cpp
    int x = 723423423;

    cout << format("|{}|\n", x);    // 10 luk sistem
    
    cout << format("|{0}|, |{0:d}|\n", x);    // 10 luk sistem
    
    cout << format("|{0:x}|, |{0:X}|\n", x);    // kucuk/buyuk harf hexadecimal

    cout << format("|{0:#x}|, |{0:#X}|\n", x);   // kucuk/buyuk harf  (onek+hexadecimal)

    cout << format("|{0:o}|, |{0:#o}|\n", x);   //  hexadecimal, 0hexadecimal
    
    cout << format("|{0:b}|, |{0:#b}|, |{0:#B}|\n", x);   // 10001, 0b1001.., 0B101010 
  ```

  _ex_
  ```cpp
    cout << format("{} {}\n", true, false);         // true, false  (default, 1 degil)
    
    cout << format("{:s} {:s}\n", true, false);     // true, false
    
    cout << format("{:b} {:b}\n", true, false);     // 1, 0

    cout << format("{:#b} {:#b}\n", true, false);     // 0b1, 0b0
    
    cout << format("{:#rBb} {:#B}\n", true, false);   // 0B1, 0B0
    
    cout << format("{:x} {:x}\n", true, false);       // 1 0 
    
    cout << format("{:#x} {:#x}\n", true, false);     // 0x1 0x0
    
    cout << format("{:#X} {:#X}\n", true, false);     // 0X1 0X0
  ```

## genel formul: resim, tablo

### gercek sayi
  _ex_
  ```cpp
    double dval = 1234.78981232;

    cout << format("{:f}\n", dval); // fixed 12323.1321312

    cout << format("{:e}\n", dval); // ustel: 7.812322e+11
    cout << format("{:E}\n", dval); // ustel: 7.81232E+11
  ```

  _ex_
  ```cpp
    cout << format("{}\n", "necati ergin");
    
    cout << format("{:s}\n", "necati ergin");
    
    cout << format("{:s}\n", 10 > 5);
  ```

  _ex_
  ```cpp
    int x = 8765;

    cout << format("{:18}{}", x, "ali") << "\n";    // x i 18 karakterlik alan
                                                    // fill char = space

    // hizlama
    format("{:<18}{}", x);      // x i 18 karakter, sola dayali (<-)
                                // |8765             |

    format("{:>18}{}", x);      // x i 18 karakter, saga dayali (->)
                                // |             8765|
    
    format("{:^18}{}", x);      // x i 18 karakter, orta
                                // |     8765        |

    // fill char
    format("{:.<18}{}", x);      // x i 18 karakter, sola dayali (<-)
                                 // |8765.............|

    format("{:.>18}{}", x);      // x i 18 karakter, saga dayali (->)
                                 // |.............8765|
    
    format("{:.^18}{}", x);      // x i 18 karakter, orta
                                // |.....8765........|
  ```

## precision
  _ex_
  ```cpp
    // precision
    cout << format("{:6}{}", "necati ergin");      // 6 -> yazma alani genisligi
                                                   // necati ergin
    
    cout << format("{:.6}{}", "necati ergin");      // 3 -> sadece 6 karakteri yazdir
                                                    // nec

   
    int x = 5;
    cout << format("{:.{}}{}", "necati ergin", x);    // precisioni belirleyen x
                                                      // necat

    int x = 762374;
    cout << format("{:15x}\n", x);    // 15 karakterlik alana hexadecimal 
    
    cout << format("{:#15X}\n", x);    // 15 karakterlik alana hexadecimal 
                                        // 0X23123

    cout << format("{:<#15X}\n", x);    // 15 karakterlik alana hexadecimal 
                                        // sola dayali

    cout << format("{:_<#15X}\n", x);   // 15 karakterlik alana hexadecimal 
                                        // sola dayali
                                        // fill char: _
  ```

## sign-> ???> kontrol et
  _ex_
  ```cpp
    cout << format("|{0}|\n", 94);     // 94
    
    cout << format("|{0}|\n", -94);     // -04
    
    cout << format("|{0}| "{0:-}"\n", 94);     // 94, - isaretini goster
    
    cout << format("|{0}| "{0:-}"\n", -94);     // -94, - isretini goster
    
    cout << format("|{0}| "{0:- }"\n", -94);     // poziifse bosluk gonder
  ```