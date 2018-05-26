#include <vector>
#include <iostream>
#include <algorithm>

#include "wifiapcollection.h"
#include "wifiap.h"

WifiAPCollection::WifiAPCollection() {

}

WifiAPCollection::~WifiAPCollection() {
    this->APs.clear();
}

void WifiAPCollection::addAPs(std::vector<WifiAP> APs) {

    this->APs.clear();
    for (auto it = APs.begin(); it != APs.end(); it++)
        this->APs.push_back(*it);

    // sort by RSSI ascending
    std::sort(this->APs.begin(), this->APs.end());

    // correct RSSI values
    std::vector<WifiAP> correctedVector;
    int index = 0;
    for (auto it = this->APs.rbegin(); it != this->APs.rend(); it++) {
        correctedVector.push_back(WifiAP((*it).getName(), this->APs[index].getRSSI()));
        index++;
    }

    this->APs.clear();
    for (auto it = correctedVector.begin(); it != correctedVector.end(); it++) {
        this->APs.push_back(*it);
    }

}

void WifiAPCollection::clearAPs() {
    this->APs.clear();
}

std::vector<WifiAP> WifiAPCollection::getAPs() {
    std::vector<WifiAP> returnVector;
    for (auto it = this->APs.begin(); it != this->APs.end(); it++)
        returnVector.push_back(*it);
    return returnVector;
}

