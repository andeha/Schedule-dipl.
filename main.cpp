/*  main.cpp | Conversationalism, the occasional interrupt and state possibly-maybe. */

#include <experimental/coroutine>

/* xcrun clang -g -std=c++2a -fcoroutines-ts -fno-exceptions -lc++ main.cpp */

namespace coro = std::experimental; typedef int ℕ;
#define co₋await co_await /* Suspend and 'do enter' a called. A․𝘬․a 'co_ⁱtask' and 'co_op₋schedule'. */
#define co₋yield co_yield /* Suspend and return result. 𝘊․𝘧 Scandianvian 'förledande': 'co_notify' and 'co_emit'. */
#define co₋return co_return /* Complete execution and return value. */

#pragma mark - The 'promise' type and the 'awaitable' type:

struct void₋return₁ {
  struct promise_type {
   auto initial_suspend() { return coro::suspend_never(); }
   auto final_suspend() { return coro::suspend_never(); }
   auto return_void() { return void₋return₁(); }
   auto get_return_object() { return void₋return₁(); }
   void unhandled_exception() { /* throw; */ }
  };
};

struct int₋return₁ { /* typedef coro::coroutine_handle<promise_type> Handle; 
  
  Handle coroutine; int₋return₁(Handle routine): coroutine(routine) { } */
  
  struct promise_type { int cached=-1; 
    auto initial_suspend() { return coro::suspend_never(); }
    auto final_suspend() { return coro::suspend_never(); }
    /* auto get_return_object() { Handle::from_promise(*this) } */
    auto get_return_object() { return int₋return₁(cached); }
    void return_value(int expr) { cached=expr; }
    void unhandled_exception() { /* throw; */ }
    /* Box-expression-before-transmitting-via-yield: */
    auto yield_value(int expr) { cached=expr; return coro::suspend_always(); }
    /* auto yield_value(int₋return₁ expr) { return coro::suspend_never(); } */
  } inner; /* Submit a coroutine result through this type. */
  
  int₋return₁(int reference) { inner.cached = reference; }
  
  constexpr bool await_ready() const noexcept { return true; }
  constexpr void await_suspend(coro::coroutine_handle<> handle) const noexcept { }
  /* constexpr ℕ await_resume() const noexcept { return coroutine.promise().cached; } */
  constexpr ℕ await_resume() const noexcept { return inner.cached; }
  /* ⬷ Awaitable concept. */
};

void₋return₁ first₋coroutine() { printf("first-coroutine\n"); co_return; }

int₋return₁
inner₋coroutine()
{
   printf("inner-coroutine\n");
   co₋yield 5; co₋yield 7; co₋yield 9;
   co₋return 11;
}

int₋return₁ second₋coroutine()
{
   printf("second-coroutine\n");
   for (int i=0; i<4; ++i) { ℕ y = co₋await inner₋coroutine(); printf("yield-y = %d\n", y); }
   co₋return 13;
}

/* promise₋const₋char₋⁑ tamagotchi() { }
promise₋Unicodes tamagotchi(void (^tellus)(Unicodes ⁻¹cheek)) { } */

#pragma mark - and the main function:

/* int₋return₁ coroutine₋server() {
  tcpconnection conn = co₋await connect();
  for (;;) { int bytes = co₋await conn.read(); }
  co_return 42;
} */

int
main(
  int argc, 
  const char * argv[]
)
{
   void₋return₁ x = first₋coroutine();
   int₋return₁ y = second₋coroutine();
   /* printf("returned-y = %d\n", y.coroutine.promise().cached); */
   printf("returned-y = %d\n", y.inner.cached);
   return 0;
}


