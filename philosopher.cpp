//
// Created by alex on 27.03.2021.
//

#include "philosopher.h"


philosopher::philosopher(int _index, std::string _name, waiter &w, bool &_isPrior,
                         std::condition_variable &_trigger, tableware &_left, tableware &_right, std::mutex &_m)
        : index(_index), name(std::move(_name)),
          w(w), isPrior(_isPrior), trigger(_trigger),
          left(_left), right(_right), m(_m) {
}


void philosopher::life_cycle() {

    while (!this->isDone) {
        think();
        eat();
    }

    //std::cout << this->name << " is done" << std::endl;
}


void philosopher::finish() {
    this->isDone = true;
}


void philosopher::think() {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(MIN_AMOUNT, MAX_AMOUNT);

    thinkAmount = distr(gen);

    status = "thinking";
    for (int i = 0; i < thinkAmount; i++) {
        //std::cout << this->name << " is thinking..." << std::endl;

        if (hunger > 0) {
            hunger--;
        }
        if (sanity < MAX_AMOUNT) {
            sanity++;
        }

        usleep(DEFAULT_TIME_PER_UNIT);

        if (isDone) {
            return;
        }
    }

}


void philosopher::eat() {

    //std::cout << this->name << " is waiting for permission to eat..." << std::endl;

    std::unique_lock<std::mutex> ul1(m);
    status = "waiting for permission to eat";
    trigger.wait(ul1, [this] {return isPrior || isDone;});
    //std::cout << index << " " << isPrior << std::endl;

    status = "waiting for forks";
    trigger.wait(ul1, [this] {
        int result = 0;
        if (!isDone) {
            result = std::try_lock(*left.use(index), *right.use(index));
        }
        return (result == -1) || isDone;
    });

    if (isDone) {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(MIN_AMOUNT, MAX_AMOUNT);

    eatAmount = distr(gen);

    status = "eating";
    for (int i = 0; i < eatAmount; i++) {
        //std::cout << this->name << " is eating..." << std::endl;
        if (hunger < MAX_AMOUNT) {
            hunger++;
        }
        if (sanity > 0) {
            sanity--;
        }
        usleep(DEFAULT_TIME_PER_UNIT);

        if (isDone) {
            return;
        }
    }

    left.use(index)->unlock();
    right.use(index)->unlock();

    left.returnT();
    left.returnT();

    w.notify_done(index);
}


std::thread philosopher::start() {
    return std::thread(&philosopher::life_cycle, this);
}


std::string philosopher::getName() const {
    return name;
}


int philosopher::getHunger() const {
    return hunger;
}


int philosopher::getSanity() const {
    return sanity;
}


std::string philosopher::getStatus() const {
    return status;
}
