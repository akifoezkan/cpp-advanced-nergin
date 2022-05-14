# async examples:
...

# future, promise

promise(sender) -> [set] -> channel -> [get] -> future(receiver)

set:
- value
- exception


- promise ve future ayri threadler tarafindan kullanilabilir
- shared state (channel) sadece bir defaya mahsus kullanilabilir
 birden fazla kez kullanmak icin shared state kullanilmali??
- promise object is not copyable but moveable 

  _ex:_
  ```cpp
    #include <future>

    int main()
    {
        std::promise<int> prom;

        std::future<int> ftr = prom.get_future();

        if(ftr.valid())
            cout << "future object is valid\n";
        else
            cout << "future object is not valid\n";

        // ftr is valid

        prom.set_value(991);
        auto val = ftr.get();

        cout << "val = " << val << "\n";
        
        // ftr is invalid
        // this communication channel is one -time

        try {
            val = ftr.get();
        }
        catch(const std::future_error& err) {
            cout << err;
        }

        // exception is throwed
    }
  ```

> wait_for, wait_until  ??

> herhangi bir sekilde async() isimizi gorurse, onu kullanalim


  _ex:_
  ```cpp
    void sum_square(std::promise<int>&& prom, ...)
    {
        prom.set_value(x * x + y * y)
    }

    struct Div {
        void operator() (std::promise<int>&& prom, int x, int y)
    };

    int main()
    {
        int x{90}, y{15};

        
    }
  ```

  _ex:_
  ```cpp
    std::promise<int> foo(int x, int y)
    {
        std::promise<int> prom;

        prom.set_value(x * y);

        return prom;
    }
  ```

  _ex:_
  ```cpp
    struct Div {
        void operator()(std::promise<int>&& prom, ...) const
        {
            try{
                if(b == 0) {
                    throw std::runtime_error(err_str);
                }

                prom.set_value(a / b);
            }
            catch(...) {
                prom.set_exception(std::current_exception());
            }
        }
    };

    void func(int x, int y)
    {
        std::promise<int> prom;
        std::future<int> ftr = pro,.get_future();
        std::thread th(Div{}. std::move(prom), x, y);

        try{
            cout << x << " / " << y << " = " >> ftr.get() << '\n';
        }
        catch(const std::exception& ex) {
            cout << ex,what();
        }

        th.join();
    }

    ???
  ```

# std::future_error
- bir std::promise nesnesini 2.kez set edersek std::future_error sinifi turunden hata gonderir
- future<> objesine 2.kez get islemi yapmak

  _ex:_
  ```cpp
    // bir std::promise nesnesini 2.kez set edersek std::future_error
    // sinifi turunden hata gonderir

    #include <future>
    #include <iostream>

    int main()
    {
        std::promise<int> prom;
        prom.set_value(10);

        try{
            prom.set_value(19);
        }
        //catch(const std::exception& ex)
        //catch(const std::logic_error& ex)
        catch(const std::future_error& ex)
        {
            cout << ex.what() << "\n";
        }
    }
  ```


  _ex:_
  ```cpp
   ....

   auto val = ft.get();
  ```

  _ex:_
  ```cpp
    int  foo()
     ...
  ```

## shared_future
  - promise in set ettigi shared future, get() fonk birden fazla cagirilabilir
  - ayni kanal uzerinden farkli veri paylasma isine yaramaz
  - copyable

  _ex:_
  ```cpp
    struct SumSquare {
        void operator()(std::promise<int>&& prom, int a , int b) const
        {
            prom.set_value(a*a + b*b);
        }
    };

    void func(std::shared_future<int> sftr)
    {
        // copy syntax
        // birden fazla thread bu fonk kullanacagi icin senkronizasyon gerekir

        std::osyncstream os{std::cout};

        os << "threadId(" << this_thread::get_id() << "): ";
        os << "result = " << sftr".get() << std::endl;  // flush is necessary
    }


    int main() {
        std::promise<int> prom;
        std::shared_future<int> sftr = prom.get_future();

        std::thread thr(SumSquare{}, std::move(prom), 5, 9);

        std::thread t1(func, sftr);
        std::thread t2(func, sftr);
        std::thread t3(func, sftr);
        std::thread t4(func, sftr);
        std::thread t5(func, sftr);

        th.join();
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
    }
  ```
