
def rank(categoryResults):
    categoryRanks = rankCategories(categoryResults)
    
    categories = [c for c in categoryRanks.keys() if c != 'Overall']
    methods = categoryRanks['baseline'].keys()
    for method in methods:
        l = [categoryRanks[category][method].index for category in categories]
        categoryRanks['Overall'][method].avgOverallRanking = mean(l)
    
    return categoryRanks

def rankCategories(rawCategoryResults):
    categoryResults = resultsByCategoryMethod(rawCategoryResults)
    for category, data in categoryResults.items():
        computeRank(data)
    
    return categoryResults

def computeRank(data):
    methods = data.keys()
    
    lstRecall = sorted([data[method].recall for method in methods], reverse=True)
    lstSpecificity = sorted([data[method].specificity for method in methods], reverse=True)
    lstFPR = sorted([data[method].FPR for method in methods])
    lstFNR = sorted([data[method].FNR for method in methods])
    lstPBC = sorted([data[method].PBC for method in methods])
    lstFMeasure = sorted([data[method].fmeasure for method in methods], reverse=True)
    lstPrecision = sorted([data[method].precision for method in methods], reverse=True)
    
    ranks = {}
    for method in methods:
        categoryResult = data[method]
        rankRecall = lstRecall.index(categoryResult.recall)
        rankSpecificity = lstSpecificity.index(categoryResult.specificity)
        rankFPR = lstFPR.index(categoryResult.FPR)
        rankFNR = lstFNR.index(categoryResult.FNR)
        rankPBC = lstPBC.index(categoryResult.PBC)
        rankFMeasure = lstFMeasure.index(categoryResult.fmeasure)
        rankPrecision = lstPrecision.index(categoryResult.precision)
        
        avgRanking = (rankRecall + rankSpecificity + rankFPR + rankFNR + rankPBC + rankFMeasure + rankPrecision + 7) / 7.0
        
        ranks[method] = avgRanking
        categoryResult.avgRanking = avgRanking
        categoryResult.avgOverallRanking = 0.0
    
    sortedRanks = sorted(ranks.values())
    for method, avgRanking in ranks.items():
        data[method].index = sortedRanks.index(avgRanking) + 1.0



def resultsByCategoryMethod(results):
    dictResults = {}
    for r in results:
        category = r.category
        if category not in dictResults:
            dictResults[category] = {}
            
        dictResults[category][r.method] = r
    return dictResults

def mean(lst):
    return sum(lst) / len(lst)
