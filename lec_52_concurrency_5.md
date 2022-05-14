#
 - ozellikle birden fazla threadin ayni variable i init. etmesi

## once_flag + call_once??
  - call once, a ilk gelen thread init eder
  -

  _ex:_
  ```cpp
    #include <iostream>
    #include <thread>
    #include <mutex> 

    std::once_flag gflag;

    void func(int id)
    {
        using namespace std::literals;

        std::this_thread::sleep_for(500ms);
        std::call_once(gflag, [id](){
            std::cout << id << "threadi icin cagirildi";

            // only the first flag will call this lambda function
        });

        // ...
    }

    int main()
    {
        std::vector<std::thread> tvec;

        for(int i = 0; i < 10; ++i) {
            tvec.emplace_back(func, i);
        }

        for(auto& th : tvec)
            th.join();
    }
  ```

  _ex:_
  ```cpp
    #include <iostream>
    #include <thread>
    #include <mutex> 

    std::once_flag gflag;

    // !!!
    // asagidaki foo ve bar fonksiyonlari ayni gflag i kullandigi icin
    // ya bar, yada foo will be registered

    void foo(int id)
    {
        using namespace std::literals;

        std::this_thread::sleep_for(500ms);
        std::call_once(gflag, [id](){
            std::cout << id << "registered in foo\n";

            // only the first flag will call this lambda function
        });

        // ...
    }

    void bar(int id)
    {
        using namespace std::literals;

        std::this_thread::sleep_for(500ms);
        std::call_once(gflag, [id](){
            std::cout << id << "registered in bar\n";

            // only the first flag will call this lambda function
        });

        // ...
    }

    int main()
    {
        std::thread a[10];

        for(int i = 0; i < 10; ++i) {
            a[i] = i % 2 == 0 ? std::thread{foo} : std::thread{bar};
        }

        for(auto& th : a)
            th.join();
    }
  ```

tavsiye olarak yazilacak bir singleton degil ancak call_once icin iyi bir ornek:
  _ex:_
  ```cpp
    #include <mutex>
    #include <vector>
    #include <stringstream>
    #include <fstream>

    class Singleton {
    public:
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;

        static Singleton* get_instance()
        {
            // if(m_instance)
            //     m_instance = new Singleton;

            std::call_once(m_init_flag, init);
            return m_instance;
        }

        static void init()
        {
            m_instance = new Singleton;
        }

    private:
        inline static std::once_flag _init_flag; 
        inline static Singleton* m_instance{};
        Singleton() = default;
    };

    void func()
    {
        static std::ofstream ofs{"out.txt"};


        //std::osyncstream{ofs} << Singleton::get_instance() << "\n";
    }

    int main()
    {
        std::vector<std::thread> tvec;

        for(int i = 0; i < 100; ++i)
        {
            tvec.emplace_back(func);
        }

        for(auto& th : tvec)
            th.join();
    }
  ```

# Conditional variable
- future: biri bir deger uretir, digeri onu tuketir
- _condition variable_: belirli bir koşul _(condition)_ gerçekleşinceye kadar bir _thread_'in bloke olarak beklemesini sağlayan bir senkronizasyon yapısıdır.


## naive approach:
producer olan thread isini bitirdiginde boolean bir degiskeni set eder. consumer thread bu degiskeni observe eder.

  _ex:_
  ```cpp
    #incluce <mutex>

    int shared_variable{};
    std::mutex mtx;

    void producer()
    {
        using namespace std::literals;
        std::this_thread::sleep_for(1000ms);
        std::lock_guard lk{ mtx };

        // production
        shared_variable = 999;
    }

    void consumer_naiveset()
    {
        std::unique_lock ulock {mtx};

        while(shared_variable == 0)
        {
            ulock.unlock();
            ulock.lock();
        }

        // consumption

        std:cout << "the value is : " << shared_variable << "\n";
    }

    void consumer_naive()
    {
        std::unique_lock ulock {mtx};

        while(shared_variable == 0)
        {
            ulock.unlock();
            std::this_thread::yield();
            std::this_thread::sleep_for(1000ms);
            ulock.lock();
        }

        // consumption

        std:cout << "the value is : " << shared_variable << "\n";
    }

    int main()
    {
        std::thread t1{producer};
        std::thread t2{consumer};

        t1.join();
        t2.join();
    }
  ```

