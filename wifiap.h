#ifndef WIFIAP_H
#define WIFIAP_H

#include <string>


class WifiAP {
    private:
        std::string name;
        int rssi;
        double ratio;
    public:
        WifiAP(std::string, int);
        std::string getName();
        int getRSSI();
        void setRSSI(int);
        // ratio to max RSSI of all the visible APs
        void setRatio(double);
        double getRatio();
        bool operator<(const WifiAP&);
};

#endif // WIFIAP_H
