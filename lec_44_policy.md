-> ilk 5 dk yi kacirdim

# policy based design cont
  _ex:_
  ```cpp
    
    template<typenmae T>
    struct DefaultDelete
    {
        void operator()(T* p) const
        {
            delete p;
        }
    }
   
    template<typenmae T, typenmae DelPolicy = DefaultDelete<T>>
    class ScopedPtr : private DelPolicy {
        // burada private kalitim kullanilmasi tercih edilmeli
        // ...
        // public kalitim kullanma amaci su olabilir:
        //      policy class in interface ini ana class a enjecte etmek istiyoruz

    public:
        explicit ScopedPtr(T* p = nullptr, DelPolicy&& del_policy = DelPolicy()) 
        {}


        ~ScopedPtr()
        {
            DelPolicy::operator()(m_p);
                // template kodda ismi nitelemeden kullanilirsa
                // isim taban sinifta dogrudan aranmaz

            // alternatiif:
            this->operator()(m_p);

            // 3. alternatif private bolumde using
        }
    }
  ```

Kitap tavsiyes: Design patterns with C++
-> 

# bazen policy based olustururken, duruma gore bir seyin olup olmamasi karari verebilir:

  _ex:_
  ```cpp
    // scoped ptr 
    
    template<itypename T, typenmae DelPolicu = DefaultDelete<T>, template <typename> class ReleasePolicy = Releasable>
    class ScopedPtr : private DelPolicy, public ReleasePolicy<ScopedPtr<T, DelPolicy, ReleasePolicy>>
        // CRTP kullaniliyor
    {

    }
  ```

