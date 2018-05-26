# Wifi Scanner App for Linux

![Wifi Scanner](wifiscanner.png?raw=true "Wifi Scanner")


Wifi Scanner App for Linux, using Qt (https://www.qt.io/) and wifi-scan library (https://github.com/bmegli/wifi-scan)

The Wifi Scanner App is built for linux, and it shows a circle shaped window with the surrounding access point names.
The size of the arc in the circle and the darkness of the color indicates the signal strength of the Wifi AP.

## Building
The build script will run ```qmake``` to generate the ```Makefile``` and the run ```make``` to build the app:

```./build.sh```

### Some technical details
The qmake project file ```MeerkatWifiScannerLinux.pro``` contains the reference to be able to use ```libmnl``` which is needed to build the app.

```
QMAKE_CFLAGS += -lmnl
QMAKE_LIBS += -lmnl
```

If you don't have ```libmnl``` in your system, try the following:
```
sudo apt-get update && sudo apt-get install build-essential libmnl0 libmnl-dev
```

## Configuring
```./configure.sh```

This script will get the default wireless adapter name and saves it to ```MeerkatWifiScanner.config```.
The Wifi Scanner app reads this config file to determine which Wireless Adapter needs to be used.
This script also calls ```xhost``` to ensure that the app can be run as root.

## Running
```./run.sh```

The Wifi Scanner app will run as root. This is needed to be able to query the list of AP's.

## License
This Wifi Scanner App has MIT License (https://github.com/nlharri/WifiScannerLinux/blob/master/LICENSE).
The Wifi Scanner library which is built in the app, has Mozilla Public License 2.0 (https://github.com/bmegli/wifi-scan/blob/master/LICENSE).

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
