#BEFORE RUNNING THIS RUN DirList.java TO CREATE MODEL FOLDERS
#CHANGE ./kdash_texture to ./kdash.out to tun color based BGS
import numpy as np
import cv2
import subprocess
from itertools import izip
import string
import os

with open("fileList.txt") as textfile1:
   #with open("ModelDirPath.txt") as textfile2:
   #for x, y in izip(textfile1, textfile2):
   for x in textfile1:
       print "Training phase \n";
       x=x.rstrip();
       y=x.replace("dataset","model");
       y=y.rstrip();
       y=y+"/";
      
       x=x.rstrip(); 
       os.makedirs(y);
       arg= ['./kdash.out','training', x +"/%05d.jpg",y+"KTHbgmodel.txt"];
       subprocess.call(arg);

       print "Testing phase \n";
       z=x.replace("dataset","result");
       z=z+"/";
       z=z.rstrip();
       x=x.rstrip();
       os.makedirs(z)
       

      # arg= ['./kdash.out', x +"/%05d.jpg",y+"/KTHbgmodel.txt",z];
       arg= ['./kdash.out', x +"/%05d.jpg",y+"KTHbgmodel.txt",z];
       #str1=''.join(arg)
       #print str1
   #    print arg
       subprocess.call(arg);


#---------------------------------------------------------------
#arg=['TestOpenCV' , 'training', #'D:/CodeBlockOpenCV/TestProj/TestOpenCV/Dataset/CDNetDataset/da#taset2014/dataset/badWeather/blizzard/input', #'D:/CodeBlockOpenCV/TestProj/TestOpenCV/Dataset/CDNetDataset/da#taset2014/model/badWeather/blizzard/model/MyModel.txt']
#print arg
#subprocess.call(arg);
#--------------------------------------------------------------
