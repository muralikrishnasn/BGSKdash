
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

NBSTATS = 7
(RECALL, SPECIFICITY, FPR, FNR, PBC, FMEASURE, PRECISION) = range(NBSTATS)

from rank import rank

__author__ = "Nil Goyette"
__copyright__ = "Copyright 2012, Universite de Sherbrooke"

class CategoryResult():
    def __init__(self, method, category):
        self.method = method
        self.category = category
        
        self.avgOverallRanking = 0.0
        self.avgRanking = 0.0
        
        self.recall = 0.0
        self.specificity = 0.0
        self.FPR = 0.0
        self.FNR = 0.0
        self.PBC = 0.0
        self.fmeasure = 0.0
        self.precision = 0.0

def videoResults2categoryResults(resultsByVideo):
    categoryResults = []
    
    # Categories
    resultsByCategory = {}
    for result in resultsByVideo:
        category = result.category
        if category not in resultsByCategory:
            resultsByCategory[category] = {}
        
        method = result.method
        if method not in resultsByCategory[category]:
            resultsByCategory[category][method] = [[] for i in range(NBSTATS)]
    
        fillListWithVideoResults(resultsByCategory[category][method], result)
    
    overallLists = [[] for i in range(NBSTATS)]
    for category, methodInfo in resultsByCategory.items():
        for method, lists in methodInfo.items():
            categoryResult = CategoryResult(method, category)
            fillCategoryResult(categoryResult, lists)
            categoryResults.append(categoryResult)
            
            fillListWithCategoryResults(overallLists, categoryResult)
    
    # Overall
    for method in resultsByCategory['baseline'].keys():
        overallResult = CategoryResult(method, 'Overall')
        fillCategoryResult(overallResult, overallLists)
        categoryResults.append(overallResult)
    
    # Each new method will change the ranking
    return rank(categoryResults)

def fillCategoryResult(result, lists):
    """Fill a CategoryResult with a list of lists."""
    result.avgOverallRanking = 0.0
    result.avgRanking = 0.0
    result.recall = mean(lists[RECALL])
    result.specificity = mean(lists[SPECIFICITY])
    result.FPR = mean(lists[FPR])
    result.FNR = mean(lists[FNR])
    result.PBC = mean(lists[PBC])
    result.fmeasure = mean(lists[FMEASURE])
    result.precision = mean(lists[PRECISION])

def fillListWithVideoResults(l, result):
    """Append a stats list with a CategoryResult."""
    l[RECALL].append(result.Recall())
    l[SPECIFICITY].append(result.Specificity())
    l[FPR].append(result.FPR())
    l[FNR].append(result.FNR())
    l[PBC].append(result.PBC())
    l[FMEASURE].append(result.FMeasure())
    l[PRECISION].append(result.Precision())

def fillListWithCategoryResults(l, result):
    """Append a stats list with a CategoryResult."""
    l[RECALL].append(result.recall)
    l[SPECIFICITY].append(result.specificity)
    l[FPR].append(result.FPR)
    l[FNR].append(result.FNR)
    l[PBC].append(result.PBC)
    l[FMEASURE].append(result.fmeasure)
    l[PRECISION].append(result.precision)

def mean(lst):
    return sum(lst) / len(lst)
