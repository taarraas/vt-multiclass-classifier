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
            std::vector<int>& labels,
            bool hasLabel)
{
    std::ifstream ifs(file.c_str());
    while (!ifs.eof())
    {
        Attributes a;
        
        if (!(ifs >> a.a[0]))
            break;
                
        for (int i = 1; i < Attributes::ATTRIBUTE_COUNT; i++)
        {
            ifs.ignore(1, ',');
            ifs >> a.a[i];            
        }                
        
        if (!hasLabel) {
            attrs.push_back(a);
            continue;
        }
            
        ifs.ignore(1, ',');        

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
    std::ofstream ofs(file.c_str());
    ofs.precision(1);
    ofs.setf(std::ios::fixed, std::ios::floatfield);
    for (size_t i=0; i < attrs.size(); i++)
    {
        for (size_t j=0; j < Attributes::ATTRIBUTE_COUNT; j++)
            ofs << attrs[i].a[j] << ",";
        
        ofs << "Label" << labels[i] << std::endl;
    }
}