concurrency in action:
  section 4.1 Waiting for an event or another condition
  an anology with train

## usage of condition variable
source: https://github.com/necatiergin/cpp-kursu-kodlar/blob/main/concurrency/condition_variable/notlar.md
  - _condition variable_ bir başka _thread_'in tetikleyeceği bir _event_'i beklemenin temel mekanizmasıdır.
  - Birden fazla _thread_'in kullanıldığı programlarda çoğu zaman bir _thread_'in belirli bir _event_ oluşuncaya kadar başka bir _thread_'i beklemesi gerekir (giriş çıkış işleminin tamamlanması ya da bir verinin hazırlanması gibi)
  - Bir _thread_'in bir _event_'in gerçekleşip gerçekleşmediğini sürekli olarak sorgulaması, işlemci zamanının boşa harcanmasına, dolayısıyla verimin düşmesine neden olabilir.
  - Böyle durumlarda bekleyen _thread_'in bloke olması ve ilgili _event_ gerçekleştikten sonra tekrar çalışmaya başlaması genellikle daha iyidir.
  - Bir _condition variable_, belirli bir koşul _(condition)_ gerçekleşinceye kadar bir _thread_'in bloke olarak beklemesini sağlayan bir senkronizasyon yapısıdır.
  + Bir _condition variable_ bir _event_ ile ilişkilendirilir.
  + Bir _event_'i beklemek isteyen bir _thread_ _condition variable_'ın _wait_ fonksiyonlarından birini çağırır. _(wait, wait\_for, wait\_until)_
  + Bir _thread_ _(producer)_ bir _event_'in gerçekleşmiş olduğunu bir ya da birden fazla _thread_'e _(consumers)_ bildirmek için _condition variable_'ın _notify_one_ ya da _notify_all_ fonksiyonlarından birini çağırır.
  + Bildirimi alan _thread_ çalışmaya devam ettiğinde istenen koşulun sağlanmış olma garantisi yoktur.
