//
// HttpRequest.cpp for ZIA in /home/cugede/work/Epitech/rendu/TEK3/ZIA/cpp_zia/core/include/api/HttpRequest
//
// Made by Quentin Guerin
// Login <quentin.guerin@epitech.net>
//
// Started on  Tue Jan 31 16:48:42 2017 Quentin Guerin
// Last update Tue Jan 31 16:55:11 2017 Quentin Guerin
//

#pragma once

#include "api/HttpRequest/IHttpRequest.hh"
#include "api/HttpHeader/HttpHeader.hh"
#include "api/HttpBody/HttpBody.hh"

/*! \namespace apouche
 *
 * namespace that contains all http component
 */

namespace apouche {

    /*! \class HttpRequest
    * \brief class that represent an HttpRequest
    *
    */

    class HttpRequest: public IHttpRequest {
    protected:
        IHttpHeader *m_header;
        IHttpBody *m_body;
        std::string m_version;
        apouche::Method m_method;
        std::string m_uri;
        std::map<std::string, std::string> m_uriParameters;
        const static std::map <apouche::Method,  std::string> m_mapMethodFromEnum;


    public:
        /*!
        *  \brief Construct an HttpRequest
        *
        *  Create an HttpRequest
        */

        HttpRequest();

        /*!
        *  \brief Destructor
        *
        *  Destructor of HttpRequest
        */

        ~HttpRequest();

        /*!
        *  \brief Get the method of the header
        *
        *  Get the method of the header
        *
        *  \return apouche::Method : The type of the request, maybe post, get, etc...
        */

        apouche::Method getMethod() const override;

        /*!
        *  \brief Set the method of the header
        *
        *  Set the method of the header
        *
        *  \param method : set the type of the request, maybe post, get, etc...
        *  \return void
        */

        void setMethod(apouche::Method) override;

        /*!
        *  \brief Get the uri
        *
        *  Get the uri
        *
        *  \return std::string : complete uri of the Header
        */

        const std::string &getURI() const override;

        /*!
        *  \brief Set the uri of the header
        *
        *  Set the uri of the header
        *
        *  \param uri : set the uri of the request
        */

        void setURI(const std::string &) override;
        /*!
        *  \brief Check if a get parameter is in the uri
        *
        *  Check if a get parameter is in the uri
        *
        *  \param key : a get parameter key in the uri
        *  \return bool : true if the key have a value
        */

        bool containsUriParameters(const std::string &) const override;

        /*!
        *  \brief Get all get parameters in the uri
        *
        *  Get all get parameters in the uri
        *
        *  \return std::map<std::string, std::string> : all get parameters in the uri
        */

        const std::map<std::string, std::string> &getUriParameters() const override;

        /*!
        *  \brief Set all get parameters in the uri
        *
        *  Set all get parameters in the uri
        *
        *  \param map : all get parameters in the uri
        */

        void setUriParameters(const std::map<std::string, std::string> &) override;

        /*!
        *  \brief Check if a get parameter is in the uri
        *
        *  Check if a get parameter is in the uri
        *
        *  \param key : a get parameter key in the uri
        *  \return std::string : value of the key
        */

        const std::string &getUriParameter(const std::string &key) const override;

        /*!
        *  \brief Set a get parameters in the uri
        *
        *  Set a get parameters in the uri
        *
        *  \param key : a get parameter key in the uri, value : a get parameters value in the uri
        *  \throw std::out_of_range : if the key doesn\'t exist
        */

        void setUriParameter(const std::string &, const std::string &) override;

        /*!
        *  \brief Get the request line of your request
        *
        *  Get the request line of your request
        *
        *  \return std::string : the request line
        */

        const std::string getRequestLine() override;

        /*!
        *  \brief Get the IHttpHeader of your request
        *
        *  Get the IHttpHeader of your request
        *
        *  \return apouche::IHttpHeader : Your header of the request
        */

        IHttpHeader *getHeaders() override;

        /*!
         * \brief Constant version of getHeaders()
         * @return apouche::IHttpHeader: Your header of the request or response
         */

        const IHttpHeader *getHeaders() const override;

        /*!
        *  \brief Delete the old header and set the IHttpHeader of your request
        *
        *  Set the IHttpHeader of your request
        *
        *  \param header : set the IHttpHeader of your request
        *  \return void
        */

        void setHeaders(IHttpHeader *header) override;

        /*!
        *  \brief Get the IHttpBody of your request, you can give you own implementation or use our implementation.
        *
        *  Get the IHttpBody of your request, you can give you own implementation or use our implementation.
        *
        *  \return apouche::IHttpBody: Your body of the request or response
        */

        IHttpBody *getBody() override;

        /*!
         * \brief Constant version of getBody()
         * @return apouche::IHttpBody: Your body of the request or response
         */

        const IHttpBody *getBody() const override;

        /*!
        *  \brief Delete the old body and set the IHttpBody of your request
        *
        *  Set the IHttpBody of your request
        *
        *  \param body : set the IHttpBody of your request
        *  \return void
        */

        void setBody(IHttpBody *body) override;

        /*!
        *  \brief Get the http version of your request.
        *
        *  Get the http version of your request.
        *
        *  \return std::string: The http version used for your request
        */

        const std::string &getVersion() const override;

        /*!
        *  \brief Set the http version of your request.
        *
        *  Set the http version of your request.
        *
        *  \param version, The http version for your request
        */

        void setVersion(const std::string &version) override;
    };
}