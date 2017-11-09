//
// Created by ubuntu on 11/7/17.
//

#ifndef AUTH_BRIDGE_H
#define AUTH_BRIDGE_H
#include <Wt/Dbo/Dbo>
#include <string>
#include "User.h"

namespace dbo = Wt::Dbo;

class User;

class Bridge {
public:
    //add other stuff here I guess.
    dbo::ptr<User> user;

    template<class Action>
    void persist(Action& a)
    {
        dbo::belongsTo(a, user, "user");
    }

};


#endif //AUTH_BRIDGE_H
