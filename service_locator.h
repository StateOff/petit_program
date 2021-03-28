//
// Created by bfloch on 3/15/21.
//

#ifndef PETIT_PROGRAM_SERVICE_LOCATOR_H
#define PETIT_PROGRAM_SERVICE_LOCATOR_H

#include "service_input.h"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

template<typename ...TYPES>
class ServiceLocator
{
public:
    using ServiceLocatorType = std::tuple<TYPES...>;

    template<typename T>
    static T& get()
    {
        T* service = std::get<T*>(services_);
        assert(service != nullptr);
        return *service;
    }

    template<typename T>
    static void set(T* service)
    {
        std::get<T*>(services_) = service;
    }

private:
    static ServiceLocatorType services_;
};

// -- Service Locator definition

using GameServiceLocator = ServiceLocator<InputService*>;
// Initialized lazily, be sure to set the service as early as possible
template<> GameServiceLocator::ServiceLocatorType GameServiceLocator::services_{nullptr};

// Shortcuts
#define INPUT GameServiceLocator::get<InputService>()

#ifdef PETIT_TESTS

#include "debug.h"

class HelloTestService
{
public:
    [[nodiscard]] static constexpr std::string_view name()
    {
        return "Hello";
    }
};

class WorldTestService
{
public:
    [[nodiscard]] static constexpr std::string_view name()
    {
        return "World";
    }
};

using TestServiceLocator = ServiceLocator<HelloTestService*, WorldTestService*>;
template<> TestServiceLocator::ServiceLocatorType TestServiceLocator::services_{nullptr, nullptr};

#define HELLOTEST TestServiceLocator::get<HelloTestService>()
#define WORLDTEST TestServiceLocator::get<WorldTestService>()

bool testServiceLocator()
{
    enum TestServices : size_t
    {
        TEST_SERVICE_HELLO,
        TEST_SERVICE_WORLD,
    };

    HelloTestService helloTestService;
    WorldTestService worldTestService;
    TestServiceLocator::set(&helloTestService);
    TestServiceLocator::set(&worldTestService);

    PETIT_ASSERT(HELLOTEST.name() == "Hello");
    PETIT_ASSERT(WORLDTEST.name() == "World");

    return true;
}
#endif

#endif//PETIT_PROGRAM_SERVICE_LOCATOR_H
