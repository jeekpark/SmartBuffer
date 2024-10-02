#include <iostream>


#include "SmartBuffer.h"

int main()
{
    SmartBuffer buffer(10);
    buffer.Push(1);
    buffer.Push(2);
    buffer.Push(3);
    buffer.Push(4);
    buffer.Push(5);
    buffer.Push(6);
    buffer.Push(7);
    buffer.Push(8);
    buffer.Push(9);
    buffer.Push(10);
    std::cout << buffer.GetAverage() << std::endl;
    buffer.ResizeCapacity(20);
    return 0;
}