# constinit
 bir degiskeninin constant init. olarak yapilma garantisi verir

  _ex:_
  ```cpp
    int x = expr;   
    // expr const ise static init
    //          degilse dynamaic inir

    constinit int y = expr;
        // constant (static) initialization yapilamazsa syntax error olur
  ```

  _ex:_
  ```cpp
    constexpr int foo(int a)
    {
        return a * a;
    }

    int foo2(int a)
    {
        return a * a;
    }

    constinit int x = foo(5);   // ok

    constinit int x = foo2(5);   // syntax error
  ```

> constinit, const demek degil
> constinit static initialization yapilacak demektir
> ancak degeri degistirilebilir

> Bu sayede static initialization fiasko riskini ortadan kaldirir.

  _ex:_
  ```cpp
    constexpr int foo(int a)
    {
        return a * a;
    }

    constinit int x = foo(5);   // static initialization

    int main()
    {
        x = 5;  //ok; 
                // constinit, const demek degildir, degeri degistirilebilir
    }
  ```

  _ex:_
  ```cpp
    struct A {
        
    };

    constinit A ax; // ok, A nin ctoru constexpr
  ```

  _ex:_
  ```cpp
    class A
    {
    public:
        constexpr A(int val) : mx{val}{}

    private:
        int mx;
    }

    constinit A ax{12};     // ok
                            // thanks to constexpr ctor, static initialization

    constinit const A bx{12};     // ok

    int main()
    {
        ax.set(12);     // ok
                        // A constinit, not const

        bx.set(12);     // error, b is const
    }
  ```

> Dolayisiyla, sartlar saglandigi surece ctor lar constexpr yapilmali 

sonuc, static initialization fiasco dan nasil kurtulabiliriz?
- global degiskenleri constexpr yapmak iyi
- constinit bir sinama araci olarak kullanilabilir 

> ayni translation unit icinde yukaridan asagi dynamic initialization sirasi garanti altinda  
> Ancak tanimlar farkli dosyalarda oldugunda bir siralama soz konusu degildir

# nifty counter (schwarz) idiom

  _ex:_
  ```cpp
    class Neco {
    public:
        Neco()
        {
            std::cout << "ali";
        }
    }

    // Burada normalda static initialization fiasco olmasi gerekir?
    // Neco adinda bir global degisken hayata geldiginde, Neco nun ctor u cagilacak
    // cout da global bir nesne, cout un dynamic initializationi tamamlanmak zorunda


    // Neden syntax error degil?
    // nifty counter (schwarz) idiom
  ```

# inline degiskenler
  kullanildigi yerde tanimlanmis gibi muamele gorur

  _ex:_
  ```cpp
    inline int g = func();
  ```

# dtor larin cagirilmasi
  global degiskenklerden hangisinin dtor unun once cagirilacagi belli degildir

> garanti verilmedigi surece, ctor icinde global bir degisken kullanilmamali
> ayni kurallar destructer icinde gecerli.

# constexpr fonksiyon

  _ex:_
  ```cpp
    constexpr int func(bool b)
    {
        if (b)
            return 16;

        return std::getchar();
    }

    int x = func(true);     // const init
    int y = func(false);    // dynamic init

    constinit int x = func(true);     // ok, 
    constinit int y = func(false);    // syntax error 
  ```

#consteval

  _ex:_
  ```cpp
    consteval int foo(int x)
    {
        return x + 5;
    }

    constexpr int foo(int x)
    {
        return x + 5;
    }

    int main()
    {
        int a = 5;

       foo(10);     // ok

       foo(a);      // syntax error
    }

  ```

# Static initializtion fiasko

  _ex:_
  ```cpp
    // nec.cpp
    Nec nec;

    // other.cpp
    A ga = nec.foo()    // nec init edilmemis olabilir
  ```

# Teknikler (nasil engelleriz):
## leverage constant initialization

