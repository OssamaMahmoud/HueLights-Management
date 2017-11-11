
 //Created by ubuntu on 11/3/17.


#ifndef AUTH_MYAUTHWIDGET_H
#define AUTH_MYAUTHWIDGET_H


#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/Dbo/UserDatabase>
#include "User.h"


#include <Wt/Auth/AuthWidget>

 class Session;

 class AuthWidget : public Wt::Auth::AuthWidget
 {
 public:
     AuthWidget(Session& session);

     /* We will use a custom registration view */
     virtual Wt::WWidget *createRegistrationView(const Wt::Auth::Identity& id);

 private:
     Session& session_;

     void createLoggedInView();
 };



#endif //AUTH_MYAUTHWIDGET_H
