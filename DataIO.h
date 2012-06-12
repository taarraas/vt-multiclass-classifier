/* 
 * File:   DataIO.h
 * Author: taras
 *
 * Created on 9 червня 2012, 13:23
 */

#ifndef _DATAIO_H
#define	_DATAIO_H

#include "BinaryClassifier.h"
#include <string>

class DataIO {
public:
    static void load(std::string file,
            std::vector<Attributes>& attrs,
            std::vector<int>& labels,
            bool hasLabel = true);
    
    static void save(std::string file,
            const std::vector<Attributes>& attrs,
            const std::vector<int>& labels);
};

#endif	/* _DATAIO_H */

