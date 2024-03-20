#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <thread>

#include "thread_pool.hpp"

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<int> dist(-1000, 1000);

auto rnd = std::bind(dist, mt);

void simulate_hard_computation() {
  std::this_thread::sleep_for(std::chrono::milliseconds(2000 + rnd()));
}

void multiply(const int a, const int b) {
  simulate_hard_computation();
  const int res = a * b;
  std::cout << "current thread id : " << std::this_thread::get_id()
            << std::endl;
  std::cout << a << " * " << b << " = " << res << std::endl;
}

void multiply_output(int &out, const int a, const int b) {
  simulate_hard_computation();
  out = a * b;
  std::cout << a << " * " << b << " = " << out << std::endl;
}

int multiply_return(const int a, const int b) {
  simulate_hard_computation();
  const int res = a * b;
  std::cout << a << " * " << b << " = " << res << std::endl;
  return res;
}

void example() {
  auto const MaxThreadCount = std::thread::hardware_concurrency();
  std::cout << "Max thread count supported: " << MaxThreadCount << std::endl;

  demo_thread_pool::ThreadPool pool(std::thread::hardware_concurrency());

  pool.init();

  for (int i = 1; i <= 3; ++i) {
    for (int j = 1; j <= 10; ++j) {
      pool.submit(multiply, i, j);
    }
  }
  int out_ref;
  auto future1 = pool.submit(multiply_output, std::ref(out_ref), 5, 6);
  future1.get();
  std::cout << "Last operation result is : " << out_ref << std::endl;

  // 使用带返回值的提交函数
  auto future2 = pool.submit(multiply_return, 5, 3);
  auto res = future2.get();
  std::cout << "Last operation result is : " << res << std::endl;

  pool.shutdown();
}

int main(int argc, char *argv[]) {
  std::cout << "hello,world" << std::endl;
  example();
  return 0;
}
