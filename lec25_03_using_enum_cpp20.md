# (c++20) using enum class

## enum, legacy code
Remember that legacy code, consisting of ``enum`` does not require scope parameters.  
Therefore, without the newly introduced ```enum class``, we had to add scope parameters everywhere to modernize the code

```cpp
  // legacy code: enum does not require scope parameters

  enum rgba_color_channel {red, green, blue, alpha};

  std::string_view to_string(rgba_color_channel channel)
  {
      switch(channel) {
      case red  :  return "red  ";
      case green:  return "green";
      case blue :  return "blue ";
      case alpha:  return "alpha";
      }
  }
  ```
## disadvantages of enums, which are solved by enum types
  - scope u kirletir
    > ``enum class`` gelmeden once unscoped ``enum`` lari
    > (1) nested type yada (2) namespace icine alarak scope probleminden kurtulurduk.

  - underlying type depends on the compiler: therefore forward declaration was not allowed.
  
  - implicit conversions  
      * **arithmetic -> enum** is not allowed but
      * **enum -> arithmetic** is ok (shouldn't be).
  
However, to use enum types, we have to use scope parameters everywhere:  
  _ex_: ``color::red``  
  
This causes the following problems:  
 - kod kalabaligi 
 - daha fazla kod daha fazla hataya yol acar  
 

## (c++20) using enum class

  _ex:_
  ```cpp
    // see how ugly the scope operators look like
    enum class rgba_color_channel {red, green, blue, alpha};

    std::string_view to_string_old(rgba_color_channel channel)
    {
        switch(channel) {
        case rgba_color_channel::red  :  return "red  ";
        case rgba_color_channel::green:  return "green";
        case rgba_color_channel::blue :  return "blue ";
        case rgba_color_channel::alpha:  return "alpha";
        }
    }

    // we can transform the code above to by "using enum" expression

    std::string_view to_string(rgba_color_channel channel)
    {
        // here:
        // using enum rgba_color_channel;

        switch(channel) {
        // or here
        using enum rgba_color_channel;
        case red  :  return "red  ";
        case green:  return "green";
        case blue :  return "blue ";
        case alpha:  return "alpha";
        }
    }
  ```

### "using enum", bir enum set icerisindeki bir oge icin kullanilabilir

```cpp
  enum class rgba_color_channel {red, green, blue, alpha};

  int main()
  {
      // we can select only one element in an enum set
      using rgba_color_channel::red;
  }
```

### "using enum", nested type icinde tanimlanmis (geleneksel) enum lar icin de kullanilabilir

  _ex_
  ```cpp
    class Myclass{
    public:
        enum Color {red, green, blue};
    };

    int main()
    {
        auto c = Myclass::green;

        using enum Myclass::Color;
        
        auto c = red; 
    }
  ```