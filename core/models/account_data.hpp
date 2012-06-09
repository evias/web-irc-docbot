#ifndef __EVIAS_DOCBOT_MODELS_ACCOUNT_TRAITS__
#define __EVIAS_DOCBOT_MODELS_ACCOUNT_TRAITS__

#include <Wt/Dbo/Types>

namespace dbo = Wt::Dbo;

namespace evias {
namespace models {
namespace account_traits {

    class default_traits
    {
    public:

        template <class action>
        void persist(action& a)
        {
        }
    };

    class account_type
        : public default_traits
    {
    public:
        std::string label;

        template <class action>
        void persist(action& a)
        {
            dbo::field(a, label, "label");
        }
    };

}
}
}

DBO_EXTERN_TEMPLATES(evias::models::account_traits::account_type);

#endif /** __EVIAS_DOCBOT_MODELS_ACCOUNT_TRAITS__ **/
