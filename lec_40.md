# yanlis anlamalar - covariance/unique_ptr
  1. sinifin sanal fonk. private olabilir
  2. ???

# resource return:
  - hata1: pointer dondurmek
  - hata2: shared_pointer dondurmek

> Fabrika fonksiyonlarinin geri donus degeri unique_ptr olmali
> fonksiyonu cagiran exclusive ownership isterse unique_ptr
> shared ownership isterse shared_ptr kullanir 


  _wrong:_
  ```cpp
    #include "car.h"

    class CarFactory {
    public:
        // gunumuzde nesne donduren fonksiyonlarin pointer olmasi hata gibi gorunuyor
        // geri dondurulen objenin sahibi belli degil
        static Car* create(std::string car_type)
        {
            if(car_type == "fiat")
            {
                return new Fiat;
            }
        }
    };
  ```
  _correct:_
  ```cpp
    #include "car.h"

    class CarFactory {
    public:
        static std::unique_ptr<Car> create(std::string car_type)
        {
            if(car_type == "fiat")
            {
                return new Fiat;
            }
        }
    };


    int main()
    {
        std::shared_ptr<Car> sptr{ CarFactory::create("fiat")};
    }
  ```
> unique_ptr den shared_pter ye donus var
> Dolayisiyla unique_ptr donduren fonksiyon, ownership in unique yada shared olma kararini, fonksiyonu cagirana birakir.

---
# Exception handling

## std::exception_ptr
  - ??
  - nullable type

> reminder:  
> nullable types:

temel varlik nedeni sunlarla ortaya cikar,
bunlar yakalanan exceptioni baska objeye yonlendirir:
  - rethrow_exception
  - current_exception
  - make_exception_ptr


std::exception_ptr
  - ya bos, yada
  - yakalanmis ve rethrow edilebilir bir exception nesnesi

  _ex:_
  ```cpp
    int main()
    {
        using namespace std;

        exception_ptr ptr;
        
        std::cout << (ptr ? "Null degil" : "Null");   //null ptr

        if(ptr) { //... }
    }
  ```

su ise yarar:

  _ex:_
  ```cpp
    #include<exception>
    #include<stdexcept>

    void func(std::exception_ptr ptr)
    {
        try{
            std::rethrow_exception(ptr);
        }
        catch(cont std::out_of_range& ex)
        {
            cout << "hata yakalandi...\n";
            cout << ex.what() << "\n";
        }
    }

    int main()
    {
        vector<int> ivec(10);

        try {
            auto x = ivec.at(20);
        }
        catch(const std::exception& ex) {
            cout << "exception caught " << ex.what() << "\n";
            auto eptr = current_exception();
            func(eptr);
        }
    }
  ```

amac: 
  - bir exceptionin bir fonksiyondan baska bir fonksiyona aktarilmasi
  - aktarilan exception baska bir thread tarafindan kullaniliyor olabilir
  - aktarma sirasinda dinamik tur korunur

std::exception_ptr
- esitlik karsilastirilmasi yapilabilir (null degerler icind de)

> exception_ptr kopyalandiginda

## current_exception:
  - nothrow except

## rethrow
  - noreturn
  - kopyalama yapilip yapilamayacagi derleyiciye bagli
  - kopyalama yapilirsa ve bu sirada dinamik olarak heap den yer elde edilmezse std::badalloc throw eder

  [[noreturn]] void rethrow_exception(std::exception_ptr p)

standarda website
> std::exception_ptr bool turu harici hic bir ture donusturulemez
> ??



=> I had to leave, 

====== ara sonrasi
  _ex:_
  ```cpp
    // soru: exception kopyalama
    // dtor cagirilir, dinamik tur korunmaz
  ```

# terminate in cagirildigi yerler
  - uncaught exception
  - noexcept garantisi veren bir fonksiyondan exception gonderilmesi
    * Modern C++ da dtor noexcept olmak zorunda
   dtor exception gonderirse ve bu yakalanmazsa exception emit edilir
   bu durumda terminate cagirilri
  - rethrow statement


  _ex:_
  ```cpp
    class Myclcass{
    public:
        ~Myclass()
        {
            throw std::runtime_error{}
        }
    };

    ...
  ```

  _ex:_
  ```cpp
    void f()
    {
        throw std::out_of_range{"hata..."};
    }

    void foo() noexcept
    {
        f();
    }

    int main()
    {
        try() {
            foo();
        }
    }
  ```

  _ex:_
  ```cpp
    // f, rethrow statemetn
    // f i exception throw eden bir fonksiyon call etmezse terminate cagirili
    void f()
    {
        throw;
    }

    int main()
    {
        try {
i           f()
        }
        catch(...){
            std::cout << "hata yakalandi\n";
        }
    }

  ```
## function try block
  - normal fonksiyonlar icin hic bir faydasi yok
  - suralarda kullanilabilir:
    * fonksiyonun tamamini try blogu icine koyabiliriz
    * return statement catch blogu icine koyulabilir 

  _ex:_
  ```cpp
    void func(int x)
    try {
       throw std::runtime_error{"hata"};
    }
    catch (const std::exception &ex) {
        cout << "exception caught ... " << ex.what() << "\n";
        return x * x;
    }


    int main()
    {
        auto i = func(12);

        cout << "i = " << func(12);
    }
  ```

