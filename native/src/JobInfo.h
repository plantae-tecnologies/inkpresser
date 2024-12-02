#ifndef JOB_INFO_H
#define JOB_INFO_H

#include <string>

struct JobInfo {
    int id;
    std::string printer;
    std::string document;
    std::string status;
    std::string user;
};

#endif
