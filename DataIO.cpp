/* 
 * File:   DataIO.cpp
 * Author: taras
 * 
 * Created on 9 червня 2012, 13:23
 */

#include "DataIO.h"
#include <fstream>

void DataIO::load(std::string file,
            std::vector<Attributes>& attrs,
            std::vector<int>& labels)
{
    std::ifstream ifs(file.c_str());
    while (!ifs.eof())
    {
        Attributes a;
        for (int i = 0; i < Attributes::ATTRIBUTE_COUNT; i++)
        {
            ifs >> a.a[i];            
            ifs.ignore(1, ',');
        }

        std::string labelStr;
        ifs >> labelStr;
        
        if (ifs.eof())
            break;
        
        assert(labelStr.substr(0, 5) == "Label");
        int label = labelStr[5] - '0';

        attrs.push_back(a);
        labels.push_back(label);
    }
}

void DataIO::save(std::string file,
            const std::vector<Attributes>& attrs,
            const std::vector<int>& labels)
{
    assert(false);
}

