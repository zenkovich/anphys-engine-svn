#ifndef PARAMETER_CONNECTOR_H
#define PARAMETER_CONNECTOR_H

#include "parameter_container.h"

/** ���������, ��������� ��������� ��������� � ��������.
  * ��� �������� ������� ��������. */
template<typename object> struct cParameterConnector
{
	cParameterContainer<object> mObjectToConnect;   /**< ��������� ���������. */
	cParameterContainer<object> mObjectFromConnect; /**< �������� ���������. */

	/** �����, � ������� �������� ���������� ���������� ������������� �������� ���������. */
	inline void update() { mObjectToConnect = mObjectFromConnect; }
};


#endif