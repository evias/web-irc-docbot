#ifndef __EVIAS_DOCBOT_MODELS_ABSTRACT__
#define __EVIAS_DOCBOT_MODELS_ABSTRACT__

#include <Wt/Dbo/Types>
#include <Wt/Dbo/WtSqlTraits>
#include <Wt/Dbo/backend/Postgres>
#include <Wt/Dbo/Session>

#include <string>
#include <sstream>

#include "core/utilities.hpp"

namespace dbo = Wt::Dbo;

namespace evias {

    class model
    {
        static std::string              conn_policy;
        static bool                     initialized;
        static dbo::backend::Postgres*  adapter;

    public :
        // db_session declared public to allow instantiation from outside the class.
        // Session(const Session&) is private, the purpose of publishing
        // this variable is to keep a valid and efficient
        // model / view / controller pattern.
        static dbo::Session*            db_session;

        static void create(std::string = "");
        static void destroy();

        static bool setPolicy(std::string);

        static dbo::backend::Postgres&  getAdapter();
        static dbo::Session&            getSession();
    };

    class db_notready_exception
        : public std::exception
    {
        std::string msg;
    public:
        db_notready_exception(std::string m)
            : msg(m) {};
        virtual ~db_notready_exception() throw() {};

        const char* what() const throw()
        {
            std::stringstream s;
            s << "The database object is not configured, error message: "
              << msg;
            return s.str().c_str();
        }
    };

    class db_connection_exception
        : public std::exception
    {
        std::string msg;
    public:
        db_connection_exception(std::string m)
            : msg(m) {};
        virtual ~db_connection_exception() throw() {};

        const char* what() const throw()
        {
            std::stringstream s;
            s << "An error ocurred while trying to establish the connection: "
              << msg;
            return s.str().c_str();
        }
    };

}

#endif /** __EVIAS_DOCBOT_MODELS_ABSTRACT__ **/
