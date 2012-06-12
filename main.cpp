/* 
 * File:   main.cpp
 * Author: taras
 *
 * Created on 9 червня 2012, 10:12
 */

#include <stdlib.h>
#include <iostream>
#include "DataIO.h"
#include "MulticlassClassifier.h"

/*
 * 
 */
int main(int argc, char** argv)
{
    std::vector<Attributes> trainAttrs;
    std::vector<int> trainLabels;
    DataIO::load("test1/train.data", trainAttrs, trainLabels, true);
    
    MulticlassOneByOneClassifier< SVMClassifier, SVMClassifierTrain<LinearSVMSoftMarginPolicy> > mc(trainAttrs, trainLabels);

    std::cout << "[main] result on train set :" << std::endl;
    for (size_t i = 0; i < trainAttrs.size(); i++)
    {
        int label = mc.classify(trainAttrs[i]);
        std::cout << "Classified as : " << label << ". Expected : " << trainLabels[i] << std::endl;
    }

    std::vector<Attributes> validateAttrs;
    std::vector<int> validateLabels;
    DataIO::load("test1/cross-validate.data", validateAttrs, validateLabels);
    std::cout << std::endl << "[main] result on cross-validate set :" << std::endl;
    int TP = 0;
    int FP = 0;
    int TN = 0;
    int FN = 0;
    for (size_t i = 0; i < validateAttrs.size(); i++)
    {
        int label = mc.classify(validateAttrs[i]);
        int expected = validateLabels[i];
        std::cout << "Classified as : " << label << ". Expected : " << expected << std::endl;
        if (label == expected)
        {
            TP++;
            TN += 2;
        } else {
            FP++;
            TN++;
            FN++;
        }
    }
    std::cout << "[main] Precision : " << (double)TP / (TP + FP) << std::endl;
    std::cout << "[main] Accuracy : " << (double)(TP + TN) / (TP + FP + TN + FN) << std::endl;
    
    std::vector<Attributes> testAttrs;
    std::vector<int> testLabels;
    DataIO::load("test1/test.data", testAttrs, testLabels, false);
    testLabels.reserve(testAttrs.size());
    std::cout << std::endl << "[main] result on test set :" << std::endl;
    for (size_t i = 0; i < testAttrs.size(); i++)
    {
        int label = mc.classify(testAttrs[i]);
        std::cout << "Classified as : " << label << std::endl;
        testLabels.push_back(label);
    }
    DataIO::save("test.data.out", testAttrs, testLabels);    
}