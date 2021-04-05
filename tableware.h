//
// Created by alex on 27.03.2021.
//

#ifndef PHILOSOPHERS_TABLEWARE_H
#define PHILOSOPHERS_TABLEWARE_H

#include <mutex>
#include <string>
#include <unistd.h>

class tableware {

public:

    tableware();

    std::mutex* use(int index);

    [[nodiscard]] std::string getName() const;

    [[nodiscard]] int getIndexOfUser() const;

    void returnT();

private:

    std::mutex m;

    std::string name;

    int index_of_user = -2;

    static int instance_count;
};


#endif //PHILOSOPHERS_TABLEWARE_H
