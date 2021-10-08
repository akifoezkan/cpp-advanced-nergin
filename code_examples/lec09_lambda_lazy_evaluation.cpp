// why use invoke(f, args...) instead of std::invoke(f, args...)
// when f is a pointer to a class member function and arg1 is a class object
// https://stackoverflow.com/questions/46388524/when-to-use-stdinvoke-instead-of-simply-calling-the-invokable

#include <iostream>
#include <tuple>
#include <functional>



int foo(int x, int y, int z)
{
    std::cout << x << " " << y << " " << z << "\n";
    std::cout << "foo is called\n";
    return x + y + z;
}

template<typename F, typename ...Args>
auto direct_invoke(F f, Args... args)
{
    return std::invoke(f, args...);
}

// =================== c++17 ============================
// both of the functions below copies args... to lambda
template<typename F, typename ...Args>
auto delay_invoke(F f, Args... args)
{
    return [f, args...]() -> decltype(auto) {
        return std::invoke(f, args...);
    };
}

// same with above
template<typename F, typename ...Args>
auto delay_invoke1(F f, Args... args)
{
    return [=]() -> decltype(auto) {
        return std::invoke(f, args...);
    };
}


// ========= capture parameter pack by forwarding or moving ===========
#define syntax_error
#ifdef syntax_error
// this is syntax error in both c++17, c++20
template<typename F, typename ...Args>
auto delay_invoke(F&& f, Args&&... args)
{
    // variable 'args' cannot be implicitly captured in a lambda with no capture-default specified
    return [f = std::move(f), _args = std::move(args)...]() -> decltype(auto) {
        return std::invoke(f, _args...);
    };
}
#endif

// # workaround before cpp20 => use tuple for wrapping parameter pack
template<typename F, typename ...Args>
auto delay_apply(F&& f, Args&&... args)
{
    return [f = std::move(f), _args = std::make_tuple(std::move(args)...)]() -> decltype(auto) {
        return std::apply(f, _args);
    };
}

template<typename F, typename ...Args>
auto delay_apply_forward(F&& f, Args&&... args)
{
    return [f = std::forward<F>(f), _args = std::make_tuple(std::forward<Args>(args)...)]() -> decltype(auto) {
        return std::apply(f, _args);
    };
}

#define cpp20
#ifdef cpp20
// =================== c++20 ============================
template<typename F, typename ...Args>
auto delay_call(F&& f, Args&&... args)
{
    return [f = std::forward<F>(f), ..._args = std::forward<Args>(args)]() -> decltype(auto) {
        return std::invoke(f, _args...);
    };
}
#endif


int main() {

    std::cout << "======= direct invoke test ============\n";
    std::cout << direct_invoke(foo, 3, 5, 7) << "\n";

    std::cout << "======== delay invoke test ==========\n";
    std::cout << "create f:\n";
    auto f = delay_invoke(foo, 3, 5, 7);

    std::cout << "call f:\n";
    f();

    std::cout << "======== delay apply move test ==========\n";
    std::cout << "create g:\n";
    auto g = delay_apply(foo, 3, 5, 7);

    std::cout << "call g:\n";
    g();

    std::cout << "======== delay apply forward test ==========\n";
    std::cout << "create g:\n";
    auto g2 = delay_apply_forward(foo, 3, 5, 7);

    std::cout << "call g:\n";
    g2();


#ifdef cpp20
    std::cout << "======== delay call c++20 test ==========\n";
    std::cout << "create g:\n";
    auto g3 = delay_call(foo, 3, 5, 7);

    std::cout << "call g:\n";
    g3();
#endif

    return 0;
}
