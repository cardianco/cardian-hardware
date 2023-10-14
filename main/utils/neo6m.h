#pragma once

#include <string>
#include <vector>

#include "attachable.h"

namespace cardian::gps {
class neo: public core::attachable {
public:
    enum NmeaType {
        UTC = 0,
        LATITUDE,
        LONGITUDE = 3,
        QUALITY_CODE = 5,
        SATELLITES_NUM,
        HORIZONTAL_DILUTION,
        ALTITUDE,
        MEAN_SEA_LEVEL = 10,
        DGPS_STATION_ID = 12,
        CHECKSUM,
        NONE = 0xF
    };

    std::vector<std::string> parseGpsNmeaMsg(const std::string &data) {
        std::vector<std::string> strList{};
        size_t e = 6;
        if(data.substr(0, 6) == "$GPGGA") {
            while(e < data.length()) {
                size_t s = e;
                e = data.find(',', e + 1);
                strList.push_back(data.substr(s + 1, e - s - 1));
            }
        }
        return strList;
    }
};
}
