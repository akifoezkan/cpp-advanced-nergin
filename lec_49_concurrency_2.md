# Thread and exception handling

## thread nesnesinin dtor u cagirildiginda, thread joinable ise exception throw edilir.
Bunu engellemek icin "join" yada "detach" fonksiyonlari cagirilmali
  _ex:_
  ```cpp
    void foo()
    {
        std::cout << "foo called!\n";
    }

    void func()
    {
        std::cout << "func called!\n";
    }

    int main()
    {
        std::thread t{ foo };
        
        func();

        t.join();   // will not be executed
    }
  ```

For this reason, it makes sense to use thread class in a RAII block (JThread).
Otherwise, following could also be a solution, but ugly.

  _ex:_
  ```cpp
    void foo()
    {
        std::cout << "foo is called!\n";
    }

    void func()
    {
        std::cout << "func is called!\n";
    }

    int main()
    {
        std::thread t{ foo };
        
        try {
            func();
            t.join();   // will be executed
        }
        catch( const std::exception& ex {
            std::cout << ex.what() << "\n"
            t.join();
        }
    }
  ```
## Bir thread in icersinden gonderilen exception nasil halledilir?

think about hte following case:
exception sent from the thread "t" won't be caught in the following code.
therefore, terminate will be called from the try block.

  _ex:_
  ```cpp
    void func()
    {
        std::cout << "func is called!\n";

        if(1) {
            throw std::runtime_error{"error from func\n"};
        }
    }

    int main()
    {
        try {
            std::thread t{ func };
            t.join();

            // exception sent by func won't be catch from the next catch block
            // terminate will be called from here
        }
        catch( const std::exception& ex {
            std::cout << ex.what() << "\n"
        }

        t.join();
    }
  ```

Bir exception i yakalayip bir degiskende tutup, sonra bunu rethrow etmeliyiz
use
- exception_ptr
- current_exception: 
  saves the exception to exception_ptr
- rethrow_exception: 
  rethrows the exception pointed  by exception_ptr

  _ex:_
  ```cpp
    std::exception_ptr exptr = nullptr;

    void func(int x)
    {
        std::cout << "func is called!\n";

        try {
            if(x % 2 == 0)
                throw std::invalid_argument{"invalid argument"};
        }

        catch(...) {
            exptr = std::current_exception();   // save the exception
        }

        std::cout << "func is ended\n";
    }

    int main()
    {
        std::thread t{ func, 1 };
        t.join();

        try {
            if(exptr){
                std::rethrow_exception(exptr); // if exptr is no Null, there is an exception
            }
        }
        catch( const std::exception& ex {
            std::cout << ex.what() << "\n"
        }
    }
  ```
Of course, we do not have to rethrow the exception stored in an exception_ptr
  _ex:_
  ```cpp
    std::exception_ptr exptr{ nullptr };

    void func(int x)
    {
        std::cout << "func is called!\n";

        try {
            if(x % 2 == 0)
                throw std::invalid_argument{"invalid argument"};
        }

        catch(...) {
            exptr = std::current_exception();   // save the exception
        }

        std::cout << "func is ended\n";
    }

    int main()
    {
        std::thread t{ func, 1 };
        t.join();

        if(exptr){
            std::cout << "exception gonderilmis\n";
        }
        else {
            std::cout << "exception gonderilmemis\n";
        }
    }
  ```
  _ex:_
  ```cpp
    std::vector<std::exception_ptr> g_ex_vec;

    std::mutex g_mutex;

    void f1()
    {
        throw std::exception{ "exception from f1" };
    }

    void f2()
    {
        throw std::exception{ "exception from f1" };
    }


    void th_func1()
    {
        try {
            f1();
        }
        catch(...) {
            std::lock_guard<std::mutex> guard{ g_mutex };
            g_ex_vec.push_back(std::current_exception());
        }
    }

    // ???
    void th_func2()



    int main()
    {
        ???
    }
  ```

## osyncstream, thread cikislarini duzenler
ostream de su garanti yok:
  asagidaki kod ciktisinda isimler birbirinden karisik olacak.
  sirasiyla f1, f2 ve f3 ciktilari gorunmeyecek
  hatta bir thread in ciktisi tamamlanmadan digeri gelebilir
  _ex:_
  ```cpp
    void f1()
    {
        for(int i = 0; i < 10000; ++i)
            std::cout << "necatiergin" << haydar;

        // there is no warranty that necatiergin and haydar will be consequent in the output
    }

    void f2()
    {
        for(int i = 0; i < 10000; ++i)
            std::cout << "akifoezkan" << burak akgun;
    }

    void f3()
    {
        for(int i = 0; i < 10000; ++i)
            std::cout << "mehmet" << asdasdas;
    }

    int main()
    {
        std:thread t1{ f1 };
        std:thread t2{ f2 };
        std:thread t3{ f3 };

        t1.join();
        t2.join();
        t3.join();

        // there is no warranty that f1, f2, f3 outputs are consequent 
    }
  ```

