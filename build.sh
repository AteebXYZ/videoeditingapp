cd build
rm -rf * 
cmake .. -DCMAKE_PREFIX_PATH="/usr/local/lib/cmake/KDDockWidgets-qt6"
make -j12