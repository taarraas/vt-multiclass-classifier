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
    DataIO::load("test1/train.data", trainAttrs, trainLabels);

    MulticlassOneByOneClassifier< SVMClassifier, SVMClassifierTrain<LinearSVMHardMarginPolicy> > mc(trainAttrs, trainLabels);

    std::cout << "[main] result on train set :" << std::endl;
    for (size_t i = 0; i < trainAttrs.size(); i++)
    {
        int label = mc.classify(trainAttrs[i]);
        std::cout << "Classified as : " << label << ". Expected : " << trainLabels[i] << std::endl;
    }

    std::vector<Attributes> testAttrs;
    std::vector<int> testLabels;
    DataIO::load("test1/cross-validate.data", testAttrs, testLabels);
    std::cout << std::endl << "[main] result on test set :" << std::endl;
    for (size_t i = 0; i < testAttrs.size(); i++)
    {
        int label = mc.classify(testAttrs[i]);
        std::cout << "Classified as : " << label << ". Expected : " << testLabels[i] << std::endl;
    }

}


#if 0
int main(int argc, char** argv) {
    std::vector<Attributes> trainAttrs;
    std::vector<int> trainLabels;
    DataIO::load("test1/train.data", trainAttrs, trainLabels);
    
    std::vector<bool> trainIs1(trainLabels.size(), false);
    for (size_t i = 0; i < trainLabels.size(); i++)
        trainIs1[i] = trainLabels[i] == 3;

    SVMData svmData;

    SVMClassifierTrain<LinearSVMHardMarginPolicy> trainer(trainAttrs, trainIs1);
    trainer.train();
    trainer.getSVMData(svmData);


    std::cout << "Train data : " << std::endl;
    SVMClassifier svmClassifier(svmData);
    for (size_t i = 0; i < trainAttrs.size(); i++)
    {
        bool isOne = svmClassifier.classify(trainAttrs[i]);
        std::cout << isOne << " <- " << trainIs1[i] << std::endl;
    }

    std::vector<Attributes> testAttrs;
    std::vector<int> testLabels;
    std::cout << "Cross validate : " << std::endl;
    DataIO::load("test1/cross-validate.data", testAttrs, testLabels);
    for (size_t i = 0; i < testAttrs.size(); i++)
    {
        bool isOne = svmClassifier.classify(testAttrs[i]);
        std::cout << isOne << " <- " << (testLabels[i] == 3) << std::endl;
    }

    
    return (EXIT_SUCCESS);
}
#endif