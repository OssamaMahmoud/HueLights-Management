//
// Created by ubuntu on 11/7/17.
//

#ifndef AUTH_USERDETAILSMODEL_H
#define AUTH_USERDETAILSMODEL_H


#include <Wt/WFormModel>

class Session;

class UserDetailsModel : public Wt::WFormModel
{
public:
    static const Field fNameField, lNameField;

    UserDetailsModel(Session& session, Wt::WObject *parent = 0);

    void save(const Wt::Auth::User& user);

private:
    Session& _session;
};

#endif //AUTH_USERDETAILSMODEL_H
