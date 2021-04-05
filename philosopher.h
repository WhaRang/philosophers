//
// Created by alex on 27.03.2021.
//

#include <thread>
#include <string>
#include "tableware.h"
#include "waiter.h"
#include <condition_variable>
#include <random>
#include <utility>

#ifndef PHILOSOPHERS_PHILOSOPHER_H
#define PHILOSOPHERS_PHILOSOPHER_H


class philosopher {

public:

    std::thread start();

    void finish();

    explicit philosopher(int _index, std::string _name, waiter &w, bool &_isPrior,
                         std::condition_variable &_trigger, tableware &left, tableware &right, std::mutex &m);

    [[nodiscard]] std::string getName() const;

    [[nodiscard]] int getHunger() const;

    [[nodiscard]] int getSanity() const;

    [[nodiscard]] std::string getStatus() const;

private:

    const int MAX_AMOUNT = 23;

    const int MIN_AMOUNT = 17;

    int thinkAmount;

    int eatAmount;

    int hunger = MAX_AMOUNT;

    int sanity = MAX_AMOUNT;

    const float DEFAULT_TIME_PER_UNIT = 200000;

    void think();

    void eat();

    void life_cycle();

    int index;

    bool isDone = false;

    bool &isPrior;

    std::string name;

    std::string status;

    std::mutex &m;

    waiter &w;

    std::condition_variable &trigger;

    tableware &left;

    tableware &right;
};


#endif //PHILOSOPHERS_PHILOSOPHER_H
