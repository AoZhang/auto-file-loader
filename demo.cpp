// @file demo.cpp, 11/09/2015
// Copyright (c) 2015 ZhangAo, Inc. All rights reserved
//
// @author  Zhang Ao, zhangaoup@aliyun.com
// @brief   demo of using auto-file-loader
// @version 1.0

#include "auto_file_loader.h"
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <map>
#include <string>

// type of data to be loaded
typedef std::map<std::string, int> load_data_type_t;

struct loader_conf_t {
    int hash_size;
};

int parse_line(const char* line_input, load_data_type_t* data) {
    std::string line = line_input;
    size_t sep_pos = line.find('\t');
    if (sep_pos == std::string::npos) {
        return -1;
    }
    std::string key = line.substr(0, sep_pos);
    int value = strtol(line.substr(sep_pos + 1).c_str(), NULL, 10);

    (*data)[key] = value;
    return 0;
}

int create(load_data_type_t& data, loader_conf_t& conf) {
    return 0;
}

void free(load_data_type_t& data) {
    data.clear();
}

int main(int argc, const char *argv[])
{
    loader_conf_t lconf;
    lconf.hash_size = 100;
    AutoFileLoader<load_data_type_t, loader_conf_t> afl("afl_test", "data/test.in", create, free, parse_line, lconf);
    int ret = afl.init();
    if (ret != 0) {
        std::cerr << "init AutoFileLoader failed with code " << ret << std::endl;
        return -1;
    }

    while (true) {
        load_data_type_t* data = afl.get();
        if (data == NULL) {
            std::cerr << "get loader buffer errro!" << std::endl;
            break;
        }
        std::cerr << "print loaded data:" << std::endl;
        std::cerr << "data size: " << data->size() << std::endl;
        for (load_data_type_t::iterator it = data->begin();
                it != data->end();
                ++it) {
            std::cout << it->first << " |\\t| " << it->second << std::endl;
            std::cout << std::flush;
        }
        sleep(10);
    }
    return 0;
}
