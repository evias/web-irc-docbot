#include "commands.hpp"

#include <Wt/Dbo/Impl>

using namespace std;
using namespace evias;

bool bot_user_auth::operator()(string user, string pass)
{
    dbo::Transaction trx(model::getSession());

    // first get the password salt
    dbo::Query<string> salt_query = model::getSession().query<string>(
                    "select u.password_salt from public.user u")
                    .where("login = ?").bind(user);

    string salt = salt_query.resultValue();

    // now check credentials provided
    dbo::Query<int> cred_query = model::getSession().query<int>(
                    "select id_user from public.user u")
                    .where("login = ?").bind(user)
                    // prepend password to salt and md5() the result.
                    .where("password = md5(?)").bind(salt.insert(0, pass));

    int user_id = cred_query.resultValue();
    if ((bool) user_id)
        user_ = model::getSession().find<__m::user>()
            .where("id_user = ?").bind(user_id);

    return (bool) user_id;
}

bool bot_user_auth::isset()
{
    return (bool) user_;
}

