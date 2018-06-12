//
// Buffer.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Fri Jan 20 22:16:17 2017 Adrien Vasseur
// Last update Fri Jan 20 22:16:17 2017 Adrien Vasseur
//

/*!
 * \file Buffer.hpp
 */

#pragma   once

#include  <cstdint>
#include  <list>

/*! \class Buffer
 *  \brief Ring buffer in order to save all input/output data.
 */

class     Buffer
{
public:
  /*! \struct Buffer::Data
   *  \brief Some Data into the Buffer, it is a node of his internal list.
   */
  struct   Data
  {
    /*!
     *  \brief Data size of the buffer node
     */
    std::uint64_t   size;

    /*!
     *  \brief Pure data into the Data buffer node
     */
    char            *data;
  };

  /*!
   *  \brief Buffer constructor
   */
  Buffer();

  /*!
   *  \brief Buffer destructor
   */
  ~Buffer();

  /*!
   *  \brief Permit to create a new Buffer::Data node of size param1.
   *  \param size(std::uint64_t) : take the block size in parameter.
   *  \return Return a new Buffer::Data pointer.
   */
  static Data *createData(std::uint64_t);

  /*!
   *  \brief Push a new Buffer::Data node at the end of the buffer.
   *  \param node(Buffer::Data *) : take the input block in parameter.
   *  \return Return itself.
   */
  Buffer  &operator<<(Data *);

  /*!
   *  \brief Get first Buffer::Data node on the top of the buffer.
   *  \param node(Buffer::Data *) : take the output block in parameter.
   *  \return Return itself.
   */
  Buffer  &operator>>(Data *);

  /*!
   *  \brief Put first Buffer::Data node on the top of the parameter buffer.
   *  \param buffer(Buffer &) : take the output buffer in parameter.
   *  \return Return itself.
   */
  Buffer  &operator>>(Buffer &);

  /*!
   *  \brief Remove param1 bytes at the beginning of the buffer.
   *  \param size(std::uint64_t) : the size to remove.
   */
  void    removeFront(std::uint64_t);

  void    assemble();

  void    clear();

  /*!
   *  \brief Does the buffer is actually containing data ?
   *  \return Return true if the buffer is containing data, false otherwise.
   */
  bool    hasData() const;

private:
  /*!
   *  \brief Buffer internal node list
   */
  std::list<Data *> _buffer;

  /*!
   *  \brief Buffer actually position writer on the first node of the list
   */
  std::uint64_t     _pos;
};