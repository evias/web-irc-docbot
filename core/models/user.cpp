#include "user.hpp"

#include <Wt/Dbo/Impl>
#include <Wt/WDateTime>
#include <Wt/Dbo/WtSqlTraits>

DBO_INSTANTIATE_TEMPLATES(evias::models::user);

using namespace evias;
using namespace models;

dbo::ptr<user_traits::account> user::getAccount(int idx)
{
    if (accounts.size() == 0)
        accounts = model::getSession().find<user_traits::account>()
            .where ("id_user = ?").bind(id_user);

    account_lt::const_iterator it = accounts.begin();
    for (int c = 0; it != accounts.end(); it++)
        if (c++ == idx)
            return *it;

    if (accounts.size() == 0)
        throw missing_user_data_exception("At least one public.user_account entry must exist.");

    return *(accounts.begin());
}

dbo::ptr<user_traits::profile> user::getProfile(int idx)
{
    if (profiles.size() == 0)
        profiles = model::getSession().find<user_traits::profile>()
            .where ("id_user = ?").bind(id_user);

    profile_lt::const_iterator it = profiles.begin();
    for (int c = 0; it != profiles.end(); it++)
        if (c++ == idx)
            return *it;

    if (profiles.size() == 0)
        throw missing_user_data_exception("At least one public.user_profiles entry must exist.");

    return *(profiles.begin());
}

dbo::ptr<user_traits::prefs> user::getPrefs(int idx)
{
    if (preferences.size() == 0)
        preferences = model::getSession().find<user_traits::prefs>()
            .where ("id_user = ?").bind(id_user);

    prefs_lt::const_iterator it = preferences.begin();
    for (int c = 0; it != preferences.end(); it++)
        if (c++ == idx)
            return *it;

    if (preferences.size() == 0)
        throw missing_user_data_exception("At least one public.user_prefs entry must exist.");

    return *(preferences.begin());
}

