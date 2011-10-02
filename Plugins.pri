#
# LCDHost API5 compatiblity layer
#

CONFIG += lh_plugin
hid: CONFIG += lh_hidapi
usb: CONFIG += lh_libusb
load($$PWD/PluginsConfig.prf)