soru:
- function try block parametrelerin hayata gelmesi sirasinda gelen exception i yakalayamaz
  _ex:_
  ```cpp
    class Myclass{};

    Myclass func(Myclass x)
    try {
       throw std::runtime_error{"hata"};
       return x;
    }
    catch (const std::exception &ex) {
        cout << "exception caught ... " << ex.what() << "\n";
        return x;
    }
  ```

- catch blogunda return statement yoksa undefined behaviour
  _ex:_
  ```cpp
    try{
        throw 1;
    }
    catch (nt)
    {
       // no return 
    }
  ```

### When do we use function catch block
  - class constructo

  _ex:_
  ```cpp
    class Member {
    public:
        Member()
        {
            cout << "Member default ctor\n";
            throw 1;
        }
    };

    // Member mx hayata gelmezse Nec zaten hayata gelemez
    // This cannot catch the exceptions sent while constructing members
    class Nec {
    public:
        Nec()
        {
            try {

            }
            catch(int) {
                cout << "exception caught\n";
            }

        }
    private:
        Member mx;
    };
  ```

correct way to catch exception in constructor
  _ex:_
  ```cpp
    class Member {
    public:
        Member()
        {
            cout << "Member default ctor\n";
            throw 1;
        }
    };

    class Nec {
    public:
        Nec() try : mx{ 12 }
        {
        }
        catch(int) {
            cout << "exception caught\n";
            // throw yazmasak da exception yakalandiginda rethrow eder
            // yada kendimiz baska bir exception throw edebiliriz
            throw std::runtime_error{"member exception\n"};
        }
    private:
        Member mx;
    };
  ```
---
# a technique to
  _ex:_
  ```cpp
    // X and Y might throw exceptions that have the same type
    struct X{
        X(int) {
            std::runtime_error{"X error\n"};
        }
    };

    struct Y{
        Y(int) {
            std::runtime_error{"Y error\n"};
        }
    };

    class Nec {
    private:
        X mx;
        Y my;

    public:
        Nec() try i : mx{23}, my{34} {

        }
        catch(const std::exception& ex) {
            // I will write a different behaviour depending on the object that sends the exception
            // however both classes sends the same exception
        }
    };
  ```

  _ex:_
  ```cpp
    // X and Y might throw exceptions that have the same type
    struct X{
        X(int) {
            std::runtime_error{"X error\n"};
        }
    };

    struct Y{
        Y(int) {
            std::runtime_error{"Y error\n"};
        }
    };

    class Nec {
    private:
        X mx;
        Y my;
        enum Tracker {NONE, FIRST, SECOND}

    public:
        Nec(Tracker tracker = NONE) 
        try : mx((tracker=First, 1)), my((tracker = SECOND, 2)) {
            assert(tracker == SECOND);
        }
        catch(const std::exception& ex) {
            if(tracker == FIRST)
            {
                cout << "X threw exception\n";
            }
            else if (tracker == SECOND)
            {
                cout << "Y threw exception\n";
            }

            throw;
        }
    };
  ```
# exception dispatcher idiom
  _ex:_
  ```cpp
    // ...

    void handle_exception()
    {
        try {
            throw;  // rethrow
        }
        catch(const a_exception&) {
            
        }
        catch(const b_exception&) {
            
        }
        catch(const c_exception&) {
            
        }
    }

    int main()
    {
        try {
            f1();
        }
        catch(...) {
            handle_exception();
        }

        // ...
    }
  ```

# polymorphic exception
  _ex:_
  ```cpp
    class ExBase {
    public:
        virtual ~ExBase() = default;
    };

    class ExDerA : public ExBase {};
    class ExDerB : public ExBase {};
    class ExDerC : public ExBase {};

    void foo(ExBas& ex)
    {
        // other code
        throw ex;

        // here throw copies ex
    }

    int main()
    {
        ExDerA eda;

        try {
            foo(eda);
        }
        catch(const ExderA&)
        {
            cout << "ExDerA exception caught\n";
        }
        catch(const ExderB&)
        {
            cout << "ExDerB exception caught\n";
        }
        catch(const ExderC&)
        {
            cout << "ExDerC exception caught\n";
        }
        catch(...) 
        {
            cout << "Other exception caught\n";


            // enters here ???
        }
    }
  ```

  bunu soyle yapariz: yarim kaldi
  _ex:_
  ```cpp
    class ExBase {
    public:
        virtual ~ExBase() = default;
        virtual void raise()
        {
            throw *this;
        }

        virtual void raise() const
        {
            throw *this;
        }
    };

  ```

# idiom: Empty base optimization
  - use inheritance instead of having a composite member

  _ex:_
  ```cpp
    class Empty {
        // functions
        // no data memeber
    };

    class Myclass {
    public:
        Empty e;
    };

    int main()
    {
        cout << "sizeof(Myclass) = " << sizeof(Myclass) << "\n";    // 8
    }

    // Empty bos olmasina ragmen size i 8 byte artirdi
  ```

solution:
- not: erleyici bunu yapmak zorunda degil

  ```cpp
    class Empty {
    }; 

    class Myclass : Empty {
    public:
        int mx;
    };

    int main()
    {
        cout << "sizeof(Myclass) = " << sizeof(Myclass) << "\n"     // 4
    }
  ```

## c++20: no_unique_address

  _ex:_
  ```cpp
    class Empty {
        // functions
        // no data memeber
    };

    class Myclass {
    public:
        [[no_unique_address]] Empty e;
    };

    int main()
    {
        cout << "sizeof(Myclass) = " << sizeof(Myclass) << "\n";    // 8
    }
    
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