asagidaki Logger sinifi
  _ex:_
  ```cpp
    class Logger {
    public:
        constexpr Logger() = default;
            // dynamic initialize edilmeyecek

        void log(std::string_view msg); 

    private:
        std::optional<std::ofstream> m_ofs;
            // optional lazy initialization' a izin verir
            // construct on use
    };

    extern Logger;  // declaration
  ```

  _ex:_
  ```cpp
    // Logger.cpp

    CONSTINIT Logger logger;
        // bunu yapabilmemin nedeni ctor unun constexpr olmasi

    void Logger::log(std::string_view msg)
    {
        if(!m_ofs)
        {
            m_ofs.emplace("log.txt");
        }

        *m_ofs << msg << '\n';
        std::cout << msg << '\n'
    }
  ```

## ???
 - ayni zamanda inline keyword eklenmeden once header-only library gelistirme yolu

  _ex:_
  ```cpp
    class Myclass{
        // 
    };

    Myclass& gx()
    {
        static Myclass m;
            // dynamic initialization bu noktada yapilir

        return m;
    }
  ```

  _ex:_
  ```cpp
    A ax = gx().foo();  // static order fiasko olma ihtimali yok
  ```

  m programin basinda hayata gelme garantisi verir.

  _ex:_
  ```cpp
    // nec.cpp
    Nec& get_nec()
    {
        static Nec nec;
        return nec;
    }

    // a.cpp
    Nec& ... ??
  ```

    sunu yaparsak bu bizi koriumaz
  _ex:_
  ```cpp
    // nec.cpp
    Nec& get_nec()
    {
        static Nec nec;
        return nec;
    }

    Nec& gr = get_nec();
  ```
 

> ??? durumunda basimiz derde girebilir

### ikinci bir teknik

  _ex:_
  ```cpp
    class Myclass{
        // 
    };

    Myclass& gx()
    {
        static Myclass* p = new Myclass;

        return *p;
    }
  ```

  dinamik omurlu nesnenin dtoru delete edildiginde cagirilir.
  Ancak burada dinamik olusturulan Myclass nesnesinin destructor'i cagirilmaz
  Resource leak'e sebep olur ancak bunun kabul edilebilecegi durumlarda kullanilabilir.
  *p nin destructori cagirilmayacagindan, baska global nenelerinin destructor inda *p kullanilsa dahi probleme sebep olmayacak

### zekice bir template function

  _lazy_init.h_
  ```cpp
    template<typenmae Tag, typenmae T>
    class lazy_init {
    public:
        constexpr lazy_init() = default;

        T& get()
        {
            static T global;
            return global;
        }

        T& operator*()
        {
            return get();
        }

        T* operator->()
        {
            return &get();
        }
    };
  ```


  Neden birinci template parametresi var
  - birden fazla static instance yaratabilmek icin

  _ex:_
  ```cpp
    #include "lazy_init.h"
    class Myclass{
    public:
        void func();
        void foo();
    };

    class Nec{
    public:
        void f();
    };

    lazy_init<struct MyclassTag, Myclass> gm;   // incomplete tag for a new class
    lazy_init<struct NecTag, Nec> gm;

    int main()
    {

    }
  ```

Sorunlar
  - destructor problemini cozmez

## Nifty counter (schwarz)

> std::cout 

  _stream.h:_
  ```cpp
    class Stream{
    public:
        Stream();
        ~Stream();
    };

    extern Stream& gstream; // global stream object
                            // hangi sinifin ctor/dtor unda kullanilirsa kullanilsin hayatta olacak

    static struct StreamInitializer {
        StreamInitializer();
        ~StreamInitializer();
    }  gstreamInitializer;  // static initializer for every translation unit
  ```

  herhangi bir dosya bu headeri include ettiginde 
  static global degisken olan streamInitializer

  20 farkli dosya'da 20 tane ayri ayri (internal linkage turunden)
  gstreamInitializer olusturacak

  _stream.cpp:_
  ```cpp
    static int g_nifty_couter;
    static std::aligned_storage_t<sizeof(Stream), alignof(Stream)> g_stream_buffer;
        // memory for the object

    Stream& stream = reinterpret_cast<Stream&>(g_stream_buffer);
        // ayni bellek alani icin yeni bir pointer

    Stream:: Stream()
    {
        // initialize things
    }

    Stream:: ~Stream()
    {
        // clean up 
    }

    StreamInitializer::StreamInitializer()
    {
        if(g_nifty_counter++ == 0)
            new(&stream) Stream();  // placement new
    }

    StreamInitializer::~StreamInitializer()
    {
        if(g_nifty_counter++ == 0)
            new(&stream) Stream();  // placement new
    }
  ```