c++20 bu sorunu "std::osyncstream" nesnesi kullanarak cozebiliriz

  _ex:_
  ```cpp
    int func()
    {
        using namespace std::literals;
        std::this_thread::sleep_for(2000ms);

        std::osyncstream os{cout};

        for(int i=0; i < 10000; ++i)
            os << 13 << " alican " << 34.7234234 << "mustafa\n";
    }

    int foo()
    {
        using namespace std::literals;
        std::this_thread::sleep_for(2000ms);
        
        for(int i=0; i < 10000; ++i)
            std::osyncstream{std::cout} << 13 << " necati " << 34.7234234 << "ergin\n";
    }

    int main()
    {
        std::thread t1{ func };
        std::thread t2{ foo  };

        t1.join();
        t2.join();
    }
  ```

## thread nesneleri bir containerda (array/vector) tutulabilir

  _ex:_
  ```cpp
    #include<syncstream>
    #include<iostream>
    #include<chrono>

    void cprint(char c)
    {
        using namespace std::literals;

        for(int i=0; i < 1000; ++i)
        {
            std::cout << c;
            std::this_thread::sleep_for(50ms);
        }
    }

    int main()
    {
        std::thread ta[10];

        for(int i{}; auto& t : ta)
        {
            t = std::thread{cprint, 'A' + i++};
        }

        for(auto& t: ta)
        {
            t.join();
        }

    }
  ```

  _ex:_
  ```cpp
        for(int i{}; auto& t : ta)
        {
            tvec.push_back(std::thread{cprint, 'A' + i++} );    // move
        }

        for(int i{}; auto& t : ta)
        {
            t  = std::thread{cprint, 'A' + i++};
            // tvec.push_back(t);               // syntax error, copy
            tvec.push_back(std::move(t));       // ok, move semantic 
        }
    }
  ```

note that static code analysis tools sometimes show incorrect results.
For instance, following code is fine but most of the tools show error
  _ex:_
  ```cpp
    int main()
    {
        ifstream ifs{"async_01.txt"};
        if(!ifs)
        {
            cerr << "cannot be opened!\n";
            return 1;
        }

        vector<string> svecl
        string sline;

        while(get_line(ifs, sline))
        {
            // svec.push_back(sline);           // copy, sline is lval
            
            svec.push_back(std::move(sline));   // move 
        }
    }
  ```

## this_thread::get_id, this_thread::yield

  _ex:_
  ```cpp
    std::thread::id main_thread_id;

    void func()
    {
        if(this_thread::get_id() == main_thread_id)
            cout << "called from main\n";
        else
            cout << "called from the other thread";
    }

    int main()
    {
        main_thread_id = this_thread::get_id();

        func();

        thread t{func};
        t.join();
    }
  ```

## this_thread::yield
  - yield edilen thread, bu function cagirildigi surece oncelik siralamasinda arkaya alinir
  - sleep bloke eder, yield onceligi digerlerine verir

  _ex:_
  ```cpp
    example for this_thread::yield ???
  ```

  _ex:_
  ```cpp
    another example

    std::atomic
  ```

