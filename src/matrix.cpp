#include "matrix.h"

void GLEngine::InvalidRowMetricsException::LogExceptionMessage()
{
    Debug->Log(this->Message);
}

void GLEngine::InvalidRowLengthException::LogExceptionMessage()
{
    Debug->Log(this->Message);
}

void GLEngine::InvalidRowStrideException::LogExceptionMessage()
{
    Debug->Log(this->Message);
}