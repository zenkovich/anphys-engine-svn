#ifndef PARAMETER_COMPLEX_CONNECTOR_H
#define PARAMETER_COMPLEX_CONNECTOR_H

#include "parameter_connector.h"

/** Описание комплексного контейнера. 
  * В нем могут перечислятся несколько различных контейнеров. */
struct cParameterComplexConnector
{
	virtual void update() = 0;
};


#endif //PARAMETER_COMPLEX_CONNECTOR_H