## thread_local
  multithread memory model eklenmesiyle, storage duration kategorilerine yenisi eklendi
  - automatic storage duration
  - static storage duration
  - dynamic storage 

  - new: thread-local storage duration
  her thread icin ayri static omurlu nesne ihtiyaci var.
  ancak threadin omru bitince bu static omurlu nesne nin omru de biter


  _ex:_
  ```cpp
    thread_local int tval{ 0 };   // her thread de bundan 1 tane var

    void func(const std::string& thread_name)
    {
        ++tval;     // senkronizasyon gerekmez
        std::osyncstream{ std::cout } << "tval in thread " << thread_name << " is " << tval << "\n";
    }

    int main()
    {
        std::thread tx(func, "a");
        std::thread ty(func, "b");

        {
            std::osyncstream << "tval in main" << tval << ?? 
        }

        tx.join();
        ty.join();
    }

    sirasi belli degil ama 
    main 0
    a 1
    b 1 
  ```

  _ex:_
  ```cpp
    ??
    thread_local int ival = 0;

    void thread_func(int* p)
    {
        *p = 43;
        std::cout << "ival = " << ival >> "\n";
    }

    int main()
    {
        // ...
        // ??
    }
  ```

  _ex:_
  ```cpp
    another example thread_local

  ```

  21.45

  _ex:_
  ```cpp
    thread_local int gt{};

    void func(char c)
    {
        ++gt;
        std::osyncstream{ std::cout } << c << " " << gt;
    }

    int main()
    {
        using namespace std;

        vector<thread> tvec;
        for(char c = 'a'; c <= 'z'; ++c)
        {
            tvec.emplace_back(func, c);
        }

        for(auto& t: tvec)
        {
            t.join();
        }
    }


    output
    a 1
    c 1
    b 1
    g 1
    d 1
    ...

    cunku her thread in kendi degiskeni var
  ```

  _ex:_
  ```cpp
    int gt{};

    std::mutex m;

    void func(char c)
    {
        std::lock_guard<std::mutex> myguard{ m };
        ++gt;
        std::osyncstream{ std::cout } << c << " " << gt;
    }

    int main()
    {
        using namespace std;

        vector<thread> tvec;
        for(char c = 'a'; c <= 'z'; ++c)
        {
            tvec.emplace_back(func, c);
        }

        for(auto& t: tvec)
        {
            t.join();
        }
    }


    output
    a 1
    c 2
    b 3
    g 4
    d 5
    ..
    g 26

    cunku butun thread ler ayni global degiskeni kullanir
  ```

## paralel programlama ile kodun hizlandirilmasi (farkli core lar)
  _ex:_
  ```cpp
    // seknronizasyona gerek yok cunku thread ler farkli global degiskeni degistiriyor

    constexpr uint64_t n = 1'000'000;

    uint64_t sum_odd = 0ull;
    uint64_t sum_even = 0ull;

    get_sum_odds()
    {
        for(uint64_t i = 1; i < n; i +=2)
        {
            std::this_thread::sleep_for(2ms);
            sum_odd += i;
        }
    }

    get_sum_evens()
    {
        using namespace std::literals;
        for(uint64_t i = 0; i < n; i +=2)
        {
            std::this_thread::sleep_for(2ms);
            sum_even += i;
        }
    }

    int main()
    {
        auto start = steady_clock::now();
        get_sum_evens();
        get_sum_odds();
        auto start = steady_clock::now();

        std::cout << " total time: " << duration_cast<milliseconds>(end - start).count() << "\n";
        std::cout << "odds time " << sum_odd << "\n";
        std::cout << "evens time" << sum_even << "\n";
    }

    // time:spent = 
  ```

  _ex:_
  ```cpp
    constexpr uint64_t n = 1'000'000;

    uint64_t sum_odd = 0ull;
    uint64_t sum_even = 0ull;

    get_sum_odds()
    {
        for(uint64_t i = 1; i < n; i +=2)
        {
            std::this_thread::sleep_for(2ms);
            sum_odd += i;
        }
    }

    get_sum_evens()
    {
        using namespace std::literals;
        for(uint64_t i = 0; i < n; i +=2)
        {
            std::this_thread::sleep_for(2ms);
            sum_even += i;
        }
    }

    int main()
    {
        using namespace std::chrono;

        std::cout << std::thread::hardware_concurrency() << "\n";

        auto start = steady_clock::now();
        std::thread t1{ get_sum_evens() };
        std::thread t2{ get_sum_odds() };

        t1.join();
        t2.join();
        auto start = steady_clock::now();

        std::cout << " total time: " << duration_cast<milliseconds>(end - start).count() << "\n";
        std::cout << "odds time " << sum_odd << "\n";
        std::cout << "evens time" << sum_even << "\n";
    }

    // time:spent = 
  ```

## .native_handle()
    isletim sisteminin kendi API leri ile kullanmak icin bir address verir

  _ex:_
  ```cpp
    std::thread t{get_sum_evens};

    std::cout << t.native_handle() << "\n";     // ex: 0x0000e4

    t.join();
  ```

# async
 - high-level interface: olabilecek en ust seviye
 - amac sadece birden fazla thread i paralel calistirip bir outcome elde edilecekse, high level solution sunar
 - thread lerin paralel calistirilmasini ve bu threadlerin geri donus degerlerini programa iletir

 calistirilan thread -> calistiran thread: shared state bir variable gerekir (ex future type)
 async kullanildiginda buna gerek yok

  ```cpp
    template<...>
    future<...> async(Function&& f, Args&&... args);
  ```
  ```cpp
    template<...>
    future<...> async(launch_policy, Function&& f, Args&&... args);

    launch_policies 
        => std::launch::async     // enable asynchronous evaluation
        => std::launch::deferred  // enable lazy evaluation
    default: std::launch::async | std::launch::deferred
  ```

  _ex:_
  ```cpp
    int foo(int x)
    {
        std::cout << "foo cagirildi x = " << x << "\n";

        return x * x;
    }

    int main()
    {
        sd::thread t{foo, 10};
        t.join();


        // I cannot get the return value and catch exception 
    }
  ```

