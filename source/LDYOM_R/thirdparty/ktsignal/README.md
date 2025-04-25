# ktsignal

A single header signal library

## Usage

Simply include ktsignal.hpp. C++17 compatible compiler required

## Examples

### Basics

The following example shows how to connect and emit a signal

```cpp
void on_click(int value) { }

class A {
public:
    void on_class_click(int value) { }
};

int main() {
    // Signal with function signature in template parameter
    ktsignal::ktsignal<void(int)> click{};

    // Connect to a function callback
    click.connect(on_click);
    
    // Connect to a member function callback
    A object;
    click.connect(&object, &A::on_class_click);

    // Connect to a lambda callback
    click.connect([](int){ });

    // Signal emit
    click.emit(1);
}
```

Scoped connections automatically disconnect in the end of scope

```cpp
void on_click(int value) { }

class A {
public:
    void on_class_click(int value) { }
};

int main() {
    ktsignal::ktsignal<void(int)> click{};
    
    {
        auto connection = click.scoped_connect(on_click);
        A object;
        auto method_connection = click.scoped_connect(&object, &A::on_class_click);

        click.scoped_connect([](int v){ std::cout << "This will never be printed" << std::endl; });

        // on_click and on_class_click are called
        click.emit(1);
    }
    // nothing is called
    click.emit(1);
}
```

### Connectins / Disconnecting callbacks

You can easily disconnect slot from Signal

```cpp
void on_click(int value) { }

class A {
public:
    void on_class_click(int value) { }
};

int main() {
    ktsignal::ktsignal<void(int)> click{};

    // Save connection
    auto connection = click.connect(on_click);

    // Connect to a lambda callback
    click.connect([](int){ });

    // Signal emit (on_click and lambda are called)
    click.emit(1);

    // Disconnect on_click callback from signal
    connection.disconnect();

    // Only lambda will be called
    click.emit(1);
}
```

A few important notes about `ktsignal_connection` object
- `ktsignal_connection` is default-constructible, moveable but not copyable.
- You must make sure that the connection will not be used after the signal is destroyed.

### Iterating through signal slots

You can easily iterate over signal slots via range-based for loop and get slot return value

```cpp
int on_click(int value) { return 5; }
int on_click_second(int value) { return 1; }

int main() {
    ktsignal::ktsignal<int(int)> click{};

    click.connect(on_click);
    click.connect(on_click_second);

    // Will display `emit_iterate returned 5 emit_iterate returned 1`
    for (auto returned : signal.emit_iterate(1)) {
        std::cout << "emit_iterate returned " << returned << " ";
    }
}
```

Also you can use functions from the stdlib

```cpp
int on_click(int value) { return 5; }
int on_click_second(int value) { return 1; }

int main() {
    ktsignal::ktsignal<int(int)> click{};

    click.connect(on_click);
    click.connect(on_click_second);

    auto iterate = signal.emit_iterate(0);
    auto accumulated = std::accumulate(iterate.begin(), iterate.end(), 0);

    // Will display 6
    std::cout << "Accumulated: " << accumulated << std::endl;
}
```

### Using `ktsignal` in multithreaded code

For multi-threaded use, you should use `ktsignal_threadsafe`

```cpp
void func_thread(int v) {
    std::cout << "before sleep" << std::endl;
    std::this_thread::sleep_for(1000ms);
    std::cout << "after sleep" << std::endl;
    return 2;
}

int main() {
    ktsignal::ktsignal_threadsafe<int(int)> signal{};

    signal.connect(func_thread);

    // Create a thread that emit immediately
    std::thread([&signal]() {

        // Create a thread that emit after 100ms
        std::thread(
            [&signal]() {
                std::this_thread::sleep_for(100ms);
                signal.emit(1);
            }
        ).detach();

         signal.emit(2);
    }).join();

    std::this_thread::sleep_for(1.5s);
}
```

```
Output:
[func_thread] - before sleep
[func_thread] - before sleep
[func_thread] - after sleep
[func_thread] - after sleep
```

You can also use the `ktsignal_threadsafe_emit` when you need to make the emit function blocking.

```cpp
void func_thread(int v) {
    std::cout << "before sleep" << std::endl;
    std::this_thread::sleep_for(1000ms);
    std::cout << "after sleep" << std::endl;
    return 2;
}

int main() {
    ktsignal::ktsignal_threadsafe_emit<int(int)> signal{};

    signal.connect(func_thread);

    // Create a thread that emit immediately
    std::thread([&signal]() {

        // Create a thread that emit after 100ms
        std::thread(
            [&signal]() {
                std::this_thread::sleep_for(100ms);
                signal.emit(1);
            }
        ).detach();

         signal.emit(2);
    }).join();

    std::this_thread::sleep_for(1.5s);
}
```

```
Output:
[func_thread] - before sleep
[func_thread] - after sleep
[func_thread] - before sleep
[func_thread] - after sleep
```