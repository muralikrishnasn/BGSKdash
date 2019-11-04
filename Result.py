#!/usr/bin/env python

#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
#AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
#FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
#OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import glob
import os

__author__ = "Nil Goyette"
__copyright__ = "Copyright 2012, Universite de Sherbrooke"

class Result:
    def __init__(self, method, video, category, TP, FP, FN, TN):
        self.method = method
        self.video = video
        self.category = category
        self.TP = TP
        self.FP = FP
        self.FN = FN
        self.TN = TN

    def Recall(self):
        if self.TP + self.FN == 0:
            return 1
        else:
            return float(self.TP) / (self.TP + self.FN)
    
    def Specificity(self):
        if self.TN + self.FP == 0:
            return 1
        else:
            return float(self.TN) / (self.TN + self.FP)
    
    def FPR(self):
        if self.FP + self.TN == 0:
            return 0
        else:
            return float(self.FP) / (self.FP + self.TN)
    
    def FNR(self):
        if self.TP + self.FN == 0:
            return 0
        else:
            return float(self.FN) / (self.TP + self.FN)
    
    def PBC(self):
        if self.TP + self.FN + self.FP + self.TN == 0:
            return 0
        else:
            return 100.0 * (self.FN + self.FP) / (self.TP + self.FN + self.FP + self.TN)
    
    def Precision(self):
        if self.TP + self.FP == 0:
            return 0.0
        else:
            return float(self.TP) / (self.TP + self.FP)
    
    def FMeasure(self):
        recall = self.Recall()
        precision = self.Precision()
        if recall + precision == 0:
            return 0
        else:
            return (2.0 * recall * precision) / (recall + precision)

    def __str__(self):
        return 'Result (' + self.method + ', ' + self.video + ')'

def createResultsFromMethod(path, method):
    resultsList = []
    for file in glob.glob(os.path.join(path, method, method + '*.txt')):
        __addResultFile(file, resultsList)
    
    return resultsList

def createResultsFromFolder(path):
    resultsList = []
    for file in glob.glob(os.path.join(path, '*.txt')):
        __addResultFile(file, resultsList)

    return resultsList

def __addResultFile(file, resultsList):
    methodName = file.replace('\\', '/').split('/')[-1].split('.txt')[0]
    with open(file) as f:
        for line in f.readlines():
            if line.startswith('cm video'):
                elems = line.split()
                category, video = elems[2:4]
                TP, FP, FN, TN = [int(nb) for nb in elems[4:8]]

                resultsList.append(Result(methodName, video, category, TP=TP, FP=FP, FN=FN, TN=TN))