ctor:
include eden kaynak dosyalardan, ilk inlcude eden nesneyi hayata getirir.
diger kaynak dosyalar g_nifty_counter degerini artirir.

dtor:
butun kaynak dosyalarinin kullanimi bittikten sonra obje destroy edilir

========================================
# Singleton
## klasik Singleton
 - butun singletonlarda ortak nokta: sinifin copy-memberlerini delete ediyoruz
 - data race riski cok yuksek: multithreading
   cozum: manuel olarak call_once yada mutex vs
 - nesneyi dinamik olarak hayata getiririz ancak delete etmeyiz.
   duruma gore, nesnenin dtorunun cagirilmasi kritik ise mutlaka cagirilmali.
 - ctor private oldugu icin kalitim da kullanilamaz. 
   Bu gerekirse dtor, protected olmali

  _ex:_
  _csingleton.h:_
  ```cpp
    class CSingleton {
    public:
        CSingleton(const CSingleton&) = delete;
        CSingleton& operator = (const CSingleton&) = delete;

        void foo()
        {
            //...
        }

        static CSingleton& instance()
        {
            if(!p_instance)
            {
                p_instance = new CSingleton;
            }

            return *p_instance;
        }

    private:
        CSingleton();
        inline static CSingelton* p_instance{};
    };
  ```

  ```cpp
    #include "csingelton.h"

    int main()
    {
        CSingleton::instanece().foo();

        CSingleton& cs = ::instanece();
        cs.foo()
    }
  ```

## Klasik singleton, get_instance a olan gerekliligi yok ettik
  _ex:_
  ```cpp
    class DSingleton {
    ....

    int main()
    {
        DSingleton::foo(); // get_instance fonksiyona gerek kalmadi
    }
  ```

  _ex:_
  ```cpp
  namespace NSingleton {
        namespace {

        }

        void foo()
        {
            // use the data 
        }
  }


  //main.cpp

  ```

## amacimiz lazy initialization degilse, singleton nesnesini sinifin static degiskeni yapabiliriz
  - eager: not lazy, bu obje kullanilsa da , kullanilmasa da hayata gelecek
  - static initialization fiasco ya vulnerable 

  _ex:_
  ```cpp
    class ESingleton
    {
    public:
        // ....

    private:
        static ESingleton instance;
            // incomplete type, sadece bir bildirim
            // tanimlamasi cpp dosyasinda yapilacak

        ESingleton();
    };
  ```

## Mayor singleton
 - C++11 den sonra: Multithread calissada sorun yok
 - lazy initialization i saglar
 - destructor cagirlir, bu avantaj da olabilir/ dezavantaj da olabilir

  _ex:_
  ```cpp
  ```

### necati hocanin hack i
 - pointer i fonk icinde olustur
  _ex:_
  ```cpp
  ```

## TSingleton
  - destructor not called

  _ex:_
  ```cpp
  ```

## USingleton
  - sinifin ctor ve dtor u private
  - default delete yerine Deleter yazdik?  
  hem dtoru private yapayim. hem de ben delete edebileyim

  _ex:_
  ```cpp
    class USingleton {
    public:
        inline static std::unique_ptr<Usingleton, Deleter> mp_instance;
        USingleton();
        ~USingleton();

    public:
    };
  ```
  default delete yerine Deleter yazdik?
  - 


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
