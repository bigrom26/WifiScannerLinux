#include <QApplication>
#include <iostream>
#include <chrono>
#include <future>
#include <mutex>
#include <vector>
#include <fstream>

#include "wifi_scan.h"
#include "shapeddashboard.h"
#include "wifiapcollection.h"
#include "wifiap.h"


std::mutex mu;

void wifiScannerThread(std::future<void> futureObj, std::string WifiIfName, WifiAPCollection *wifiAPCollection);


int main(int argc, char *argv[]) {
    // read config file
    std::ifstream confFile;
    std::string wifiIfName;
    confFile.open("./MeerkatWifiScanner.conf");
    if (!confFile) {
        std::cerr << "Unable to open file MeerkatWifiScanner.conf";
        return -1;
    }
    confFile >> wifiIfName;
    confFile.close();
    if (wifiIfName.empty()) {
        std::cerr << "Unable to get Wifi Interface name from config file MeerkatWifiScanner.conf";
        return -1;
    }

    // define AP Collection object as volatile to be able to manipulate it from the thread
    WifiAPCollection* volatile wifiAPCollection = new WifiAPCollection();

    //-----------------------------------------------
    // thread stuff >>>
    // Create a std::promise object
    std::promise<void> exitSignal;

    //Fetch std::future object associated with promise
    std::future<void> futureObj = exitSignal.get_future();

    // Starting Thread & move the future object in lambda function by reference
    std::thread th(&wifiScannerThread, std::move(futureObj), wifiIfName, wifiAPCollection);
    // <<< end of thread stuff
    //-----------------------------------------------

    // initialize Qt application
    QApplication app(argc, argv);
    ShapedDashboard dashBoard;
    dashBoard.setWifiAPCollection(wifiAPCollection);
    dashBoard.show();
    int app_exec_code = app.exec();

    //-----------------------------------------------
    // thread stuff >>>
    //Set the value in promise
    exitSignal.set_value();

    //Wait for thread to join
    th.join();
    // <<< end of thread stuff
    //-----------------------------------------------

    delete wifiAPCollection;

    return app_exec_code;
}

void wifiScannerThread(std::future<void> futureObj, std::string WifiIfName, WifiAPCollection *wifiAPCollection) {
    int maxNumberOfAPs = 6; // maximal number of APs scanned
    int refreshTimeInMillis = 3000; // scan frequency

    while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
        std::this_thread::sleep_for(std::chrono::milliseconds(refreshTimeInMillis));

        int status, i;
        struct bss_info bss[maxNumberOfAPs];
        struct wifi_scan *wifi = wifi_scan_init(WifiIfName.c_str());

        status=wifi_scan_all(wifi, bss, maxNumberOfAPs);

        std::vector<WifiAP> APs;
        for (i=0;i<status && i<maxNumberOfAPs;++i)
            APs.push_back(WifiAP(bss[i].ssid, bss[i].signal_mbm/100));

        mu.lock();
        wifiAPCollection->addAPs(APs);
        mu.unlock();

        wifi_scan_close(wifi);

    }
}

