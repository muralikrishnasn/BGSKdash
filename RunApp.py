#BEFORE RUNNING THIS RUN DirList.java TO CREATE MODEL FOLDERS
#CHANGE ./kdash_texture to ./kdash.out to tun color based BGS
import numpy as np
#import cv2
import subprocess
from itertools import izip
import string

with open("InputDirPath.txt") as textfile1:
   #with open("ModelDirPath.txt") as textfile2:
   #for x, y in izip(textfile1, textfile2):
   for x in textfile1:
       x = x.rstrip()
       y = x.replace("input","model/");
       y=y.rstrip();
       #print y
       print ("Training Phase\n");
       arg =['./kdash.out','training',x+'/in%06d.jpg',y+'/bgmodel.txt'];
       #print arg
       #str1 = ''.join(arg);
       #print str1
       #subprocess.call(arg);
       
       print ("Testing phase \n");
       z=string.replace(x,"/dataset/","/results/");
       z=string.replace(z,"/input","/");
       z=z.rstrip();
       arg= ['./kdash.out', x +"/in%06d.jpg",y+"bgmodel.txt",z];
#       str1=''.join(arg)
    #   print str1
       print arg
       subprocess.call(arg);


#---------------------------------------------------------------
#arg=['TestOpenCV' , 'training', #'D:/CodeBlockOpenCV/TestProj/TestOpenCV/Dataset/CDNetDataset/da#taset2014/dataset/badWeather/blizzard/input', #'D:/CodeBlockOpenCV/TestProj/TestOpenCV/Dataset/CDNetDataset/da#taset2014/model/badWeather/blizzard/model/MyModel.txt']
#print arg
#subprocess.call(arg);
#--------------------------------------------------------------
