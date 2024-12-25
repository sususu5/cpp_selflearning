#include <iostream>
#include <vector>
#include <functional>

// Defines the callback function type with parameters
using Callback = std::function<void (int)>;

// Event system class
class EventSystem {
private:
    std::vector<Callback> callbacks;

public:
    // Registers a callback function
    void registerCallback(const Callback &cb) {
        callbacks.push_back(cb);
    }

    // Triggers the event and passes the parameter to the callback function
    void triggerEvent(int data) {
        std::cout << "Event triggered with data = " << data << ". Executing callbacks..." << std::endl;
        for (auto &cb : callbacks)
            cb(data);
    }
};

// Example function as a callback
void onEvent(int data) {
    std::cout << "Function callback received data: " << data << std::endl;
}

int main() {
    // This instance is created on stack...
    EventSystem eventSystem;

    // Registers a callback based on a function pointer
    eventSystem.registerCallback(onEvent);

    // Registers a callback based on a Lambda expression
    eventSystem.registerCallback([](int x) {
        std::cout << "Lambda callback received: " << x * 2 << std::endl;
    });

    // Registers a callback based on a Lambda expression that captures variables
    int multiplier = 5;
    eventSystem.registerCallback([multiplier](int x) {
        std::cout << "Lambda with capture received: " << x * multiplier << std::endl;
    });

    // Triggers events and passes the parameter
    eventSystem.triggerEvent(10);

    return 0;
}