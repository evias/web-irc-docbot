#ifndef __EVIAS_DOCBOT_MODELS_USER__
#define __EVIAS_DOCBOT_MODELS_USER__

#include <Wt/WDateTime>
#include <Wt/Dbo/Types>
#include <Wt/Dbo/WtSqlTraits>

#include "model.hpp"
#include "user_data.hpp"

#include <string>

namespace dbo = Wt::Dbo;

namespace evias {
namespace models {

    typedef dbo::ptr<user_traits::account>  account_t;
    typedef dbo::ptr<user_traits::profile>  profile_t;
    typedef dbo::ptr<user_traits::prefs>    prefs_t;

    typedef dbo::collection<dbo::ptr<user_traits::account> >   account_lt;
    typedef dbo::collection<dbo::ptr<user_traits::profile> >   profile_lt;
    typedef dbo::collection<dbo::ptr<user_traits::prefs> >     prefs_lt;

    class user
    {
    public:
        int         id_user;
        std::string login;
        std::string password;
        std::string password_salt;
        Wt::WDateTime date_creation;
        Wt::WDateTime date_update;

        account_lt accounts;
        profile_lt profiles;
        prefs_lt   preferences;

        template <class action>
        void persist(action& a)
        {
            dbo::field(a, login, "login");
            dbo::field(a, password, "password");
            dbo::field(a, password_salt, "password_salt");
            dbo::field(a, id_user, "id_user");
            dbo::field(a, date_creation, "date_creation");
            dbo::field(a, date_update, "date_update");
        }

        account_t getAccount(int = 0);
        profile_t getProfile(int = 0);
        prefs_t   getPrefs(int = 0);
    };

    class missing_user_data_exception
        : public std::exception
    {
        std::string msg;
    public:
        missing_user_data_exception(std::string m)
            : msg(m) {};
        virtual ~missing_user_data_exception() throw() {};

        const char* what() const throw()
        {
            std::stringstream s;
            s << "Some user data is missing in the database: "
              << msg;
            return s.str().c_str();
        }
    };

}
}

/**
 * Wt::Dbo::dbo_straits specializations
 *
 * @brief
 * change primary key names and disable version fields.
 **/

namespace Wt {
namespace Dbo {

    using namespace evias::models;

    template<>
    struct dbo_traits<user>
        : public dbo_default_traits
    {
        static const char *surrogateIdField()
        {
            return "id_user";
        }

        static const char *versionField()
        {
            return 0;
        }
    };

}
}

DBO_EXTERN_TEMPLATES(evias::models::user);

#endif /** __EVIAS_CODEREVIEW_MODELS_USER__ **/
