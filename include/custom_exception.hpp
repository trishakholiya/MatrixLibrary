#pragma once
#include <stdexcept>

/**
 * @class InvalidMatrixSize
 * @brief Exception thrown this error if the matrix size is invalid for specified operation.
 */
class InvalidMatrixSize : public std::domain_error {
public:
	InvalidMatrixSize(const std::string& what_arg) : std::domain_error(what_arg) 
    {}
};