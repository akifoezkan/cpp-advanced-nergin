# Concurrency - 1
kullanim alanlari:
- arka planda farkli islemlerin takibi
- paralel programlama
- GUI programlama

## thread.h
  - arguman olarak bir callable alir 
  - thread olusturmanin bir maliyeti var, zaman kazanimi buna degmeyebilir

  - default co
 
  > joinable: thread nesnesine is yuku verilebilir

  _ex:_
  ```cpp
    void func()
    {
        std::cout << "func\n";

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    int main()
    {
        std::thread t;              // no workload
        std::cout << t.joinable();  // false, there is no workload

        std::thread t2 { func };    // has a workload
        std::cout << t.joinable();  // true, has a workload

        t.join();   // work is completed
        std::cout << t.joinable();  // false, previous workload is finished
    }
  ```

  > .join: synchronization point, block the program until the task is completed 

  _ex:_
  ```cpp
    void func()
    {
        std::cout << "func\n";
    }

    int main()
    {
        using namespace std;

        thread t { func };      // will start talking with OS and running func function

        // at this point:
        // there are 2 threads, main and t
        // (if t is not finished)

        t.join();               // block the program until func is finished

        // I am sure that func work is finished
    }
  ```

  _ex:_
  ```cpp
    void func()
    {
        for(int i = 0; i < 1000000; ++i) 
            std::cout << '*';
    }

    void foo()
    {
        for(int i = 0; i < 1000000; ++i) 
            std::cout << '?';
    }

    void bar()
    {
        for(int i = 0; i < 1000000; ++i) 
            std::cout << '-';
    }

    int main()
    {
        using namespace std;

        thread t1 { func };
        thread t2 { foo };
        thread t3 { bar };

        for(int i = 0; i < 1000000; ++i)
            cout << '.';

        // all the loops are executing (non-deterministic)

        t1.join();
        t2.join();
        t3.join();

        // all the loops are finished 
    }
  ```

  joinable thread destroy edilirse std::terminate cagirilir

  _ex:_
  ```cpp
    void func()
    {
        for(int i = 0; i < 1000000; ++i) 
            std::cout << '-';
    }

    int main()
    {
        using namespace std;

        set_terminate(my_terminate);    // change terminate function, it calls my_terminate

        thread t { func };

        // my terminate is called
    }
  ```

  madem joinable her thread nesnesinin join edilmesi gerekiyor.
  Neden RAII idiomu kullanilmamis?
  - C++20 de bu eksik kapatildi: jthread
  - neden baslangicta olusturmadilar?
    *  

==
talk recommendation: Jossitous, jthread, how it is developed
antonhy williams: concurrency in action
rainer grimm, tarzi guzel, konuya introduce etmek icin ama antonhy williams ile karsilastirilamaz
==

> bir thread program bitiminden once 
>  - ya join edilecek
>  - yada detach edilecek (programdan bagimsiz hale getirilir)

joinable olmayan bir thread join edilirse exception cagirilir

  _ex:_
  ```cpp
    thread tx;
    
    try {
        tx.join();
    }
    catch(const std::exception& ex) {
        cout << "exception caught" << ex.what() << "\n";
    }
   
    // exception cagirilir
  ```

