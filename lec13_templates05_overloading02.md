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
