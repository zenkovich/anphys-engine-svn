#ifndef PARAMETER_CONNECTOR_H
#define PARAMETER_CONNECTOR_H

#include "parameter_container.h"

/**  оннектор, св€зывает зависимый контейнер с исходным.
  * ƒл€ быстрого доступа значений. */
template<typename object> struct cParameterConnector
{
	cParameterContainer<object> mObjectToConnect;   /**< «ависимый контейнер. */
	cParameterContainer<object> mObjectFromConnect; /**< »сходных контейнер. */

	/** ћетод, в котором значение завис€щего контейнера присваиваетс€ значению исходного. */
	inline void update() { mObjectToConnect = mObjectFromConnect; }
};


#endif