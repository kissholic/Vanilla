

#include "core/coroutine/async_mutex.h"
#include "core/coroutine/task.h"
#include <set>
#include <string>

async_mutex mutex;
std::set<std::string>;

co_task<> add_item(std::string value) {
  async_mutex_lock lock = co_await mutex.scoped_lock_async();

  values.insert(std::move(value));
}

int main() {
  add_item("Hello");
  add_item("world");

  return 0;
}