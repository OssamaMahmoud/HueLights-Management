//
// Created by ubuntu on 11/7/17.
//

#ifndef AUTH_REGISTRATIONVIEW_H
#define AUTH_REGISTRATIONVIEW_H


#include <Wt/Auth/RegistrationWidget>

class Session;
class UserDetailsModel;

/*
 * @brief creates the registrantion view by overriding the parents view
 * @author Ossama
 *
 */
class RegistrationView : public Wt::Auth::RegistrationWidget
{
public:
    RegistrationView(Session& session, Wt::Auth::AuthWidget *authWidget = 0);

    /* specialize to create user details fields */
    virtual Wt::WWidget *createFormWidget(Wt::WFormModel::Field field);

protected:
    /* specialize to also validate the user details */
    virtual bool validate();

    /* specialize to register user details */
    virtual void registerUserDetails(Wt::Auth::User& user);

private:
    Session& session_;

    UserDetailsModel *detailsModel_;
};


#endif //AUTH_REGISTRATIONVIEW_H
