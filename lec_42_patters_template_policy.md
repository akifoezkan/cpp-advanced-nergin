#15 dk gec kaldim


# Template Pattern 

## herb sutter: virtuality article
  - read multiple times

  summary:
  - Gudeline-1: prefer to make interfaces non-virtual ...
  -Guideline-2
  -Guideline-3


=> ?? 30 dk kacirdim


## 

_ex:_

```cpp
    // pizza.h
    class Pizza {
    public:
        Pizza(std::string name);
        ~Pizza() = default;

        void prepare();     // NVI

    protected:
        void prepare_dough();
        virtual add_sauce();
        virtual void bake();


    private:
        virtual void add_toppings() = 0;
            // her pizza turu kendisi tanimlayacak
    };
```

```cpp
    // pizza.cpp

    void Pizza::prepare()
    {
        prepare_dough();
        
        add_toppings();
    }
```

```cpp
    // veggie_pizza.hpp
    #include "pizza.h"

    class VeggiePizza : public Pizza {
    public:

    protected:
        virtual add_sauce() override;
        virtual void bake() override;


    private:
        virtual void add_toppings() override;
    }
```


```cpp
    // main 
    #include "pizza.h"

    void func()
    {
        Pizza* p = new VeggiePizza;

        p->prepare();
        
        // prepare fonk. cagirildiginda VeggiePizza nin override lari cagirilacak
    }
```

> => Factory method, template method un bir varyasyonu olarak gorulebilir

## GoF

### Participants:
  - AbstractClass
    * Pizza class in the example
  - ConcreteClass
    * VeggiePizza

> STL: hem container tarafinda hem de algoritma tarafinda guzel ornekler sunar:
  ex: sort alg. da karsilastirma ???

================================= tuple idioms ===============================

_ex_
```cpp
    // normalde bu is icin swap fonk kullanilir
    // ancak bu puzzle vari bir uygulama
    int main()
    {
        int x = 23, y = 34;

        cout << "x = " << x << "y = " << y << 'n';
        std::tie(x, y) = pair{y, x};

        // tie returns a tuple that holds a ref
        // pair.first = x e referans
        // pair.second= y e referans
        // sunun gibi:
        // std::pair<int&, int&> p {x, y};
        // p.first = 56;
        // p.second = 99;
    }
```

> bazen yukaridaki olgu epey ise yarar

_ex_
```cpp
    #include <iostream>
    #include <tuple>
    
    int main()
    {
    	using namespace std;
    
    	int x = 10, y = 20, z = 30;
    
    	tie(x, y, z) = tuple(y, z, x);
    
    	std::cout << "x = " << x << "\n";
    	std::cout << "y = " << y << "\n";
    	std::cout << "z = " << z << "\n";
    }
```

## poor man's spaceship operator

_ex_
```cpp
    template<typename T, typename U>
    bool operator<(const std::pair<T, U>& rhs, const std::pair<T, U>& rhs)
    {
        return lhs.first < rhs.first || !((rhs.first < lhs.first) && (lhs.second < rhs.second))
    }
```

> use tuple to compare classes with multiple member instances as follows
> note that spaceship operator <=> solves this problem

_ex_
```cpp
    class Date {
    public:
    	//...
    	friend bool operator<(const Date&, const Date&);
    };
    
    class Myclass {
    public:
    	//...
    	friend bool operator<(const Myclass& r1, const Myclass& r2)
    	{
    		return std::tuple{ r1.mx, r1.m_name, r1.m_date } < std::tuple{ r2.mx, r2.m_name, r2.m_date };
    	}
    private:
    	int mx{};
    	std::string m_name{};
    	Date m_date{};
    };
```

## tuple apply

_ex_
```cpp
    int sum(int x, int y, int z)
    {
        return x + y + z;
    }

    int main()
    {
        std::tuple t{1, 6, 9};
        
        cout << apply(sum, t) << '\n'; 

        // cout << sum(get<0>t, get<1>(t), get<2>(t)) << '\n';
    }
```

==============================================================
# gof: Strategy pattern
  -> also known as policy
  -> intent: define a family of algorithms, encapsulate each onem and make them interchangable
## applicability: 
  - many related classes differ only in their behaviour. a way to configure a class with one of many behaviours
  => if/else merdiveni yerine interface ile class i configure ederiz

  - idifferent variants of an algorithm, ...
  ...

## participants:
  - strategy: context uses this interface to call the algorithm defined by a concrete strategy
  - concrete strategy: ConcreteStrategy implements the algorithm using the Strategy interface
  - context:

class diagrami:
   context <- strategy <- concreteStrategyA
                       <- concrete strategyB


_ex_
```cpp
    // Context.h
    #include <memory>

    class Strategy;

    class Context {
    public:
        Context(Strategy*);     // can be a polymorphic class
        void do_something();
        void set_strategy(Strategy*);

    private:
        std::unique_ptr<Strategy> mps;
            // composition yolu
    };
```

```cpp
    // strategy.h
    
    class Strategy {
    public:
        virtual ~Strategy() = default;
        virtual void do_algorithm = 0;
    };
```

