//
// Created by alex on 27.03.2021.
//

#include "tableware.h"


std::mutex* tableware::use(int index) {
    index_of_user = index;
    return &m;
}


tableware::tableware() {
    instance_count++;
    name = "F" + std::to_string(instance_count);
}


std::string tableware::getName() const {
    return name;
}


int tableware::instance_count = 0;


void tableware::returnT() {
    index_of_user = -1;
}


int tableware::getIndexOfUser() const {
    return index_of_user;
}
