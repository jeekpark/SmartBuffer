#pragma once

#include <new>
#include <exception>
#include <limits>

class SmartBuffer
{
public:
    class AllocationError : public std::exception
    {
    public:
        const char* what() const noexcept override
        {
            return "memory allocation error";
        }
    };
    class NewCapacityError : public std::exception
    {
    public:
        const char* what() const noexcept override
        {
            return "capacity error (hint: check param)";
        }
    };
    class EmptyBufferError : public std::exception
    {
    public:
        const char* what() const noexcept override
        {
            return "empty buffer error";
        }
    };

    SmartBuffer(size_t capacity)
        : mCapacity(capacity)
        , mSize(0)
        , mBuffer(nullptr)
    {
        if (capacity == 0)
        {
            throw NewCapacityError();
        }
        mBuffer = new (std::nothrow) int[capacity];
        if (mBuffer == nullptr)
        {
            throw AllocationError();
        }
    }
    ~SmartBuffer()
    {
        delete[] mBuffer;
    }
    void Push(int value)
    {
        if (mSize == mCapacity)
        {
            if (mCapacity > std::numeric_limits<size_t>::max() / 2)
            {
                throw AllocationError();
            }
            bool resizeResult = resizeCapacityNoException(mCapacity * 2);
            if (!resizeResult)
            {
                throw AllocationError();
            }
        }
        mBuffer[mSize] = value;
        ++mSize;
    }
    int PopOldest()
    {
        if (mSize == 0)
        {
            throw EmptyBufferError();
        }
        int value = mBuffer[0];
        shiftLeftByIndex(0);
        --mSize;
        return value;
    }
    int PopNewest()
    {
        if (mSize == 0)
        {
            throw EmptyBufferError();
        }
        int value = mBuffer[mSize - 1];
        --mSize;
        return value;
    }
    int PopMax()
    {
        if (mSize == 0)
        {
            throw EmptyBufferError();
        }
        size_t maxIndex = 0;
        for (int i = 1; i < mSize; ++i)
        {
            if (mBuffer[i] > mBuffer[maxIndex])
            {
                maxIndex = i;
            }
        }
        int value = mBuffer[maxIndex];
        shiftLeftByIndex(maxIndex);
        --mSize;
        return value;
    }
    int PopMin()
    {
        if (mSize == 0)
        {
            throw EmptyBufferError();
        }
        size_t minIndex = 0;
        for (int i = 1; i < mSize; ++i)
        {
            if (mBuffer[i] < mBuffer[minIndex])
            {
                minIndex = i;
            }
        }
        int value = mBuffer[minIndex];
        shiftLeftByIndex(minIndex);
        --mSize;
        return value;
    }
    double GetAverage()
    {
        if (mSize == 0)
        {
            throw EmptyBufferError();
        }
        double sum = 0.0;
        double compensation = 0.0;
        for (size_t i = 0; i < mSize; ++i)
        {
            double y = mBuffer[i] - compensation;
            double t = sum + y;
            compensation = (t - sum) - y;
            sum = t;
        }
        return sum / mSize;
    }
    void ResizeCapacity(size_t newCapacity)
    {
        if (newCapacity == 0)
        {
            throw NewCapacityError();
        }
        if (newCapacity < mSize)
        {
            throw NewCapacityError();
        }
        if (resizeCapacityNoException(newCapacity) == false)
        {
            throw AllocationError();
        }
    }

private:
    bool resizeCapacityNoException(size_t newCapacity) noexcept
    {
        int* newBuffer = new (std::nothrow) int[newCapacity];
        if (newBuffer == nullptr)
        {
            return false;
        }
        memcpy(newBuffer, mBuffer, mSize * sizeof(int));
        delete[] mBuffer;
        mBuffer = newBuffer;
        mCapacity = newCapacity;
        return true;
    }

    void shiftLeftByIndex(int index)
    {
        if (index >= mSize)
        {
            return;
        }
        for (size_t i = index; i < mSize - 1; ++i)
        {
            mBuffer[i] = mBuffer[i + 1];
        }
    }

    int* mBuffer;
    size_t mCapacity;
    size_t mSize;
};

