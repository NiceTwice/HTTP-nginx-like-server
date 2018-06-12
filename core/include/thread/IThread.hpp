//
// IThread.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sat Jan 21 18:40:08 2017 Adrien Vasseur
// Last update Sat Jan 21 18:40:08 2017 Adrien Vasseur
//

/*!
 * \file IThread.hpp
 */

#pragma   once

/*! \interface IThread
 *  \brief Interface for threaded class
 */

class     IThread
{
public:
  /*!
   *  \brief IThread destructor
   */
  virtual ~IThread() {}

  /*!
   *  \brief Start the thread (it will call the internal run until stop method is called)
   */
  virtual void  start() = 0;

  /*!
   *  \brief Stop the thread
   */
  virtual void  stop() = 0;

  virtual bool  isRunning() const = 0;

  /*!
   *  \brief Wait the thread to stop
   */
  virtual void  join() = 0;

protected:
  /*!
   *  \brief Internal run, has to be implemented in threaded class to be called infinitly
   */
  virtual void  run() = 0;
};