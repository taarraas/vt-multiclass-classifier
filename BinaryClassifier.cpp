/* 
 * File:   Classifier.cpp
 * Author: taras
 * 
 * Created on 9 червня 2012, 10:18
 */

#include <cmath>
#include <iostream>
#include "BinaryClassifier.h"


bool SVMClassifier::classify(const Attributes& attrs) const
{
    double res = 0;
    for (int i = 0; i < Attributes::ATTRIBUTE_COUNT; i++)
        res += attrs.a[i] * data_.w[i];
    
    res -= data_.b;
    return res > 0;
};

void LinearSVMHardMarginPolicy::solve(std::vector<double>& lambda)
{
    getInitialSolution(lambda);
    assert(checkConstraints(lambda));

    double lVal = evaluateL(lambda);
    double step = 2;
    
    while (step > EPS)
    {
        bool changed = false;
        for (size_t i = 0; i < x_.size(); i++)
        {
#ifdef DEBUG
            std::cout << "i = " << i << "/" << x_.size() << std::endl;
            for (size_t j = 0; j < x_.size(); j++)
                std::cout << lambda[j] << ", ";
            std::cout << std::endl;
#endif
            for (size_t j = 0; j < x_.size(); j++)
            {
                if (y_[i] != y_[j])
                {
                    bool locChanged = true;
                    while (locChanged)
                    {
                        locChanged = false;
                        std::vector<double> nl(lambda);
                        nl[i] += step;
                        nl[j] += step;
                        if (!checkConstraints(nl))
                            continue;

                        double nVal = evaluateL(nl);
                        if (nVal + EPS < lVal)
                        {
                            lambda = nl;
                            lVal = nVal;
                            changed = true;
                            locChanged = true;
#ifdef DEBUG
                            std::cout << "L1 = " << lVal << std::endl;
#endif
                        }
                    }

                    locChanged = true;
                    while (locChanged)
                    {
                        locChanged = false;
                        std::vector<double> nl(lambda);
                        nl[i] -= step;
                        nl[j] -= step;
                        if (!checkConstraints(nl))
                            continue;

                        double nVal = evaluateL(nl);
                        if (nVal + EPS < lVal)
                        {
                            lambda = nl;
                            lVal = nVal;
                            changed = true;
                            locChanged = true;
#ifdef DEBUG
                            std::cout << "L2 = " << lVal << std::endl;
#endif
                        }
                    }

                } else {
                    bool locChanged = true;
                    while (locChanged)
                    {
                        locChanged = false;
                        std::vector<double> nl(lambda);
                        nl[i] -= step;
                        nl[j] += step;
                        if (!checkConstraints(nl))
                            continue;

                        double nVal = evaluateL(nl);
                        if (nVal + EPS < lVal)
                        {
                            lambda = nl;
                            lVal = nVal;
                            changed = true;
                            locChanged = true;
#ifdef DEBUG
                            std::cout << "L3 = " << lVal << std::endl;
#endif

                        }
                    }
                }

            }
        }


        std::cout << "[GradientOptimization] iteration ended" << std::endl;
        if (!changed)
        {
            step *= 0.5;
            std::cout << "[GradientOptimization] STEP = " << step << std::endl;
        }
    }
}

bool LinearSVMHardMarginPolicy::checkConstraints(const std::vector<double>& lambda)
{
    double sum = 0;
    for (size_t i=0; i < lambda.size(); i++) {
        if (lambda[i] < 0)
            return false;

        if (lambda[i] > c_)
            return false;

        sum += lambda[i] * (y_[i] ? 1 : -1);
    }
    return std::abs(sum) < EPS;
}

void LinearSVMHardMarginPolicy::getInitialSolution(std::vector<double>& lambda)
{
    lambda.resize(x_.size());
    int cntPos = 0;
    for (size_t i = 0; i < x_.size(); i++)
        if (y_[i])
            cntPos++;

    double c = 100; //sum of lambda near both positive and negative y will be c
    double lPos = c / cntPos;
    double lNeg = c / (x_.size() - cntPos);
    for (size_t i = 0; i < x_.size(); i++)
        if (y_[i])
            lambda[i] = lPos;
        else
            lambda[i] = lNeg;
}

double LinearSVMHardMarginPolicy::evaluateL(const std::vector<double>& lambda)
{
    double l1 = 0;
    for (size_t i = 0; i < x_.size(); i++)
        l1 -= lambda[i];

    double l2 = 0;
    for (size_t i = 0; i < x_.size(); i++)
        for (size_t j = 0; j < x_.size(); j++)
        {
            double xDotProd = 0;
            for (size_t k = 0; k < Attributes::ATTRIBUTE_COUNT; k++)
                xDotProd += x_[i].a[k] * x_[j].a[k];
                
            l2 += lambda[i] * lambda[j] * (y_[i] ? 1 : -1) * (y_[j] ? 1 : -1) * xDotProd;
        }

    return l1 + 0.5 * l2;
}
