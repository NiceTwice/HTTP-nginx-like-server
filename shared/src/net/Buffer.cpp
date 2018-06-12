//
// Buffer.cpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Fri Jan 20 22:16:50 2017 Adrien Vasseur
// Last update Fri Jan 20 22:16:50 2017 Adrien Vasseur
//

#include <cstring>
#include    "net/Buffer.hpp"

Buffer::Buffer()
{
  this->_pos = 0;
}

Buffer::~Buffer()
{
  this->clear();
}

Buffer::Data    *Buffer::createData(std::uint64_t size)
{
  char          *pure = new char[sizeof(Buffer::Data) + size];
  Buffer::Data  *data = reinterpret_cast<Buffer::Data *>(pure);

  data->size = size;
  data->data = &(pure[sizeof(Buffer::Data)]);
  return (data);
}

Buffer          &Buffer::operator<<(Buffer::Data *data)
{
  if (data->size > 0)
    this->_buffer.push_back(data);
  return (*this);
}

Buffer          &Buffer::operator>>(Buffer::Data *data)
{
  if (this->_buffer.size() > 0)
  {
    if (this->_pos >= this->_buffer.front()->size)
      this->_pos = 0;
    data->size = this->_buffer.front()->size - this->_pos;
    data->data = &(this->_buffer.front()->data[this->_pos]);
  }
  else
    data->size = 0;
  return (*this);
}

Buffer          &Buffer::operator>>(Buffer &output)
{
  if (this->_buffer.size() > 0)
  {
    output << this->_buffer.front();
    this->_buffer.pop_front();
  }
  return (*this);
}

void            Buffer::assemble()
{
  if (this->_buffer.size() > 1)
  {
    std::uint64_t totalSize = 0;
    Buffer::Data  *total;

    for (auto ite = this->_buffer.begin(); ite != this->_buffer.end(); ite++)
      totalSize += (*ite)->size;
    total = Buffer::createData(totalSize);
    totalSize = 0;
    for (auto ite = this->_buffer.begin(); ite != this->_buffer.end(); ite++)
    {
      memcpy(&(total->data[totalSize]), (*ite)->data, (*ite)->size);
      totalSize += (*ite)->size;
    }
    this->clear();
    this->_buffer.push_back(total);
  }
}

void            Buffer::clear()
{
  while (this->_buffer.size() > 0)
  {
    delete[] this->_buffer.front();
    this->_buffer.pop_front();
  }
}

void            Buffer::removeFront(std::uint64_t size)
{
  while (this->_buffer.size() > 0 && size > 0 && size >= this->_buffer.front()->size)
  {
    size -= this->_buffer.front()->size;
    delete[] this->_buffer.front();
    this->_buffer.pop_front();
  }
  if (this->_buffer.size() > 0 && size < this->_buffer.front()->size)
    this->_pos += size;
  if (this->_buffer.size() == 0 || this->_pos > this->_buffer.front()->size)
    this->_pos = 0;
}

bool            Buffer::hasData() const
{
  return (this->_buffer.size() != 0);
}