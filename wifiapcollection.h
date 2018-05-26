#ifndef WIFIAPCOLLECTION_H
#define WIFIAPCOLLECTION_H

#include <vector>
#include <string>

#include "wifiap.h"

class WifiAPCollection {
    private:
        std::vector<WifiAP> APs;
    public:
        WifiAPCollection();
        ~WifiAPCollection();
        void addAPs(std::vector<WifiAP> APs);
        void clearAPs();
        std::vector<WifiAP> getAPs();
};

#endif // WIFIAPCOLLECTION_H
