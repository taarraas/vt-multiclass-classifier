/* 
 * File:   Classifier.h
 * Author: taras
 *
 * Created on 9 червня 2012, 10:18
 */

#ifndef _CLASSIFIER_H
#define	_CLASSIFIER_H

#include <vector>
#include <assert.h>
#include <iostream>

static const double EPS = 1E-3;

struct Attributes
{
    enum { ATTRIBUTE_COUNT = 4 };
    double a[ATTRIBUTE_COUNT];
};

struct SVMData
{
    double w[Attributes::ATTRIBUTE_COUNT];
    double b;
};

class SVMClassifier
{
    const SVMData data_;
public:
    SVMClassifier(SVMData data)
            : data_(data)
    { };

    bool classify(const Attributes& attrs) const;
};

class LinearSVMHardMarginPolicy {
    const std::vector<Attributes>& x_;
    const std::vector<bool>& y_;
    double c_;
public:
    LinearSVMHardMarginPolicy(const std::vector<Attributes>& x,
                const std::vector<bool> y)
            : x_(x)
            , y_(y)
            , c_(100)
    { }

    void solve(std::vector<double>& lambda);
private:
    bool checkConstraints(const std::vector<double>& lambda);
    void getInitialSolution(std::vector<double>& lambda);
    double evaluateL(const std::vector<double>& lambda);
};

template<class TrainPolicy>
class SVMClassifierTrain
{
    TrainPolicy trainPolicy_;
    std::vector<Attributes> x_;
    std::vector<bool> y_;
    SVMData data_;
    bool isTrained_;
public:
    SVMClassifierTrain(const std::vector<Attributes>& x, 
                       const std::vector<bool>& y)
            : x_(x)
            , y_(y)
            , trainPolicy_(x, y)
            , isTrained_(false)
    { };

    void train();

    void getSVMData(SVMData& data)
    {
        assert(isTrained_);
        data = data_;
    }

};

// see http://en.wikipedia.org/wiki/Support_vector_machine
// for description of used formulas
template<class TrainPolicy>
void SVMClassifierTrain<TrainPolicy>::train()
{
    //calculate lambda
    std::vector<double> lambda;
    trainPolicy_.solve(lambda);

    std::cout << "[SVMClassifierTrain] Lambda = ";
    for (int j = 0; j < x_.size(); j++)
        std::cout << lambda[j] << ", ";
    std::cout << std::endl;


    std::cout << "[SVMClassifierTrain] w = ";
    //evaluate vector w
    for (int i = 0; i < Attributes::ATTRIBUTE_COUNT; i++)
    {
        double wi = 0;
        for (int j = 0; j < x_.size(); j++)
            wi += lambda[j] * x_[j].a[i] * (y_[j] ? 1 : -1);

        data_.w[i] = wi;
        std::cout << wi;
    }
    std::cout << std::endl;

    //evaluate b as an average
    double bSum = 0;
    int bCnt = 0;
    for (int i = 0; i < x_.size(); i++)
    {
        if (lambda[i] < EPS)
            continue;

        double bi = 0;
        for (int j = 0; j < Attributes::ATTRIBUTE_COUNT; j++)
            bi += data_.w[j] * x_[i].a[j];

        bi -= y_[i] ? 1 : -1;
        bCnt++;
        bSum += bi;
    }
    data_.b = bSum / bCnt;
    std::cout << "[SVMClassifierTrain] b = " << data_.b << std::endl;
    isTrained_ = true;
};

#endif	/* _CLASSIFIER_H */