```cpp
    // Context.cpp
    #include "context.h" 


    Context::Context(Strategy* p) : mps{p} {}

    void Context::do_something(
    {
        std::cout << "context does something as per its business logic";
        mps->do_algorithm();

    }

    void Context::set_strategy(Strategy* p)
    {
        mps.reset(p);
    } 
```

```cpp
    // NecStrategy.h
    #pragma once
    #include "strategy.h"

    class NecStrategy : public Strategy {
    public:
        NecStrategy(); 
        ~NecStrategy(); 

        virtual void do_algorithm()
    };
```

```cpp
    // NecStrategy.cpp
    #pragma once
    #include "necstrategy.h"

    NecStrategy::NecStrategy(); 
    {
        std::cout << "Neco strategy object created!\n";
    }
    NecStrategy::~NecStrategy(); 
    {
        std::cout << "Neco strategy object destroyed!\n";
    }

    NecStrategy::do_algorithm(); 
    {
        std::cout << "Neco strategy is being used!\n";
    }
```

```cpp
    // ErgStrategy.h
    #pragma once
    #include "strategy.h"

    class ErgStrategy : public Strategy {
    public:
        ErgStrategy(); 
        ~ErgStrategy(); 

        virtual void do_algorithm()
    };
```

```cpp
    // ErgStrategy.cpp
    #pragma once
    #include "necstrategy.h"

    ErgStrategy::ErgStrategy(); 
    {
        std::cout << "Neco strategy object created!\n";
    }

    ErgStrategy::~ErgStrategy(); 
    {
        std::cout << "Neco strategy object destroyed!\n";
    }

    ErgStrategy::do_algorithm(); 
    {
        std::cout << "Erg strategy is being used!\n";
    }
```

```cpp
    #include "necstrategy.h"
    #include "necstrategy.h"
    #include "necstrategy.h"
    int main()
    {
       Context c{new NecStrategy}; 
       c.do_something();

    }


    // Context in kodunu hic degistirmeden, do_something ile hangi alg kullanacagi
    // open-close prinicple cignenmeden ...

    // degisiklik, koda ekleme yoluyla gerceklestirilecek
```


# policy-based design
  - policy bir template, strategy pattern
  - STL ex: allocator sinifi

  ana sinif: host class
  telpate  : policy class

  amac:
  - esneklik: tek bir sinif var, template arg. ile istedigim strategy i implement edecek yeni bir siinif
  - genisletebilirlik: yeni policy ler ile yeni siniflar
  - customizability
 
  dezavantajlar:
  - olusan her sinif turu, birbirinden farkli.
  farkli araclar kullanilmazsa birbirine atanabilir degil.

  > cogunlukla bagimsiz kullanilmasi yerine, template arg olarak kullanilacak siniflar: policy

_ex_
```cpp
    // host class
    template<typename T, typename AllocationPolicy, typename DeletePolicy> 
    class Container {
        
    };
```

> amac cogunlukla "has-a" relationship, "is-a" degil. 
> Bu yuzden cogunlukla public kalitim kullanmayacagiz.
> policy'i interface e eklemiyoruz. 

> Composition: containment yolu ile yada inheritance ile
> Eski C++ da cogunlukla kalitim kullanilmasinin temel sebebi empty class turu elemanlar soz konusu oldugunda bellek alaninin artirmamak.
> ancak artik optimize_??? attribute ile bu sorun ortadan kalkti.

_ex_
```cpp
    // hello policy-based design

    // print (policy), get_message(policy)
    template<typename OutputPolicy, typename LanguagePolicy>
    class HelloWorld : private OutputPolicy, private LanguagePolicy {
    public:
        // behaviour method
        void run() const
        {
            // print(get_message());   
                // boyle yazamam cunku (taban sinifi template parameter ise)
                // derleyici print ve get_message i taban siniflarinda aramaz
                // howto solve it: 
                // 1. using 
                // 2. this
                //  this->print(this->get_message());   

                // 3. 
                //  OutputPolicy::print(LanguagePolicy::get_message());   
        }

    private:
        // 1
        using LanguagePolicy::get_message;  // is visible to HelloWorld
        using OutputPolicy::print;        // is visible to HelloWorld
        
    };


    class CoutOutputPolicy{
    protected:
        
    };

    template<typename T>
    class CoutOutputPolicy2{
    protected:
        void print(T&&) const 
        {

        }
    };
    
    class CoutOutputPolicy3{
    protected:
        template<typename T>
        void print(T&&) const 
        {
            cout << x << '\n';
        }
    };

    class EnglishLanguagePolicy {
    public:
        std::string get_message() const
        {
            return "hello world!";
        }
    };
    
    class GermanLanguagePolicy {
    public:
        std::string get_message() const
        {
            return "hallo welt!";
        }
    };

    using English = HelloWorld<CoutOutputPolicy, EnglishLanguagePolicy>;
    using Turkish = HelloWorld<CoutOutputPolicy, TurkishLanguagePolicy>;
    using German = HelloWorld<CoutOutputPolicy, GermanLanguagePolicy>;


    int main()
    {
        
    }

```

_ex_
```cpp
```

_ex_
```cpp
```

_ex_
```cpp
```

_ex_
```cpp
```

_ex_
```cpp
```

_ex_
```cpp
```

_ex_
```cpp
```

_ex_
```cpp
```





























