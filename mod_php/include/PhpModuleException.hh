//
// PhpModuleException.hh for ZIA in /home/cugede/work/Epitech/rendu/TEK3/ZIA/cpp_zia/core/include/Exception/
//
// Made by Quentin Guerin
// Login <quentin.guerin@epitech.net>
//
// Started on  Tue Jan 31 16:48:42 2017 Quentin Guerin
// Last update Tue Jan 31 16:55:11 2017 Quentin Guerin
//

#pragma once

# include <stdexcept>
# include "api/Enum.hh"

class PhpModuleException: public std::exception {
protected:
    const std::string m_what;
    const apouche::StatusCode m_code;

public:
    /*!
    *  \brief Construct an PhpModuleException
    *
    *  Create an PhpModuleException
    *
    *  \param what : the error message
    */

    PhpModuleException(const std::string &what,
                       apouche::StatusCode code = apouche::StatusCode::UndefinedStatusCode);

    /*!
    *  \brief Destructor
    *
    *  Destructor of PhpModuleException
    */

    ~PhpModuleException() throw();

    /*!
    *  \brief Get the error message of the exception
    *
    *  Get the error message
    *
    *  \return const char * : The error message
    */

    const char *what() const throw();

    /*!
    *  \brief Get the error associated code for the exception
    *
    *  Get the error code
    *
    *  \return apouche::StatusCode : The error code
    */

    apouche::StatusCode const& code() const throw();
};