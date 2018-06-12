//
// Core.hpp for cpp_zia in /home/lanquemar/rendu/cpp_zia
//
// Made by Adrien Vasseur
// Login   <adrien.vasseur@epitech.eu>
//
// Started on  Sat Jan 21 19:20:26 2017 Adrien Vasseur
// Last update Sat Jan 21 19:20:26 2017 Adrien Vasseur
//

/*!
 * \file Core.hpp
 */

#pragma   once

#include  <vector>
#include  "net/TcpListener.hpp"
#include  "loader/ModuleFactory.hpp"
#include  "Worker.hpp"
#include "api/HttpConf/HttpConf.hpp"

/*! \class Core
 *  \brief Main entry for the ZIA project
 */

class     Core
{
public:
  /*!
   *  \brief ZIA Core constructor
   */
  Core();

  /*!
   *  \brief ZIA Core destructor
   */
  ~Core();

  /*!
   *  \brief Initialize ZIA Core
   *  \return True on success, false on failure
   */
  bool    init();

  /*!
   *  \brief Start to run the ZIA
   */
  void    run();

  /*!
   *  \brief Force the ZIA program to stop
   */
  static void   stop();

private:
  bool          listenAllPorts();
  bool          listenPort(std::uint16_t);

  std::vector<TcpListener *>  _servers;
  HttpConf                    _config;

  std::vector<std::uint16_t>    _listeningPorts;
  static std::vector<Worker *>  _workers; /*!< Workers list*/
};