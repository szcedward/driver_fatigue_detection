#include <iostream>
#include "RingBuffer.h"

RingBuffer::RingBuffer(void) : size(7) {
  this->internalSize = size + 1;
  base = new int[internalSize];

  for (int i = 0; i < internalSize; i++) {
    base[i] = 0;
  }

  front = rear = 0;
}

RingBuffer::RingBuffer(int size) {
  this->size = size;
  this->internalSize = size + 1;
  base = new int[this->internalSize];

  for (int i = 0; i < internalSize; i++) {
    base[i] = 0;
  }

  front = rear = 0;
}

RingBuffer::~RingBuffer(void) {
  if (base != 0) {
    delete base;
  }
}

int RingBuffer::getSize() const {
  return size;
}

int RingBuffer::getLength() const {
  return (rear - front + internalSize) % internalSize;
}

const int* RingBuffer::getBase() const {
  return base;
}

int RingBuffer::deQueue(int* value) {
  if (front == rear) {
    return -1;
  }

  *value = base[front];
  front = (front + 1) % internalSize;
  return 0;
}

int RingBuffer::enQueue(int value) {
  if ((rear + 1) % internalSize == front) {
    return -1;
  }

  base[rear] = value;
  rear = (rear + 1) % internalSize;
  return 0;
}

void RingBuffer::put(int value, int* out) {
  if (getLength() == getSize()) {
    deQueue(out);
  } else {
    *out = -1;
  }

  enQueue(value);
}

void RingBuffer::put(int value) {
  int out;

  if (getLength() == getSize()) {
    deQueue(&out);
  }

  enQueue(value);
}

void RingBuffer::dumpQueue() {
  for (int i = 0; i < getSize(); i++) {
    std::cout << base[i] << " ";
  }

  std::cout << std::endl;
}

int RingBuffer::getRearIndex() {
  return rear;
}

bool RingBuffer::isFull() {
  return (this->getLength() == this->getSize());
}