#!/bin/bash

env -i DISPLAY=$DISPLAY QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt5/plugins/platforms \
    QT_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt5/plugins \
    PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin \
    LD_LIBRARY_PATH=/lib/x86_64-linux-gnu:/usr/lib/x86_64-linux-gnu:/lib64 \
    ./UsersManagement
