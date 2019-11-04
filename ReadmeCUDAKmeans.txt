g++ -Wall -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L /usr/local/lib/ -c -g main.cpp Kdashmeans.cpp Kdashmeans_modfied.cpp KdashTest.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching

Inside kmeans folder run : make ; this contains the isntruction for comipiling cuda kmeans


 g++ -Wall -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L /usr/local/lib/ -o kdash.out -g main.cpp Kdashmeans.cpp Kdashmeans_modfied.cpp KdashTest.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching ./kmeans/labels.o ./kmeans/test.o ./kmeans/timer.o -L /usr/local/cuda-8.0/lib64/ -lcublas