# future<type>.share()
  - shared_future<type> return eder
  - future data becomes invalid
  _ex:_
  ```cpp
    #include <future>
    #include <thread>
    #include <iostream>
    #include <sstream>
    #include <set>
    #include <syncstream>
    
    struct SumSquare {
        void operator()(std::promise<int>&& prom, int a , int b) const
        {
            prom.set_value(a*a + b*b);
        }
    };

    void func(std::shared_future<int> sftr)
    {
        // copy syntax
        // birden fazla thread bu fonk kullanacagi icin senkronizasyon gerekir

        std::osyncstream os{std::cout};

        os << "threadId(" << this_thread::get_id() << "): ";
        os << "result = " << sftr".get() << std::endl;  // flush is necessary
    }


    int main() {
        std::promise<int> prom;
        std::future<int> ftr = prom.get_future();

        std::cout << "ftr is " << (ftr.valid() ? "valid" : "invalid") << '\n';
        // valid

        std::thread thr(SumSquare{}, std::move(prom), 5, 9);
        std::cout << "ftr is " << (ftr.valid() ? "valid" : "invalid") << '\n';
        // valid

        std::shared_future<int> s_ftr = ftr.share();
        std::cout << "ftr is " << (ftr.valid() ? "valid" : "invalid") << '\n';
        // invalid

        std::thread t1(func, sftr);
        std::thread t2(func, sftr);
        std::thread t3(func, sftr);
        std::thread t4(func, sftr);
        std::thread t5(func, sftr);

        th.join();
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
    }
  ```

## wait_for
  - future nesnesini belirli bir sure bekletir
  - returns an enum type:
    *
    * std::future_status::timeout
    * std::future_status::ready

  _ex:_
  ```cpp
    using namespace std::literals;

    void func(std::promise<int> iprom)  // move semantic
    {
        std::this_thread::sleep_for(5s);
        iprom.set_value(20)
    }

    using halfsec = std::chrono::duration<double, std::ratio<1, 2>>;

    int main()
    {
        std::promise<int> prom;
        auto ft = prom.get_future();
        std::thread th(func, std::move(prom));

        std::future_status status{};

        do{
            status = ft.wait_for(halfsec{12,5});
            //status = ft.wait_for(200ms);
            std::cout << "doing some worh here\n" << std::flust;

            // wait_for doesn't stall the program, just returns its status
        } while(status != std::future_status::ready);


        th.join();
    } 
  ```

  _ex:_
  ```cpp
    constexpr int x = 50;

    long long fib(int n)
    {
        return 
    }

    int main()
    {
        using namespace std::literals;

        auto ftr = std::async(fib, x);

        std::cout << "Result will come, wait for it:\n"
        while(ftr.wait_for(10ms) == std::future_status::timeout)
            cout << '.' << std::flush;

        result = ftr.get(); 

        ...
    }
  ```

## std::packaged_task
  - wrapper for a calleble for an async call
  - std::function da ayni isi yapar ancak packaged_task farkli bir thread de calistirilabilir
  - moveable, but not copyable

> https://github.com/necatiergin/cpp-kursu-kodlar/tree/main/concurrency/packaged_task

  _ex:_
  ```cpp
    std::packaged_task<int(int, int)> ptask([](int a, int b) { return a * a + b * b; });

    std::future<int> ftr = ptask.get_future();

    ftr(10, 20);
    
    ftr.get();
  ```

  _ex:_
  ```cpp
    #include <future>

    int foo(int x, int y)
    {
        return x * y + 5;
    }

    int main()
    {
        //std::packaged_task<int(int, int)> ptask{ foo };
        
        std::packaged_task ptask{ foo };    // CTAD, C++17
        
        auto ftr = ptask.get_future();

        ptask(12, 45);
    }
  ```

  _ex:_
  ```cpp
    // ptask_01.cpp

    #include <future>
    #include <iostream>
    
    int main()
    {
    	std::packaged_task<double(double, double)> ptask([](double a, double b)
    		{
    			return std::pow(a, b) + std::pow(b, a);
    		});
    
    	//std::future<double> result = ptask.get_future();
    	auto result = ptask.get_future();
    
    	ptask(1.2, 3.4);
    
    	std::cout << "result : " << result.get() << '\n';
    }
  ```


> packaged_task is a callable, therefore can be given to std::thread

  _ex:_
  ```cpp
    // moveable, but not copyable

    #include <iostream>
    #include <utility>
    #include <future>
    #include <thread>
    
    int main()
    {
    	using ftype = int(int, int);
    	std::packaged_task<ftype> pt_x;
    	std::packaged_task<ftype> pt_y([](int x, int y) {return x * x + y * y; });

    	pt_x = std::move(pt_y);

    	std::future<int> ret = pt_x.get_future();
    	std::thread(std::move(pt_x), 5, 15).detach();
    	auto result = ret.get();

    	std::cout << "the result is " << result << "\n";
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