## using async:

  _ex:_
  ```cpp
    int foo(int x)
    {
        std::cout << "foo cagirildi x = " << x << "\n";

        return x * x;
    }

    int main()
    {
        std::future<int> ft1 = std::async(foo, 10);
        auto ft2 = std::async(foo, 10);
        

        
        // std::async(std::launch::async, foo, 10);
        // std::async(std::launch::deferred, foo, 10);
        // std::async(td::launch::async | std::launch::deferred, foo, 10); // default for the other overload

    }
  ```
## .get function 
- if the result is calculated we get the result
- if not, execution is blocked until the result is calculated
  _ex:_
  ```cpp
    int foo(int x)
    {
        std::cout << "foo cagirildi x = " << x << "\n";

        return x * x;
    }

    class Myclass {};
    Myclass bar();

    int main()
    {
        auto ft = std::async(foo, 5);
        
        auto val = ft.get();

        std::cout << "val = " << val << "\n";
    }
  ```

std::launch::async parametre ile cagirildiginda, sistemin async execution icin
resource u olmamasi durumunda hata verir.

  _ex:_
  ```cpp
    int f1()
    {
        // ...
        return 1;
    }
    
    int f2()
    {
        // ...
        return 2;
    }
    
    int f3()
    {
        // ...
        return 3;
    }

    int main()
    {
        auto ft1 = std::async(std::launch::asynch, f1);
        auto ft2 = std::async(std::launch::asynch, f2);
        auto ft3 = std::async(std::launch::asynch, f3);

        cout << ft1.get() << "\n";
        cout << ft2.get() << "\n";
        cout << ft3.get() << "\n";
    }
  ```

##

  _ex:_
  ```cpp
    int func()
    {
        throw( ???)
        return 1;
    }

    int main()
    {
        try{
            std::future<int> ftr = async(std::launc::async, func);
            auto val = ftr.get();
        }
        catch(const std::exception& ex)
        {
            std::cout << ex.what() << "\n";
            func in icindeki exception i yakalar!!!
        }
    }
  ```

  _ex:_
  ```cpp
    int func();
    int foo();

    int main()
    {
        auto x = func() + foo();    // works sequential, order is unspecified
    }
  ```

  _ex:_
  ```cpp
    int func();
    int foo();

    int main()
    {
        auto x = func() + foo();    // works sequential, order is unspecified

        auto ft_foo = async(std::launch::async, foo);
        auto func_val = func();

        auto x = func_val + ft_foo.get();   // works parallel
    }
  ```

  _ex:_
  ```cpp
   get_str_letters(int n)
   
   get_str_digits(int n)

   // ??? ...

   int main()
   {
        auto start = std::chrono::steady_clock::now();

        auto ftr1 = async(std::launch::deferred, get_str_letters, 20);
        auto ftr2 = async(std::launch::deferred, get_str_digits, 20);

        //auto ftr1 = async(std::launch::async, get_str_letters, 20);
        //auto ftr2 = async(std::launch::async, get_str_digits, 20);

        auto s1 = ftr1.get();
        auto s2 = ftr2.get();
        auto end= std::chrono::steady_clock::now();

        std::cout << s1 + s2 << "\n";
   }

   // deferred: 6.01385
   // async: 3.00679
  ```

> fonksiyonun geri donus olmasa da bir future nesnesinde tutulmali
> yoksa tanimlanmayan obje destroy edilir, bu sirada .get cagirilarak program bloke edilir

  _ex:_
  ```cpp

  int main()
  {
        async(std::launch::async, get_str_letters, 20); 
            // future nesnesi tanimlanmamis.
            // tanimlanmayan future nesnesinin hayatinin bitmesi beklenir
            // bu durumda .get fonk cagirilir
        async(std::launch::async, get_str_letters, 20);
            // 2. kere .get fonk cagirilir

        // therefore the above thread are ran consequetively
  }
  ```

  _ex:_
  ```cpp
  int main()
  {
        auto f1 = async(std::launch::async, get_str_letters, 20); 
        auto f2 = async(std::launch::async, get_str_letters, 20);

        // f1 ve f2 program biterken destroy edilir.
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
