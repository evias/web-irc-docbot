#ifndef __EVIAS_DOCBOT_MODELS_USER_DATA__
#define __EVIAS_DOCBOT_MODELS_USER_DATA__

#include <Wt/Dbo/Types>
#include <Wt/Dbo/WtSqlTraits>
#include <Wt/WDateTime>

#include "account_data.hpp"

namespace dbo = Wt::Dbo;

namespace evias {
namespace models {

namespace user_traits {

    class default_traits
    {
    public:

        template <class action>
        void persist(action& a)
        {
        }
    };

    class account
        : public default_traits
    {
    public:
        int             id_user;
        int             id_account_type;
        Wt::WDateTime   date_creation;
        Wt::WDateTime   date_update;

        template <class action>
        void persist(action& a)
        {
            default_traits::persist(a);

            dbo::field(a, id_user, "id_user");
            dbo::field(a, id_account_type, "id_account_type");
            dbo::field(a, date_creation, "date_creation");
            dbo::field(a, date_update, "date_update");
        }
    };

    class profile
        : public default_traits
    {
    public:
        int             id_user;
        std::string    first_name;
        std::string    last_name;
        std::string    email;
        Wt::WDateTime date_creation;
        Wt::WDateTime date_update;

        template <class action>
        void persist(action& a)
        {
            default_traits::persist(a);

            dbo::field(a, id_user, "id_user");
            dbo::field(a, first_name, "first_name");
            dbo::field(a, last_name, "last_name");
            dbo::field(a, email, "email");
            dbo::field(a, date_creation, "date_creation");
            dbo::field(a, date_update, "date_update");
        }
    };

    class prefs
        : public default_traits
    {
    public:
        int             id_user;
        std::string     name;
        std::string     value;
        Wt::WDateTime   date_creation;
        Wt::WDateTime   date_update;

        template <class action>
        void persist(action& a)
        {
            default_traits::persist(a);

            dbo::field(a, id_user, "id_user");
            dbo::field(a, name, "name");
            dbo::field(a, value, "value");
            dbo::field(a, date_creation, "date_creation");
            dbo::field(a, date_update, "date_update");
        }
    };

}
}
}

/**
 * Wt::Dbo::dbo_traits specializations
 *
 * @brief
 * redefine primary keys and disable version field.
 **/
namespace Wt {
namespace Dbo {

    using namespace evias::models::user_traits;

    /**
     *
     * dbo_traits specializations
     *
     **/

    template<>
    struct dbo_traits<account>
        : public dbo_default_traits
    {
        static const char *surrogateIdField()
        {
            return "id_account";
        }

        static const char *versionField()
        {
            return 0;
        }
    };

    template<>
    struct dbo_traits<profile>
        : public dbo_default_traits
    {
        static const char *surrogateIdField()
        {
            return "id_profile";
        }

        static const char *versionField()
        {
            return 0;
        }
    };

    template<>
    struct dbo_traits<prefs>
        : public dbo_default_traits
    {
        static const char *surrogateIdField()
        {
            return "id_prefs";
        }

        static const char *versionField()
        {
            return 0;
        }
    };

}
}

DBO_EXTERN_TEMPLATES(evias::models::user_traits::account);
DBO_EXTERN_TEMPLATES(evias::models::user_traits::profile);
DBO_EXTERN_TEMPLATES(evias::models::user_traits::prefs);

#endif /** __EVIAS_CODEREVIEW_MODELS_USER_TRAITS__ **/
