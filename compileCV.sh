
#!/bin/bash
# usage-message.sh
if [ -z "$1" ]; then
        echo "Source filename missing"
        exit 1
elif [ -z "$2" ]; then
        echo "Output filename missing"
        exit 1
fi

#g++ $1 -o $2 `pkg-config --cflags --libs opencv` -lopencv_gpu -L /usr/lib
g++ -Wall -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L /usr/local/lib/ -g -o $2 $1 -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching
