#pragma once
class SmartBuffer
{
public:
    SmartBuffer(int capacity)
        : mCapacity(capacity)
        , mSize(0)
    {
        if (mCapacity <= 0)
        {
            throw "Capacity must be greater than 0";
        }
        mBuffer = new int[mCapacity];
    }

    ~SmartBuffer()
    {
        delete[] mBuffer;
    }

    void Push(int value)
    {
        if (mSize == mCapacity)
        {
            resize(mCapacity * 2);
        }
        mBuffer[mSize] = value;
        ++mSize;
    }

    int PopOldest()
    {
        if (mSize == 0)
        {
            throw "Buffer is empty";
        }
        int value = mBuffer[0];
        shiftLeft(0);
        --mSize;
        return value;
    }

    int PopNewest()
    {
        if (mSize == 0)
        {
            throw "Buffer is empty";
        }
        int value = mBuffer[mSize - 1];
        --mSize;
        return value;
    }

private:

    void resize(int newCapacity)
    {
        int* newBuffer = new int[newCapacity];
        for (int i = 0; i < mSize; i++)
        {
            newBuffer[i] = mBuffer[i];
        }
        delete[] mBuffer;
        mBuffer = newBuffer;
        mCapacity = newCapacity;
    }

    void shiftLeft(int index)
    {
        for (int i = index; i < mSize - 1; ++i)
        {
            mBuffer[i] = mBuffer[i + 1];
        }
    }

    int* mBuffer;
    int mCapacity;
    int mSize;



};

