#ifndef __EVIAS_DOCBOT_WEB_INTERFACE__
#define __EVIAS_DOCBOT_WEB_INTERFACE__

#include "interface.hpp"
#include <boost/shared_ptr.hpp>
#include <Wt/WContainerWidget>

namespace evias {

    class interface
        : public Wt::WContainerWidget
    {
    private:
    // PREVENT deletion with px.get()
        interface(Wt::WContainerWidget*);
        ~interface();
        class deleter;
        friend class deleter;

        class deleter
        {
        public:
            void operator() (interface* p)
            {
                delete p;
                p = NULL;
            }

        };

    public:
        static boost::shared_ptr<interface> create(Wt::WContainerWidget* root)
        {
            boost::shared_ptr<interface> shared(new interface(root), interface::deleter());
            return shared;
        }
    };

}

#endif
