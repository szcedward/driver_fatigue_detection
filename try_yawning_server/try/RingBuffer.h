#pragma once
class RingBuffer
{
private:
  int size;
  int internalSize;
  int* base;
  int front;
  int rear;
public:
  RingBuffer(void);
  RingBuffer(int size);
  ~RingBuffer(void);
  int getSize() const;
  int getLength() const;
  const int* getBase() const;
  int enQueue(int value);
  int deQueue(int* value);
  void put(int value, int* out);
  void put(int value);
  int getRearIndex();
  void dumpQueue();
  bool isFull();
};

