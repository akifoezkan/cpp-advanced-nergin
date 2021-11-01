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