# observer pattern
  - bir nesnede degisiklik oldugunda buna bagimli olan diger nesnelerin bu degisiklikten haberdar olup buna bagimli bir degisiklik yapmasi gerekmesi gereken durumlarda kullanilir

  - durumu degisen nesne digerlerini haberdar eder

  - amac yuksek duzeyde bir coupling olmadan bunun saglanmasi

  gof:
  - define a one-to-many dependency between objects so that when one object changes state, all its dependents are notified and updated automatically
 
  applicatbility: 
  use the observer pattern ina ny of the following situations:
  - When an abstraction has two aspects, one dependan..., t

  ...

  subjects:
  - knows its observers. Any number of Observer objects may observe a subject
  - provides an interface for attaching and detaching observer objects

  observer:
  - defines an updating interface for objects that should be notified of changes in a subject

   picture:
       ObserverA -> 
                       Observer -o Subject
       ObserverB  ->

  _ex:_ minimal observer
  ```cpp
    // subject.h

    class IObserver;
    
    class ISubject {
    public:
        virtual ~ISubject() = default;
        virtual void register_observer(Iobserver*) = 0;
        virtual void remove_observer(Iobserver*) = 0;
        virtual void notify_observers() = 0;
    };

  ```cpp
    // weather_data.h   (concrete subject)
    
    #include "subject.h"
    #include <vector>

    class WeatherData : public ISubject {
    public:
        virtual void register_observer(IObserver*) override;
        virtual void remove_observer(IObserver*) override;
        virtual void notify_observers(IObserver*) override;

        void set_state(double temp, double humidity, double pressure)

    private:
        std::vector<IObserver*> m_observers;
        double m_temp{};
        double m_humidity{};
        double m_pressure{};
    };
  ```
  
  ```cpp
    //vector_data.cpp 
    #include "weather_data.h"
    #include <stdexcept>
    #include "observer.h"


    void WeatherData::register_observer(IObserver*)
    {
        m_observers.push_back(p);
    }

    void WeatherData::remove_observer(IObserver*)
    {
        if(auto n = std::erase(m_observers, p); n ==0)
            throw std::runtime_error{"observer cannot be removed!"};
    }

    void WeatherData::notify_observers(IObserver*)
    {
        for(auto p : m_observers)
            p->update(m_temp, m_humidity, m_pressure) ;
    }
  ```

  ```cpp
    // observer.h

    class IObserver {
    public:
        virtual ~IObserver() = default;
        virtual void update(double temprature, double humidity, double pressure) = 0;
    };
  ```

  ```cpp
    // textbox.h
    #include "observer.h"

    class Textbox : public IObserver {
    public:
        // ...

        virtual void update(double temprature, double humidity, double pressure) override;
    };
  ```

  ```cpp
    // slider.h
    #include "observer.h"

    class Slider: public IObserver {
    public:
        // ...

        virtual void update(double temprature, double humidity, double pressure) override;
    };
  ```

  ```cpp
    // alarm_sysmtes.h
    #include "observer.h"

    class AlarmSystems: public IObserver {
    public:
        // ...

        virtual void update(double temprature, double humidity, double pressure) override;
    };
  ```

  ```cpp
    // client

    #include "weather_data.h"
    #include "textbox.h"
    #include "slider.h"
    #include "alarm_system.h"

    int main()
    {
        // concrete observer
        WeatherData wd;         

        // concrete subjects 
        TextBox textbox;
        Slider slider;
        AlarmSystem alarm;

        wd.register_observer(&textbox);
        wd.register_observer(&slider);
        wd.register_observer(&alarm);

        wd.set_state(35, 3.4, 1.3);
            // slider.update is called implicitly

        wd.remove_observer(&textbox);
        wd.set_state(70, 3.4, 1.3);


    }
  ```

## Problemler:
### tek 1 observer, 1 den fazla subject i gozler

> buradaki en buyuk problemlerden birisi update interface ini tasarlamak
> ornegin burada update(double, double, double) ile interface i sabitledik

1. en uc seneryo:
  - observerlar, ihtiyac duysa da duymasada butun bilgiler gonderilir

2. pull modeli:
  - minimal bir bilgi geceriz: degisikli var
  - observerlar, subject sinifin fonksiyonlarini kullanarak kendi ihtiyaclarini gorurler

> bu durumda observer, neyin degistigini kendisi bulacak

3. en yaygin olarak kullanilan yapi:
  - update interface: observerlar sadece belirli degisiklik icin kayit yaptiracaklar

### tek 1 observer, 1 den fazla subject i gozler
  - ??? referans yoluyla hangi subject den geldigini 

  en kiritik nokta: dangling referans olusmasi
  * subject delete edilmis olabilir, observer in haberi olmayacak
  bunu bir sekilde cozmeliyiz.
  ex: dtor da observerlara haber gecer
  ex: weak_ptr yada benzer bir akilli smart pointer

 note: esas gof kalibinda (pull model) observerlar, subject e referans tutar

  _ex:_
  ```cpp
    gof daki ornek
  ```
================================================
# visitor, double dispatch

intent: represent an operation to be performed on the elements on an object structure.

visitor lets you define a new operation without changing the class of the elements on which it operates

buradaki hikaye double disoatche dayanir: 
behaviour birden fazla dinamik ture dayanir

Stroestop: multimethod ile hazir bir tool onerisi kabul edilmedi henuz

## double dispatch

interview:
  _ex:_
  ```cpp
    collide(car p1, carp2)

    2 arabanin turunu de bagli
  ```

  _ex:_
  ```cpp
    class Bmw;
    class Mercedes;
    class Audi;
    class Volvo;

    // kapali hieararchy

    class car {
    public:
        // hem collide hem de collide_with fonksiyonlari overload edilir
        virtual void collide(Car* p) override;
        
        virtual void collide_with(Bmw* p) override;
        virtual void collide_with(Audi* p) override;
        virtual void collide_with(Mercedes* p) override;
        virtual void collide_with(Volvo* p) override;

        virtual std::string name() const
        {
            return typeid(*this).name();
        }
    };
  ```
  ```cpp
    /// bmw.cpp
    void Bmw::collide(Car* p)
    {
        p->collide_with(this);
        // this icin static tur BMW oldugu icin Car sinifinin collide_with(Bmw*) cagirilacak
    }
  ```

  ```cpp
    void g_collide(Car* p1, Car* p2)
    {
        p1->collide(p2);
    }
    
    int main()
    {
        g_collide(new Audi, new Volvo);
            // Audi->collide(Volvo*)
            // {
            //      Volvo->collide_with(Audi*)
            // }
    }
    ```

> bir sinif daha eklenirse, Car interface ine bir sinif daha eklemek gerekecek

## visitor

  _ex:_ PetVisitor
  ```cpp
   class PetVisitor;

    class Pet {
    public:
        virtual ~Pet() = default;
        ivirtual void
    };

    class Cat: public et {
        using Pet::Pet;

    }

    void Cat::accept(PetVisitor& v)
    {
        v.visit(this);
    }


    class PetVisitor {
    public:
        virtual ~PetVisitor() = default;

        virtual void visit(Cat*) = 0;
        virtual void visit(Dog*) = 0;
        virtual void visit(Bird*) = 0;
    };

  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```

  _ex:_
  ```cpp
  ```
