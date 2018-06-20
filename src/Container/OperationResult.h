/*
 * ContainerOperationResult.h
 *
 *  Created on: Jun 20, 2018
 *      Author: xgallom
 */

#ifndef CONTAINER_OPERATIONRESULT_H_
#define CONTAINER_OPERATIONRESULT_H_

namespace Container {

template<typename T>
struct OperationResult {
	explicit OperationResult(bool isValid = false) : isValid(isValid) {}
	explicit OperationResult(T value) : isValid(true), value(value) {}

	bool isValid;
	T value;
};

} /* namespace Container */

#endif /* CONTAINER_OPERATIONRESULT_H_ */
