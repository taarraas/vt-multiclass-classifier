/* 
 * File:   MulticlassClassifier.h
 * Author: taras
 *
 * Created on 10 червня 2012, 9:57
 */

#ifndef _MULTICLASSCLASSIFIER_H
#define	_MULTICLASSCLASSIFIER_H
#include "BinaryClassifier.h"
#include <map>
#include <set>

template<class BinaryClassifier, class BinaryClassifierTrain>
class MulticlassOneByOneClassifier
{
    std::vector<Attributes> attrs_;
    std::vector<int> labels_;
    std::map<int, std::map<int, BinaryClassifier> > classifiers_;
    
public:
    MulticlassOneByOneClassifier(std::vector<Attributes> attrsData, std::vector<int> labels);
    int classify(Attributes attrs);
    
private:
    void initBinaryClassifier(int label1, int label2);
};

template<class BinaryClassifier, class BinaryClassifierTrain>
MulticlassOneByOneClassifier<BinaryClassifier, BinaryClassifierTrain>
        ::MulticlassOneByOneClassifier(
            std::vector<Attributes> attrsData,
            std::vector<int> labels)
        : attrs_(attrsData)
        , labels_(labels)
{
    std::set<int> labelSet;
    labelSet.insert(labels_.begin(), labels_.end());
    for (std::set<int>::iterator i = labelSet.begin(); i != labelSet.end(); i++)
    {
        std::set<int>::iterator j = i;
        j++;
        for (; j != labelSet.end(); j++)
        {
            initBinaryClassifier(*i, *j);
        }
    }
}

template<class BinaryClassifier, class BinaryClassifierTrain>
void MulticlassOneByOneClassifier<BinaryClassifier, BinaryClassifierTrain>
        ::initBinaryClassifier(int label1, int label2)
{
    std::vector<Attributes> attrsBin;
    std::vector<bool> labelBin;
    labelBin.reserve(attrs_.size());
    attrsBin.reserve(attrs_.size());

    for (size_t i = 0; i < attrs_.size(); i++)
    {
        int& label = labels_[i];
        if (label != label1 && label != label2)
            continue;

        attrsBin.push_back(attrs_[i]);
        labelBin.push_back(label == label2);
    }

    BinaryClassifierTrain classifierTrain(attrsBin, labelBin);
    classifierTrain.train();
    SVMData svmData;
    classifierTrain.getSVMData(svmData);

    classifiers_[label1].insert(std::pair<int, BinaryClassifier>(label2,BinaryClassifier(svmData)));
}

template<class BinaryClassifier, class BinaryClassifierTrain>
int MulticlassOneByOneClassifier<BinaryClassifier, BinaryClassifierTrain>
        ::classify(Attributes attrs)
{
    typename std::map<int, std::map<int, BinaryClassifier> >::const_iterator i;
    std::map<int, int> voteCountByLabel;
    i = classifiers_.begin();
    for (; i != classifiers_.end(); i++)
    {
        typename std::map<int, BinaryClassifier>::const_iterator j = i->second.begin();
        for (; j != i->second.end(); j++)
        {
            const BinaryClassifier& classifier = j->second;
            bool isJLabel = classifier.classify(attrs);
            int label = isJLabel ? j->first : i->first;
            voteCountByLabel[label]++; // if value wasn't exists it will be initialized as 0
        }
    }

    //choose label with most votes
    std::map<int, int>::const_iterator k = voteCountByLabel.begin();
    int bestLabel = -1;
    int bestVoteCount = 0;
    for(; k != voteCountByLabel.end(); k++)
    {
        if (bestVoteCount < k->second)
        {
            bestVoteCount = k->second;
            bestLabel = k->first;
        }
    }

    assert(bestVoteCount > 0);

    return bestLabel;
}

#endif	/* _MULTICLASSCLASSIFIER_H */

