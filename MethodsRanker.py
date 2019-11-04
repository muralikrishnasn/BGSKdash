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

from Result import Result, createResultsFromMethod, createResultsFromFolder
from resultsCalculator import videoResults2categoryResults

__author__ = "Nil Goyette"
__copyright__ = "Copyright 2012, Universite de Sherbrooke"

def main():
    # Get the results from gmm*.txt file in the gmm folder
    rawResults = createResultsFromMethod('C:\\Users\\Nil\\Desktop\\Results', 'gmm')

    # Get the results from all txt files in the gmm folder
    # rawResults = createResultsFromFolder('C:\\Users\\Nil\\Desktop\\Results\\gmm')

    # Convert the results to Category Results, which I can sort
    stats = videoResults2categoryResults(rawResults)

    l = []
    for method, result in stats['Overall'].items():
        l.append( (result.avgOverallRanking, method) )

    l.sort()
    for t in l: # If you have a lot of results, replace with l[:15] or any low number
        print(t)

if __name__ == "__main__":
    main()
