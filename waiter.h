//
// Created by alex on 28.03.2021.
//

#ifndef PHILOSOPHERS_WAITER_H
#define PHILOSOPHERS_WAITER_H

#include <vector>
#include <mutex>
#include <condition_variable>
#include <utility>

class waiter {
public:

    explicit waiter(const std::vector<std::condition_variable*>& _cVars, std::vector<bool*>  _priors);

    void notify_done(int _e);

    void check_priorities(int _e);

    void init();

    void notifyAll();

private:

    int find(int _e);

    bool checkPerm(int _e);

    std::vector<int> pIndexes;

    std::vector<bool*> priors;

    std::vector<std::condition_variable*> cVars;

    std::mutex m;
};


#endif //PHILOSOPHERS_WAITER_H
