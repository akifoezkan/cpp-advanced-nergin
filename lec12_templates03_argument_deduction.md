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
