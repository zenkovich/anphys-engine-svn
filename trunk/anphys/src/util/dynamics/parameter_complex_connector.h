#ifndef PARAMETER_COMPLEX_CONNECTOR_H
#define PARAMETER_COMPLEX_CONNECTOR_H

#include "parameter_connector.h"

/** �������� ������������ ����������. 
  * � ��� ����� ������������ ��������� ��������� �����������. */
struct cParameterComplexConnector
{
	virtual void update() = 0;
};


#endif //PARAMETER_COMPLEX_CONNECTOR_H