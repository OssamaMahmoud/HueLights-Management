
 //Created by ubuntu on 11/3/17.


#ifndef AUTH_MYAUTHWIDGET_H
#define AUTH_MYAUTHWIDGET_H

#include <string.h>
#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>
#include <Wt/Auth/Dbo/UserDatabase>
#include <Wt/WBootstrapTheme>
#include <Wt/WTable>
#include <Wt/WMessageBox>
#include "User.h"
#include "RegistrationView.h"
#include "Session.h"
#include "Bridge.h"

 class Session;

 class AuthWidget : public Wt::Auth::AuthWidget
 {
     public:
         AuthWidget(Session& session);

         /* We will use a custom registration view */
         virtual Wt::WWidget *createRegistrationView(const Wt::Auth::Identity& id);

     private:
         Session& session_;
         Bridge bridge_;
         Wt :: WLineEdit *bridgeAddress_, *bridgePort_, *bridgeReference_;
         Wt :: WTable *choosePage,*table_;
         Wt :: WPushButton  *buttonChoose;
         string address, port, reference;
         bool testBridge = false;
         void createLoggedInView();
         void MainPage();
         void ConnectToBridge();
         void lightPage();
         void schedulePage();
         void groupPage();
 };



#endif //AUTH_MYAUTHWIDGET_H
