//
// Created by alex on 28.03.2021.
//

#include "waiter.h"


waiter::waiter(const std::vector<std::condition_variable *> &_cVars, std::vector<bool *> _priors)
        : cVars(_cVars), priors(std::move(_priors)) {
    for (int i = 0; i < _cVars.size(); i++) {
        pIndexes.push_back(i);
    }
}


bool waiter::checkPerm(int _e) {

    int iPrior = find(_e);

    int rPrior;
    if (_e > 0) {
        rPrior = find(_e - 1);
    } else {
        rPrior = find(pIndexes.size() - 1);
    }

    int lPrior;
    if (_e < pIndexes.size() - 1) {
        lPrior = find(_e + 1);
    } else {
        lPrior = find(0);
    }

    return (iPrior < lPrior && iPrior < rPrior);
}


void waiter::notify_done(int _e) {

    *(priors[_e]) = false;

    const std::lock_guard<std::mutex> lock(m);

    for (int i = 0; i < pIndexes.size(); i++) {
        if (pIndexes[i] == _e) {
            pIndexes.erase(pIndexes.begin() + i);
            break;
        }
    }

    pIndexes.push_back(_e);

    //TODO clear it from debug
    check_priorities(_e);
}


int waiter::find(int _e) {

    for (int i = 0; i < pIndexes.size(); i++) {
        if (pIndexes[i] == _e) {
            return i;
        }
    }

    return -1;
}


void waiter::check_priorities(int _e) {
    //std::cout << _e << ": ( ";
    for (int e : pIndexes) {
        //std::cout << e << " ";
        if (checkPerm(e)) {
            //std::cout << "<-- can eat ";
            *(priors[e]) = true;
            cVars[e]->notify_one();
        }
        else {
            *(priors[e]) = false;
        }
    }
    //std::cout << ")" << std::endl;
}


void waiter::init() {
    bool prioritized = true;
    for (int e : pIndexes) {
        *(priors[e]) = prioritized;
        cVars[e]->notify_one();
        prioritized = !prioritized;
    }
}


void waiter::notifyAll() {
    for (int e : pIndexes) {
        cVars[e]->notify_one();
    }
}