## thread, parametreli fonksiyon input

  _ex:_
  ```cpp
    void foo(int x)
    {
        std::cout << "foo x = " x << "\n";
    }

    int main()
    {
        using namespace std;

        thread tx{foo, 10};
        tx.join();
    }
  ```

  _ex:_
  ```cpp
    void foo(int x, int y, int z)
    {
        std::cout << "foo x = " x << "\n";
        std::cout << "foo y = " y << "\n";
        std::cout << "foo z = " z << "\n";
    }

    int main()
    {
        using namespace std;

        thread tx{foo, 10, 20, 30};
        tx.join();
    }
  ```

  _ex:_
  ```cpp
    #include<vector>
    #include<thread>
    #include<iostream>

    void foo(const std::vector<int>& ivec)
    {
        for(const auto val : ivec)
            cout << val << " ";
    }

    int main()
    {
        using namespace std;
        
        std::vector<int> ivec{}
        thread tx{foo, ivec};       // ivec in kopyasi gonderildi
        
        tx.join();

        thread ty{foo, ref(ivec)};  // ivec in kendisi gonderildi 
        ty.join();
    }
  ```

  eger thread tarafindan calistirilacak bir sinifa argumanin kendisini gondermek istersek reference wrapper kullanmak zorundayiz

  _ex:_
  ```cpp
    #include<vector>
    #include<thread>
    #include<iostream>

    void foo(std::vector<int>& ivec)
    {
        for(const auto val : ivec)
            cout << val << " ";
    }

    int main()
    {
        using namespace std;
        
        std::vector<int> ivec{}

        thread tx{foo, ivec};   // syntax error
        tx.join();
    }
  ```


  _ex:_
  ```cpp
    #include<vector>
    #include<thread>
    #include<iostream>

    void foo(std::vector<int>&& ivec)
    {
        for(const auto val : ivec)
            cout << val << " ";
    }

    int main()
    {
        using namespace std;
        
        std::vector<int> ivec{}

        thread tx{foo, std::move(ivec)};   // move semantic 
        tx.join();
    }
  ```

### thread e gonderilecek fonksiyonun parametresi out parametres ise

  _ex:_
  ```cpp
    #include<vector>
    #include<thread>
    #include<iostream>

    ????

    void foo(int& r)
    {
        ++r;
    }

    int main()
    {
        using namespace std;

        int x{43};
        
        thread tx{foo, x};   // ?????? 
        tx.join();
    }
  ```

## Thread e parametre verme
her turlu callable thread e arguman olarak verilebilir
fonksiyon parametresi 
- referans degilse sorun yok
- referans ise reference wrapper kullanilmali
- sag taraf referans ise sag taraf degeri olarak gecebiliriz
- ??

  _ex:_
  ```cpp
    void foo(int x)
    {
        ???
    }

    int main()
    {
        auto fp = foo;
        thread tx{fp, 21};

        tx.join();
    }
  ```

  _ex:_
  ```cpp
    class Functor {
    public:
        void operator()(int x) const
        {
            cout << "x = " << x << "\n";
        }
    };

    int main()
    {
        using namespace std;

        Functor f; 
        thread tx{f, 12};

        tx.join();          // ok 
    }
  ```

  _ex:_
  ```cpp
    class Functor {
    public:
        void operator()() const
        {
            cout << "x = " << x << "\n";
        }
    };

    int main()
    {
        using namespace std;

        Functor f; 
        thread tx{Functor{}};   // ok
        
        // thread tx{Functor()};   // most vexing parse error

        tx.join();
    }
  ```

  21.45 e kadar ara
  burayi kacirdim 10 dk

  _ex:_
  ```cpp
    void func(const std::ostream& os);
        // ostream sinifi kopyalamaya kapali

    int main()
    {
        using namespace std;

        thread t1(func, std::cout);          // syntax error
        
        thread t2(func, ref(std::cout));     // ok
    }
  ```

## thread objects are non-copyable but moveable

  _ex:_
  ```cpp
    using namespace std;

    int main()
    {
        thread tx;

        thread ty(tx);  // ??? syntax error, non-copyable
        ty = tx;        // syntax error, non-copyable
    }
  ```

  _ex:_
  ```cpp
    void func() {}

    int main()
    {
        thread tx { func };

        cout << tx.joinable() << "\n";

        thread ty { std::move(tx) };

        cout << tx.joinable() << "\n";
        cout << ty.joinable() << "\n";

        ty.join();
        cout << ty.joinable() << "\n";  


        // true, false, true, false????
    }
  ```

