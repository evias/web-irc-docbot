#include "model.hpp"

#include <Wt/Dbo/Impl>

#include <exception>
#include <sstream>

using namespace evias;

std::string model::conn_policy = "";
bool model::initialized = false;
dbo::backend::Postgres* model::adapter = NULL;
dbo::Session* model::db_session = NULL;

void model::create(std::string c_policy)
{
    if (c_policy.empty() && conn_policy.empty())
        throw db_notready_exception("You have to specify a connection policy.");
    else if (! c_policy.empty())
        setPolicy(c_policy);

    // initialize db_adapter
    getAdapter();
}

void model::destroy()
{
    if (adapter != NULL)
        mem_rm(adapter);

    if (db_session != NULL)
        mem_rm(db_session);
}

bool model::setPolicy(std::string p)
{
    conn_policy = p;
}

dbo::backend::Postgres& model::getAdapter()
{
    if (! initialized || adapter == NULL) {
        initialized = true;

        try {
            // try and connect with set conn_policy
            adapter = new dbo::backend::Postgres();
            if (! adapter->connect(conn_policy))
                throw db_connection_exception("Connection could not persist.");
        }
        catch (std::exception &e) {
            if (adapter != NULL)
                mem_rm(adapter);

            // forward exception with message specialization
            throw db_connection_exception(e.what());
        }
    }

    return *adapter;
}

dbo::Session& model::getSession()
{
    return *db_session;
}

