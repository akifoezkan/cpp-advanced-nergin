# (c++20) exchange
  - utility.hpp
  - bir degiskene yeni deger atar, eski degerini return eder
  

  _ex_
  ```cpp
    template<typename T, typename U = T>
    constexpr T exchange(T& x, U&& new_value) noexcept(a condition)
    {
        T old_value = std::move(x);
        x = std::forward<U>(new_value);

        return old_value;
    }
  ```

> exchange in en cok kullanildigi seneryolardan biri move ctor/assignment implementation idir

  _ex_
  ```cpp
    struct Nec {
        int m_val;
        Nec(Nec&& other) noexcept : m_val{std::exchange(other.m_val, 0)}{}

        Nec& operator=(Nec&& other) noexcept
        {
            if(this ~= &other)
              mval = std::exchange(other.m_val, 0);
        }
    };
  ```

  _ex_
  ```cpp
    // simplified version of flags type in iostream
    class Stream {
    public:
        using flags_type = int;

        // get
        flags_type flags() const
        {
            return m_flags;
        }

        // set
        flags_type flags(flags_type val)
        {
            return std::exchange(m_flags, val);
        }

    private:
        flags_type m_flags{};
    };
  ```