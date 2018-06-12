//
// Threaded.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sat Jan 21 18:42:18 2017 Adrien Vasseur
// Last update Sat Jan 21 18:42:18 2017 Adrien Vasseur
//

/*!
 * \file Threaded.hpp
 */

#pragma   once

#include  <thread>
#include  <atomic>

#include  "thread/IThread.hpp"

/*! \class Threaded
 *  \brief Threaded class implementation
 */

class     Threaded : public IThread
{
public:
  /*!
   *  \brief Threaded constructor
   */
  Threaded();

  /*!
   *  \brief Threaded destructor
   */
  ~Threaded();

  /*!
   *  \brief Start the thread (it will call the internal run until stop method is called)
   */
  void    start();

  /*!
   *  \brief Stop the thread
   */
  void    stop();

  /*!
   *  \brief Wait the thread to stop
   */
  void    join();

  bool    isRunning() const;

  /*!
   *  \brief Get the optimal number of threads needs by the hardware
   *  \return size(std::uint32_t) number of hardware threads
   */
  static std::uint32_t getHardwareThreads();

private:
  /*!
   *  \brief The internal run method, serve to call the implemented run method
   */
  void    _internalRun();

  /*!
   *  \brief Atomic to the thread pointer implementation (thread safe)
   */
  std::atomic<std::thread *>  _thread;

  /*!
   *  \brief Atomic to the running boolean (thread safe)
   */
  std::atomic<bool>           _running;
};