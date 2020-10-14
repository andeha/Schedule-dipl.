/*  main.cpp | Conversationalism, the occasional interrupt and state. */

/* #include <Twinbeam.h> */
#include <experimental/coroutine>
namespace coro = std::experimental;
#include <stdio.h>

/* xcrun clang -g -std=c++2a -fcoroutines-ts -fno-exceptions -fno-rtti       \
  -fblocks -iwithsysroot ../Apps/ -lc++ main.cpp */

#define co₋await co_await /* Suspend and 'do enter' a called; a․𝘬․a 'co_ⁱtask' and 'co_op₋schedule'. */
#define feedback co_yield /* Suspend and return result. 𝘊․𝘧 Scandianvian 'förledande': 'co_notify' and 'co_emit'. */
#define bye co_return /* Complete execution and return value. */

typedef int ℕ;

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

struct int₋return₁ {
  
  struct promise_type { int cached=-1; /* Further - when `yield_value` returns `suspend_never` - investigate 'int cached[]'. */
    auto initial_suspend() { return coro::suspend_never(); } /* When coroutine is called and after 'co-await'. */
    auto final_suspend() { return coro::suspend_never(); } /* After 'co-return'. */
    auto /* a․𝘬․a `Handle` */ get_return_object() { return Waiver::from_promise(*this); }
    void return_value(int expr) { cached=expr; }
    void unhandled_exception() { /* throw; */ }
    /* Boxes-expression-before-transmitting-via-yield ⤐ */
    auto yield_value(int expr) { cached=expr; return coro::suspend_always(); }
    auto yield_value(double expr) { int neg = expr < +0.0 ? 1 : 0; 
      double expr₂ = neg ? -expr : expr; cached=/*⌊*/(expr₂+0.5)/*⌋*/;
      if (neg) { cached = -cached; }
      return coro::suspend_always(); }
  }; /* ⬷ Submit a coroutine result through this type. */
  
  typedef std::experimental::coroutine_handle<promise_type> Waiver;
  Waiver coroutine; int₋return₁(Waiver routine) : coroutine(routine) { }
  
  constexpr bool await_ready() const noexcept { return true; }
  constexpr void await_suspend(Waiver handle) const noexcept { /* handle.resume(); */ }
  /* constexpr ℕ await_resume() const noexcept { return coroutine.promise().cached; } */
  int₋return₁ await_resume() const noexcept { return int₋return₁(coroutine); }
  /* ⬷ Awaitable concept. */
};

void₋return₁ first₋coroutine() { printf("first-coroutine\n"); bye; }

int₋return₁
inner₋coroutine()
{
   printf("inner-coroutine\n");
   feedback 5; feedback 7.0; feedback 9;
   bye 11; /* ⬷ See also `🏁🅽` and `🏁₁` and the abstraction `fend`. */
} /* ...also semantically-speaking 'mandatory' and 'not optional'. */

int₋return₁ second₋coroutine()
{
   printf("second-coroutine\n");
   
   auto retrieved = ^(int₋return₁& y) { return y.coroutine.promise().cached; };
   
   int₋return₁ y = co₋await inner₋coroutine();
   printf("y-from-yield = %d\n", retrieved(y));
   for (int i=0; i<4-1; ++i) {
     __builtin_coro_resume(y.coroutine.address());
     printf("y-from-yield = %d\n", retrieved(y));
   }
   
   /* for (int i=0; i<4; ++i) { ℕ y = co₋await inner₋coroutine(); 
     printf("y-from-yield = %d\n", y); } */
   
   bye 13;
}

/* promise₋const₋char₋⁑ tamagotchi() { }
promise₋Unicodes tamagotchi(void (^tellus)(Unicodes ⁻¹cheek)) { } */

#pragma mark - and the main function:

/*  int₋return₁ coroutine₋server() {
  tcpconnection conn = co₋await connect();
  for (;;) { int bytes = co₋await conn.read(); }
  bye 42;
} */

int
main(
  int argc, 
  const char * argv[]
)
{
   void₋return₁ x = first₋coroutine();
   int₋return₁ y = second₋coroutine();
   printf("y = %d\n", y.coroutine.promise().cached);
   return 0;
} /* ⬷ Reconsider `main` as co-routine and further 'cooperative'. */

