# FUSS

FUSS is a simple and efficient pub/sub pattern implementation aimed for C++17 programs with an intuitive interface. 
It consists of a single header file and has no external dependencies.

## Features

- Simple, efficient, small and elegant. Just read the code!
- Leverages modern C++ to provide an intuitive and pleasant API.
- Avoids unnecessary dynamic memory as much as possible.
- No virtual functions. 
- No RTTI. 

## Example

```C++
#include <fuss.hpp>

// Define what messages are available to be published.
struct greeting : fuss::message<> {};

// Note that messages can have parameters.
struct echo : fuss::message<std::string> {};

// Create a publisher object
struct noisy : fuss::shouter<greeting, echo> {
    void greet() {
        // Publishes the `greeting` message.
        // `shout()` is protected, so we create a member function to call it
        shout<greeting>();
    }

    void repeat(const std::string &s) {
        // Publishes the `echo` message with a string as parameter
        shout<echo>(s);
    }
};

// Instantiate the publisher
noisy n;

// Subscribe to the messages
auto greeting_listener = n.listen<greeting>([]() {  
    std::cout << "Hello world!" << std::endl;
});

n.listen<echo>([](std::string s) {
    std::cout << s << std::endl;
});

n.greet(); // prints 'Hello world!'
n.repeat("What a fuss!"); // prints 'What a fuss!'

// Unsubscribe from a message
n.unlisten(greeting_listener);

n.greet(); // doesn't print anything

```

## Copyright

Copyright André Medeiros 2022
Contact: andre@setadesenvolvimentos.com.br