Başka bir _thread_ koşulun değişmesini sağlamış olabilir ya da _"spurious wakeup"_ denilen durum oluşmuş olabilir. (Bekleyen bir _thread_'in aslında diğer taraftan bir bildirim almadan uyanmasına "spurious wake" denir.)

> spurious wakeup: tamamen random ve ongorulemez
> dolayisiyla uyarilan thread suna dikkat etmeli: task gercekten tamamlandi mi, yoksa spurious wakeup mi var?
> condition, bir shared_variable. Producer isini bitirdiginde bu degiskeni set eder.

Bu nedenle uyanan _thread_'in koşulun sağlanmış olup olmadığını tekrar kontrol etmesi gerekir.
  - _condition_variable_ olarak _std::condition_variable_ sınıfı türünden bir nesne kullanılır.
  - _condition_variable_ sınıfı _\<condition_variable\>_ başlık dosyasında tanımlanmıştır. _std::condition_variable_ nesneleri kopyalanamaz ve taşınamaz _(not copyable - not moveable)_.
  - sınıfın _wait, wait_for_ ya da _wait_until_ üye fonksiyonları ile bekleyecek _thread_ bloke edilir koşulun sağlanması beklenir.
  - sınıfın _notify_one_ ve _notify_all_ üye fonksiyonları ile bekleyen _thread_'lere koşulun oluştuğu bildirilir _(signal)_.
  - uyanan _thread_'in koşulu tekrar sınaması gerekir, çünkü
    - _spurious wakeup_ oluşabilir.
    - sinyalin alınması ve _mutex_'in edinilmesi zaman aralığı içinde başka bir _thread_ koşulu değiştirmiş olabilir.
  - _wait, wait_for_ ve _wait_until_ fonksiyonları _mutex_'i atomik olarak edinirler ve ilgili _thread_'i bloke ederler.
  - _notify_one_ ve _notify_all_ fonksiyonları atomiktir.

Tipik işlem akışı şöyle gerçekleştirilir:
- Tipik olarak _std::lock_guard_ kullanarak bir _mutex_'i edinir. <br>
- Kilit edinilmiş durumdayken paylaşılan değişkeni değiştirir. Yapılan değişikliğin bekleyen _thread_(ler)e doğru bir şekilde bildirilebilmesi için, paylaşılan değişken atomik olsa dahi değişikliğin kilit edinilmiş durumda yapılması gerekir.<br>
- Bu amaçla tanımlanmış olan _std::condition_variable_ nesnesinin _notify_one_ ya da _notify_all_ fonksiyonlarından birini çağırır. Bu fonksiyonlar çağrıldığında kilitin edinilmiş durumda olması gerekmez. Eğer bu fonksiyonlar kilit edinilmiş durumda çağrılırsa bildirim alan _thread_'ler kilidi edinemezler ve tekrar bloke olurlar.
<br>

Bekleyen bir _thread_, önce _std::unique\_lock_ kullanarak (aynı) _mutex_'i edinir. Daha sonra aşağıdaki iki seçenekten birini uygular:<br>
- Birinci seçenek
  - Değişikliği zaten yapılmış ve bildirimin de gerçekleşmiş olabileceği ihtimaline karşı önce koşulu test eder.
  - wait, wait\_for, ya da wait\_until fonksiyonlarından birini çağırır. Çağrılan _wait_ fonksiyonu edinilmiş _mutex_'i otomatik olarak serbest bırakır ve _thread_'in çalışmasını durdurur.
  - _condition\_variable_ nesnesinin _notify_ fonksiyonu çağrıldığında (ya da bekleme süresi dolduğunda) ya da bir _"spurious wakeup"_ oluştuğunda, _thread_ uyanır ve _mutex_ yeniden edinilir.
 - Uyanan ve kilidi edinen _thread_'in koşulun gerçekleşip gerçekleşmediğini kontrol etmesi ve eğer bir _spurious wakeup_ söz konusu ise tekrar bekleme durumuna geçmesi gerekir.

 > ancak asagidaki ikinci secenek, yukaridaki birinci secenege gore daha kullanisli
 > asagidaki gibi sadece predicate isteyen overload cagirilir

- İkinci seçenek olarak bekleyen _thread_
  - bu işlemlerin hepsini sarmalayan _wait_ fonksiyonlarının bir _predicate_ alan _overload_'larından birini çağırır.

_std::condition\_variable_ sınıfı yalnızca _std::unique\_lock<std::mutex>_ ile kullanılabilir. Bu şekilde kullanım zorunluluğu bazı platformlarda en yüksek verimle çalışmasını sağlar. _std::condition\_variable_any_ sınıfı ise _BasicLockable_ niteliğini sağlayan herhangi bir nesneyle _(örneğin std::shared\_lock)_ çalışabilmesini sağlar.<br>
_std::condition\_variable_ sınıfının _wait, wait\_for, wait\_until, notify_one ve notify\_all_ üye fonksiyonları birden fazla _thread_ tarafından eş zamanlı çağrılabilir.

_std::condition_variable_ sınıfının _wait_ üye fonksiyonu, bloke olmadan beklemeye _(busy wait)_ karşı bir optimizasyon olarak görülebilir. _wait_ fonksiyonunu (ideal olmasa da) gerçekleştirimi şöyle olabilir:

```
template<typename Pred>
void wait(std::unique_lock<std::mutex>& lk, Predicate pred)
{
	while(!pred()) {
		lk.unlock();
		lk.lock();
	}
}
```

> gercekte yukaridakinden cok daha komplike ve efficient implement edilmistir

kullanimi:

  _ex:_
  ```cpp
    #include <mutex>
    #include <condition_variable>

    bool flag{};
        // producer isini tamamladiginda flag i set edecek

    std::mutex mtx;
    std::conditional_variable cv;

    void producer()
    {
        std::cout << "producer is producing data\n";

        {
            lock_guard lock{ mtx };
            data = 78754;
            ready_flag = true;
        }
        cv.notify_one();
    }

    void consumer()
    {
        {
            std::unique_lock ulock{ mtx };
            cv.wait(ulock, []{ return ready_flag; });
                // wait uyandiginda: 
                // predicate, yani ready_flag, false ise bunun spurious wakeup oldugu otomatik olarak anlasilacak ve beklemeye devam edilecek
        }

        // at this point, I can continue my work
        // we are sure that producer has finished its task
    }
  ```

  _ex:_
  ```cpp
    #include <thread>
    #include <mutex>
    //...

    class IStack{
        
        int pop()
        {
            std_unique_lock lock(m_);  // gets mutex lock
            m_cv.wait(lock, [this]() { return !m_vec.empty(); });   

            // after point, condition is met for sure
            // we still have the mutex lock

            int val = m_vec.back();
            m_vec.pop_back();

            // advantages of unique_lock
            // 1.
            // 2. 

            return val;
        }

        void push(int x)
        {
            {
                std::scoped_lock lock(m_);
                mvec.push_back(x);
            }

            // finished its task and notifies the consumer

            m_cv.notify_one();
        }
    private:
        mutable std::mutex m;
    };

    void producer()
    {
        for(int i = 0; i < n; ++i)
            s.push(2 * i + 1);

    }

    void consumer()
    {
        for(int i = 0; i < n; ++i)
            std::cout << s.pop() << "\n";

    }

    ??
  ```

  _ex:_
  ```cpp
    // https://en.cppreference.com/w/cpp/thread/condition_variable

    #include <iostream>
    #include <string>
    #include <thread>
    #include <mutex>
    #include <condition_variable>
     
    std::mutex m;
    std::condition_variable cv;
    std::string data;
    bool ready = false;         // condition-1. producer will set
    bool processed = false;     // condition-2, consumer will set
     
    void worker_thread()
    {
        // Wait until main() sends data
        std::unique_lock lk(m);
        cv.wait(lk, []{return ready;});
     
        // after the wait, we own the lock.
        std::cout << "Worker thread is processing data\n";
        data += " after processing";
     
        // Send data back to main()
        processed = true;
        std::cout << "Worker thread signals data processing completed\n";
     
        // Manual unlocking is done before notifying, to avoid waking up
        // the waiting thread only to block again (see notify_one for details)
        lk.unlock();
        cv.notify_one();
    }
     
    int main()
    {
        std::thread worker(worker_thread);
     
        data = "Example data";
        // send data to the worker thread
        {
            std::lock_guard lk(m);
            ready = true;
            std::cout << "main() signals data ready for processing\n";
        }
        cv.notify_one();
     
        // wait for the worker
        {
            std::unique_lock lk(m);
            cv.wait(lk, []{return processed;});
        }
        std::cout << "Back in main(), data = " << data << '\n';
     
        worker.join();
    }
  ```

  _ex:_
  ```cpp
    // https://en.cppreference.com/w/cpp/thread/condition_variable/notify_all

    #include <iostream>
    #include <condition_variable>
    #include <thread>
    #include <chrono>
     
    std::condition_variable cv;
    std::mutex cv_m; // This mutex is used for three purposes:
                     // 1) to synchronize accesses to i
                     // 2) to synchronize accesses to std::cerr
                     // 3) for the condition variable cv
                     // note that cerr doesn't need to be flashed
    int i = 0;
     
    void waits()
    {
        std::unique_lock<std::mutex> lk(cv_m);
        std::cerr << "Waiting... \n";
        cv.wait(lk, []{return i == 1;});
        std::cerr << "...finished waiting. i == 1\n";
    }
     
    void signals()
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        {
            std::lock_guard<std::mutex> lk(cv_m);
            std::cerr << "Notifying...\n";
        }
        cv.notify_all();
     
        std::this_thread::sleep_for(std::chrono::seconds(1));
     
        {
            std::lock_guard<std::mutex> lk(cv_m);
            i = 1;
            std::cerr << "Notifying again...\n";
        }
        cv.notify_all();
    }
     
    int main()
    {
        std::thread t1(waits), t2(waits), t3(waits), t4(signals);
        t1.join(); 
        t2.join(); 
        t3.join();
        t4.join();
    }
  ```

# sequential consistency
  - intra thread sequence of read and write is always the same (atomimc)
  - threadler de yapilan islemler diger threadler tarafindan hemen gorulebilir
  - derleyicinin yapacagi optimizasyonlari cok kisitlar 

  - birden fazla thread calistiginda, data memory e aktarilir. Bu da execution i yavaslatir


  add the picture 1- siralama ?? 
  add the picture 2- siralama ?? 

## happens-before relationship:
   A ve B ayni thread de yada farkli thread de yapilan islemler olsun
   
   - A ve B are in the same thread and A is sequenced before B ise: A, B den once yapilmak zorunda


   add picture


   next lessson: atomicity, order, etc


  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
