# Jellybelly
api for jelly-belly-wiki.netlify.app site about Jelly Belly candy
# main
```cpp
#include "Jellybelly.h"
#include <iostream>

int main() {
   Jellybelly api;

    auto recipes = api.get_recipes(1).then([](json::value result) {
        std::cout << result<< std::endl;
    });
    recipes.wait();
    
    return 0;
}
```

# Launch (your script)
```
g++ -std=c++11 -o main main.cpp -lcpprest -lssl -lcrypto -lpthread -lboost_system -lboost_chrono -lboost_thread
./main
```
