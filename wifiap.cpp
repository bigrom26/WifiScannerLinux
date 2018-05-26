#include <string>

#include "wifiap.h"



WifiAP::WifiAP(std::string name, int rssi) {
    this->name = name;
    this->rssi = rssi;
}

std::string WifiAP::getName() {
    return this->name;
}

int WifiAP::getRSSI() {
    return this->rssi;
}

void WifiAP::setRSSI(int rssi) {
    this->rssi = rssi;
}

void WifiAP::setRatio(double ratio) {
    this->ratio = ratio;
}

double WifiAP::getRatio() {
    return this->ratio;
}

bool WifiAP::operator<(const WifiAP& ap) {
    return this->rssi < ap.rssi;
}