thread is moveable:
  _ex:_
  ```cpp
    void func(std::thread t);
    {
        t.join();
    }

    void workload() {}

    int main()
    {
        thread tx { workload };

        func(std::move(tx));    // ok


        func(thread{workload});    // ok, R-val expression
    }
  ```

return value can be thread type thanks to move semantic
  _ex:_
  ```cpp
    std::thread make_thread();
    {
        thread t{[] { 
            std::cout << "func()\n";}};
        
        return t;    //RVO is not guaranteed (NRVO) but move semantic
    }

    void workload() {}

    int main()
    {
        thread t;

        t = make_thread();  // ok, object is moved

        t.join();
    }
  ```


  _ex:_
  ```cpp
    std::thread func(std::thread&& t);
    {
        auto tx = std::move(t);
    }

    int main()
    {
        std::thread tx{[]{}};
        
        func(std::move(tx))
    }
  ```

## jthread RAII sarmalamasi

  _ex:_
  ```cpp
    class JThread {
    public:
        JThread() noexcept = default;

        template<typename Func, typename ...Args>
        explicit JThread(Func&& f, Args&& ...args) : m_t(std::forward<Func>(f), std::forward<Args>(args)...);

        ~Jthread()
        {
            if(joinable())
                join();
        }

        explicit JThread(std::thread t) noexcept : m_t(std::move(t)) {}

        JThread(JThread&& other) noexcept : m_t(std::move(other.m_t)) {} 
        
        bool joinable() const noexcept { return m_t.joinable(); }
        
        void join() { m_t.join(); }

        void swap(JThread& other) noexcept
        {
            m_t.swap(other.m_t);
        }

        JThread& operator= JThread(JThread&& other)
        {
            if(joinable)
                join(); 

            m_t = std::move(other.m_t);

            return *this;
        }

    private:
        std::thread m_t; 
    };
  ```


## thread::id
  - karsilastirma operatoru var
  - inserter i var
  _ex:_
  ```cpp
    void func()
    {
        std::cout << std::this_thread::get_id() << "\n";    // 18356 when called from tx
        
        // returns the id of the thread that runs this function
    }

    int main()
    {
        thread tx{ func };

        auto id = tx.get_id();

        cout << typeid(id).name() << "\n";      // class std::thread::id
        cout << "thread id = " << id << "\n";   // ex: 18356

        func();                                 // ex: 24192, not the same with above
                                                //

        tx.join();
    }
  ```

### thread_id yi kullanarak, fonksiyon icinde behaviour secebiliriz 

  _ex:_
  ```cpp
    std::thread::id g_id;

    void func()
    {
        auto id = std::this_thread::get_id();
        if(id == g_id) {
            cout << "runs from the main thread\n";
        }
        else{
            cout << "runs from the child thread\n";
        }
    }

    int main()
    {
        g_id = this_thread::get_id();

        func();

        thread tx{ func };
        tx.join();
    }
  ```

### std::this_thread::sleep_for

  _ex:_
  ```cpp
    #include<thread>
    #include<iostream>
    #include<chrono>

    void func(int n)
    {
        std::this_thread::sleep_for(std::chrono::duration<double>{12.36}); // wait 12.26 seconds
    }
  ```

### std::this_thread::sleep_untill

  _ex:_
  ```cpp
    auto now() {
        return std::chrono::steady_clock::now();
    }

    auto awake_time()
    {
        using std::chrono::operator""ms;
        return now() + 2000ms;

        // timepoint +/- duration = timepoint
        // timepoint - timepoint = duration
    }

    int main()
    {
        std::cout << "Hello\n" << std::flush;
            // flush etme sebebi: girilen yazi ekrana ciksin, beklemesin

        const auto start{ now() };
        std::this_thread::sleep_until(awake_time());
        std::chrono::duration<double, std::milli> elapsed{ now() - start }

        std::cout << "Waited " << elapsed.count() << " ms\n";
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

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
